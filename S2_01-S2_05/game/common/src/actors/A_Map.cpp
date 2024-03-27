#include "A_Map.h"
#include "A_Joueur.h"
#include "A_Camera.h"
#include "A_GUI.h"

#if defined(__ANDROID__) || defined(SIMULATE_ANDROID)
#include "A_AndroidCtrl.h"
#endif

#include <fstream>

#include <json.hpp>
using json = nlohmann::json;

A_Map* A_Map::instance = 0;

Actor* A_Map::make() {
	instance = new A_Map();
	return instance;
}

int A_Map::onCreate() {
	this->drawOrder = -1;

	this->currentFilename = "castle.rsmc";
	this->currentEntranceID = 2;
	this->events = 0;

	if (this->settings) {
		this->currentFilename = "a.rsmc";
		this->currentEntranceID = this->settings;
	}
	else {
		if (SaveMgr::instance->KeyExists("mapFilename")) {
			this->currentFilename = SaveMgr::instance->GetValueStr("mapFilename");
		}

		if (SaveMgr::instance->KeyExists("mapEntranceID")) {
			this->currentEntranceID = *SaveMgr::instance->GetValue<int>("mapEntranceID");
		}

		if (SaveMgr::instance->KeyExists("mapEvents")) {
			this->events = *SaveMgr::instance->GetValue<u64>("mapEvents");
		}
	}

	this->Load(this->currentFilename, this->currentEntranceID);

	return 1;
}

bool A_Map::Load(std::string filename, int entranceID) {
	bool isFirstTime = false;

	if (this->settings)
		entranceID = this->settings;
	else {
		if (!SaveMgr::instance->KeyExists("mapFilename") || !SaveMgr::instance->KeyExists("mapEntranceID"))
			isFirstTime = true;

		SaveMgr::instance->AddValueCC("mapFilename", filename.c_str());
		SaveMgr::instance->AddValue("mapEntranceID", entranceID);
	}

	this->currentFilename = filename;
	this->currentEntranceID = entranceID;

	/* Unloading current map with fade */
	if (this->mapLoaded) {
		if (this->loadFade < 0) {
			Game::instance->stopMusic();
			this->loadFade = loadTime;
			return false;
		}
		else if (this->loadFade > 0)
			return false;
	}

	this->DeleteElems();

	/* Map loading */

	char* mapFile;
	int mapFileLen = LoadFileAndAllocate(("assets/Maps/" + filename).c_str(), &mapFile);
	json mapData = json::parse(mapFile, mapFile + mapFileLen);
	delete[] mapFile;

	json jtilesets = mapData["tilesets"];
	u32 nbTilesets = (u32)jtilesets.size();

	this->tilesets = new Tileset[nbTilesets];
	std::map<int, Tileset*> tsMap;

	for (u32 tID = 0; tID < nbTilesets; tID++) {
		json jtileset = jtilesets[tID];
		std::string tsFilename = jtileset["filename"].get<std::string>();
		int tsID = jtileset["id"].get<int>();

		Tileset* tileset = &this->tilesets[tID];
		tsMap[tsID] = tileset;

		char* tilesetFile;
		int tilesetFileLen = LoadFileAndAllocate(("assets/Maps/" + tsFilename).c_str(), &tilesetFile);
		json tilesetData = json::parse(tilesetFile, tilesetFile + tilesetFileLen);
		delete[] tilesetFile;

		tileset->texture = IMG_LoadTexture(Game::instance->renderer, ("assets/Maps/" + tilesetData["filename"].get<std::string>()).c_str());

		json jtiles = tilesetData["tiles"];
		u32 nbChildren = (u32)jtiles.size();
		int maxTileID = 0;
		for (u32 tileIdx = 0; tileIdx < nbChildren; tileIdx++) {
			int tileID = jtiles[tileIdx]["id"].get<int>();
			if (tileID > maxTileID) {
				maxTileID = tileID;
			}
		}

		tileset->tiles = new Tile[maxTileID + 1];
		for (u32 tileIdx = 0; tileIdx < nbChildren; tileIdx++) {
			int tileID = jtiles[tileIdx]["id"].get<int>();
			int tileX = jtiles[tileIdx]["x"].get<int>();
			int tileY = jtiles[tileIdx]["y"].get<int>();
			int tileW = jtiles[tileIdx]["w"].get<int>();
			int tileH = jtiles[tileIdx]["h"].get<int>();
			int flags = jtiles[tileIdx]["f"].get<int>();
			bool tileS = flags & 1;
			bool tileO = flags & 2;
			bool tileC = flags & 4;

			Tile* tile = &tileset->tiles[tileID];
			tile->x = tileX;
			tile->y = tileY;
			tile->w = tileW;
			tile->h = tileH;
			tile->solid = tileS;
			tile->overPlayer = tileO;
			tile->overrideColl = tileC;
		}
	}

	json joriginpoint = mapData["originpoint"];
	this->originPoint.x = joriginpoint["x"].get<int>();
	this->originPoint.y = joriginpoint["y"].get<int>();

	json jsize = mapData["size"];
	this->w = jsize["w"].get<int>();
	this->h = jsize["h"].get<int>();

	json jtilemaps = mapData["tilemaps"];
	this->layerCount = (u32)jtilemaps.size();

	this->tilemaps = new TileInfo*[this->layerCount];
	this->tileCounts = new u32[this->layerCount];

	this->bounds = { INT_MAX, INT_MAX, INT_MIN, INT_MIN };

	for (u32 layer = 0; layer < this->layerCount; layer++) {
		json jtilemap = jtilemaps[layer];
		u32 nbTileInfos = this->tileCounts[layer] = (u32)jtilemap.size();

		this->tilemaps[layer] = new TileInfo[nbTileInfos];

		for (u32 tInfoIdx = 0; tInfoIdx < nbTileInfos; tInfoIdx++) {
			json jtileinfo = jtilemap[tInfoIdx];
			TileInfo* tInfo = &this->tilemaps[layer][tInfoIdx];

			if (jtileinfo.size() == 0) {
				tInfo->tileset = NULL;
				tInfo->tile = NULL;
			}
			else {
				tInfo->tileset = tsMap[jtileinfo["ts"].get<int>()];
				tInfo->tile = &tInfo->tileset->tiles[jtileinfo["tl"].get<int>()];

				int x = (((tInfoIdx % w) + originPoint.x) * tilesize) - (tilesize / 2);
				int y = (((tInfoIdx / w) + originPoint.y) * tilesize) - (tilesize / 2);
				int w = x + tilesize;
				int h = y + tilesize;

				if (x < this->bounds.x)
					this->bounds.x = x;
				if (y < this->bounds.y)
					this->bounds.y = y;
				if (w > this->bounds.w)
					this->bounds.w = w;
				if (h > this->bounds.h)
					this->bounds.h = h;
			}
		}
	}

	this->bounds.w -= this->bounds.x;
	this->bounds.h -= this->bounds.y;

	if (!this->settings) {
		Actor* invMgr = this->actorMgr.createActorByID(7); // Inventaire
		invMgr->activity &= ~ACTOR_ACTIVITY_DRAW;
	}

	Entrance* matchingEntrance = NULL;

	json jspecials = mapData["special"];
	u32 nbSpecials = (u32)jspecials.size();
	for (u32 spIdx = 0; spIdx < nbSpecials; spIdx++) {
		json jspecial = jspecials[spIdx];

		SpecialTile* spTile = NULL;

		SDL_Point tPos;
		json jspposition = jspecial["position"];
		tPos.x = jspposition["x"].get<int>();
		tPos.y = jspposition["y"].get<int>();

		json jspsettings = jspecial["settings"];
		switch (jspecial["id"].get<int>()) {
			case SPETILE_WARP:
			{
				Warp* warp = new Warp;
				warp->filename = jspsettings["Filename"].get<std::string>();
				warp->entranceID = jspsettings["EID"].get<int>();

				warps.push_back(warp);

				spTile = warp;
				break;
			}
			case SPETILE_COLLOVERRIDE:
			{
				CollOverride* collOverride = new CollOverride;
				collOverride->eventIDEnable = jspsettings["EventIDE"].get<int>();
				collOverride->eventIDDisable = jspsettings["EventIDD"].get<int>();
				collOverride->solid = jspsettings["Solid"].get<bool>();

				collOverrides.push_back(collOverride);

				spTile = collOverride;
				break;
			}
			case SPETILE_ENTRANCE:
			{
				Entrance* entrance = new Entrance;
				entrance->id = jspsettings["ID"].get<int>();
				entrance->direction = jspsettings["Direction"].get<int>();

				if (entrance->id == entranceID)
					matchingEntrance = entrance;

				entrances.push_back(entrance);

				spTile = entrance;
				break;
			}
			case SPETILE_PICKUPITEM:
			{
				int uniqueID = jspsettings["UID"].get<int>();
				int type = jspsettings["Type"].get<int>();
				int triggeringEventID = jspsettings["TEID"].get<int>();

				int settings = ((triggeringEventID & 0xFF) << 24) | ((type & 0xFF) << 16) | (uniqueID & 0xFFFF);

				this->actorMgr.createActorByID(8, &tPos, settings); // Item ramassable
				break;
			}
			case SPETILE_PNJ:
			{
				int type = jspsettings["Type"].get<int>();
				int direction = jspsettings["Direction"].get<int>();
				int triggeringEventID = jspsettings["TEID"].get<int>();

				int settings = ((direction & 0xFF) << 24) | ((type & 0xFF) << 16) | (triggeringEventID & 0xFFFF);

				this->actorMgr.createActorByID(17, &tPos, settings); // PNJ
				break;
			}
			case SPETILE_CACTUS:
			{
				this->actorMgr.createActorByID(18, &tPos); // Cactus
				break;
			}
			case SPETILE_CHEST:
			{
				int inventoryID = jspsettings["InvID"].get<int>();

				this->actorMgr.createActorByID(10, &tPos, inventoryID); // Coffre
				break;
			}
			default:
			{
				break;
			}
		}

		if (spTile) {
			json jspposition = jspecial["position"];
			spTile->tilePos = tPos;
		}
	}

	SDL_Point playerTilePos = { 0, 0 };
	int playerDirection = 1;
	if (matchingEntrance) {
		playerTilePos = matchingEntrance->tilePos;
		playerDirection = matchingEntrance->direction;
	}

	Actor* tb = this->actorMgr.createActorByID(6); // TextBox
	tb->activity &= ~ACTOR_ACTIVITY_DRAW;

	u32 playerSettings = A_Joueur::PlayerSettings::PLAYER_SETTINGS_NONE;
	if(this->settings)
		playerSettings = A_Joueur::PlayerSettings::PLAYER_SETTINGS_TITLESCREEN;
	else if (isFirstTime)
		playerSettings = A_Joueur::PlayerSettings::PLAYER_SETTINGS_FIRSTTIME;

	A_Joueur* player = ((A_Joueur*)this->actorMgr.createActorByID(2, &playerTilePos, playerSettings)); // Joueur
	player->direction = playerDirection; // Direction du joueur
	if (this->settings)
		player->activity = ACTOR_ACTIVITY_NONE;

	this->actorMgr.createActorByID(4); // Caméra

	if (!this->settings) {
		json mapMusic = mapData["music"];
		int musicID = mapMusic.get<int>();
		Game::instance->playMusic(musicID);

		this->loadFade = loadTime;
		if (A_GUI::instance == NULL) {
			Actor* gui = this->actorMgr.createActorByID(11); // GUI
			gui->activity &= ~ACTOR_ACTIVITY_DRAW;
		}

		#if defined(__ANDROID__) || defined(SIMULATE_ANDROID)
		if (A_AndroidCtrl::instance == NULL)
			Game::instance->actorMgr.createActorByID(12); // AndroidCtrl
		#endif
	}

	Game::instance->gameover = false;

	return true;
}

void A_Map::DeleteElems() {
	/* Clearing attached points and actors */
	occupiedTiles.clear();
	actorsInt.clear();

	/* Deleting children actors */
	this->actorMgr.deleteActors();

	/* Deleting special tiles */
	u32 nbWarps = (u32)warps.size();
	for (u32 i = 0; i < nbWarps; i++) {
		delete warps[i];

	}
	warps.clear();

	u32 nbCollOverrides = (u32)collOverrides.size();
	for (u32 i = 0; i < nbCollOverrides; i++) {
		delete collOverrides[i];

	}
	collOverrides.clear();

	u32 nbEntrances = (u32)entrances.size();
	for (u32 i = 0; i < nbEntrances; i++) {
		delete entrances[i];

	}
	entrances.clear();
}

int A_Map::onDelete() {
	this->DeleteElems();

	instance = 0;

	return 1;
}

int A_Map::onExecute() {
	if(this->loadFade == -1 && this->mapLoaded)
		this->actorMgr.handleActors();

	if (loadFade >= 0) {
		if (loadFade == 0) {
			if (this->mapLoaded)
				this->Load(currentFilename, currentEntranceID);

			this->mapLoaded = !this->mapLoaded;

		}

		loadFade--;

	}

	return 1;
}

int A_Map::onDraw() {
	if (loadFade != loadTime) {
		#ifdef _DEBUG
		SDL_Color pColor;
		SDL_GetRenderDrawColor(Game::instance->renderer, &pColor.r, &pColor.g, &pColor.b, &pColor.a);

		if (Game::instance->debugMode) {
			SDL_SetRenderDrawColor(Game::instance->renderer, 255, 0, 0, 64);
		}
		#endif

		Render(false);
		this->actorMgr.renderActors();
		Render(true);

		if (A_GUI::instance)
			A_GUI::instance->onDraw();

		if (A_TextBox::instance)
			A_TextBox::instance->onDraw();

		if (A_InvMgr::instance)
			A_InvMgr::instance->onDraw();

		#ifdef _DEBUG
		if (Game::instance->debugMode) {
			for (int i = 0; i < occupiedTiles.size(); i++) {
				SDL_Rect r = { (occupiedTiles[i]->x * tilesize) - (tilesize / 2), (occupiedTiles[i]->y * tilesize) - (tilesize / 2), tilesize, tilesize };
				Game::RenderFillRect(&r);
			}

			SDL_SetRenderDrawColor(Game::instance->renderer, pColor.r, pColor.g, pColor.b, pColor.a);
		}
		#endif
	}

	if (loadFade >= 0) {
		float fadeOpa = (loadFade / (float)loadTime) * 255.0f;
		if (mapLoaded)
			fadeOpa = 255.0f - fadeOpa;

		int rOpa = (int)round(fadeOpa);
		
		if (rOpa > 0) {
			SDL_SetRenderDrawColor(Game::instance->renderer, 0, 0, 0, rOpa);

			Game::RenderFillRect((SDL_Rect*)NULL);
		}
	}

	return 1;
}

bool DoesRectsOverlap(SDL_Rect* rect1, SDL_FRect* rect2) {
	if (rect1 == NULL || rect2 == NULL)
		return false;

	return IsRectInRect(rect1->x, rect1->y, rect1->x + rect1->w, rect1->y + rect1->h, rect2->x, rect2->y, rect2->x + rect2->w, rect2->y + rect2->h);

	/*
	float rec1x2 = (float)(rect2->x + rect2->w);
	float rec1y2 = (float)(rect2->y + rect2->h);
	float rec2x2 = (float)(rect1->x + rect1->w);
	float rec2y2 = (float)(rect1->y + rect1->h);

	bool widthIsPositive = SDL_min(rec1x2, rec2x2) > SDL_max(rect2->x, rect1->x);
	bool heightIsPositive = SDL_min(rec1y2, rec2y2) > SDL_max(rect2->y, rect1->y);
	return (widthIsPositive && heightIsPositive);
	*/
}

void A_Map::Render(bool isOver) {
	SDL_FRect winBounds = { 0, 0, (float)Game::instance->ww, (float)Game::instance->wh };

	SDL_FRect ogRect = {
		(originPoint.x * tilesize) - ((float)tilesize / 2),
		(originPoint.y * tilesize) - ((float)tilesize / 2),
		(float)tilesize,
		(float)tilesize
	};

	A_Camera::instance->ConvertWorldRectToScreenRectF(&ogRect, &ogRect);
	int tsW = (int)SDL_round(ogRect.w);
	int tsH = (int)SDL_round(ogRect.h);

	for (u32 layer = 0; layer < this->layerCount; layer++) {
		TileInfo* tilemap = tilemaps[layer];

		int nbTileInfos = this->tileCounts[layer];

		for (int tInfoIdx = 0; tInfoIdx < nbTileInfos; tInfoIdx++) {
			TileInfo* tInfo = &tilemap[tInfoIdx];

			if (tInfo->tileset == NULL || tInfo->tile == NULL || tInfo->tile->overPlayer != isOver)
				continue;

			float x = ogRect.x + ((tInfoIdx % w) * ogRect.w);
			float y = ogRect.y + ((tInfoIdx / w) * ogRect.h);

			SDL_Rect srcRect = { tInfo->tile->x, tInfo->tile->y , tInfo->tile->w , tInfo->tile->h };
			SDL_Rect dstRect = { (int)SDL_round(x), (int)SDL_round(y), tsW, tsH };

			if (!DoesRectsOverlap(&dstRect, &winBounds))
				continue;

			SDL_RenderCopy(Game::instance->renderer, tInfo->tileset->texture, &srcRect, &dstRect);

			#ifdef _DEBUG

			int gx = (tInfoIdx % w) + originPoint.x;
			int gy = (tInfoIdx / w) + originPoint.y;

			if (Game::instance->debugMode) {
				int overstate = 0;
				u32 nbCollOverides = (u32)collOverrides.size();
				for (u32 i = 0; i < nbCollOverides; i++)
				{
					CollOverride* collOverride = collOverrides[i];
					if (collOverride->IsActive() && (gx == collOverride->tilePos.x) && (gy == collOverride->tilePos.y)) {
						overstate = collOverride->solid + 1;
						break;
					}
				}

				if (overstate != 1 && (overstate == 2 || tInfo->tile->solid)) {
					SDL_RenderFillRect(Game::instance->renderer, &dstRect);
				}
			}
			#endif
		}
	}
}

TileInfo* A_Map::GetTileAtPos(const SDL_Point* tilePos, int layer) {
	TileInfo* tilemap = tilemaps[layer];

	int nbTileInfos = this->tileCounts[layer];
	int i = ((tilePos->y - originPoint.y) * w) + (tilePos->x - originPoint.x);

	if (i >= nbTileInfos)
		return NULL;

	return &tilemap[i];
}


bool A_Map::IsTileAvailable(const SDL_Point* tilePos) {
	#ifdef _DEBUG
	if (Game::instance->debugMode) {
		return true;
	}
	#endif

	u32 nbCollOverides = (u32)collOverrides.size();
	for (u32 i = 0; i < nbCollOverides; i++)
	{
		CollOverride* collOverride = collOverrides[i];
		if (collOverride->IsActive() && (tilePos->x == collOverride->tilePos.x) && (tilePos->y == collOverride->tilePos.y))
			return !collOverride->solid;
	}

	bool available = true;
	for (u32 layer = 0; layer < this->layerCount; layer++) {
		TileInfo* tile = GetTileAtPos(tilePos, layer);
		if (tile == NULL || tile->tile == NULL)
			continue;
		if (tile->tile->solid)
			available = false;
		if (tile->tile->overrideColl)
			available = !tile->tile->solid;
	}

	u32 nbOccupiedTiles = (u32)occupiedTiles.size();
	for (u32 i = 0; i < nbOccupiedTiles; i++)
		if (tilePos->x == occupiedTiles[i]->x && tilePos->y == occupiedTiles[i]->y)
			available = false;

	return available;
}

Warp* A_Map::IsTileWarp(const SDL_Point* tilePos) {
	u32 nbWarps = (u32)warps.size();
	for (u32 i = 0; i < nbWarps; i++)
	{
		Warp* warp = warps[i];
		if ((tilePos->x == warp->tilePos.x) && (tilePos->y == warp->tilePos.y))
			return warp;
	}

	return NULL;
}

void A_Map::BindTilePos(SDL_Point* tilePos) {
	occupiedTiles.push_back(tilePos);
}

void A_Map::UnbindTilePos(SDL_Point* tilePos) {
	for (std::vector<SDL_Point*>::iterator it = occupiedTiles.begin(); it != occupiedTiles.end(); ++it) {
		if (*it == tilePos) {
			occupiedTiles.erase(it);
			break;
		}
	}
}

void A_Map::TriggerEvent(int eventID) {
	if (eventID == 0)
		return;

	events |= ((u64)1 << (u64)eventID);
	SaveMgr::instance->AddValue("mapEvents", events);
}

void A_Map::UntriggerEvent(int eventID) {
	if (eventID == 0)
		return;

	events &= ~((u64)1 << (u64)eventID);
	SaveMgr::instance->AddValue("mapEvents", events);
}

bool A_Map::isEventTriggered(int eventID) {
	if (eventID == 0)
		return true;

	return (events & ((u64)1 << (u64)eventID));
}

bool A_Map::InteragirAvecTile(const SDL_Point* tilePos) {
	bool ret = false;

	for (u32 i = 0; i < (u32)actorsInt.size(); i++) {
		if (actorsInt[i]->tilePos.x == tilePos->x && actorsInt[i]->tilePos.y == tilePos->y) {
			actorsInt[i]->Interaction();
			ret = true;
		}
	}

	return ret;
}


bool CollOverride::IsActive() {
	if (this->eventIDEnable == 0 && this->eventIDDisable == 0)
		return true;

	bool isEnabled = A_Map::instance->isEventTriggered(this->eventIDEnable);
	bool isDisabled = A_Map::instance->isEventTriggered(this->eventIDDisable);

	if (this->eventIDEnable == 0)
		return !isDisabled;

	if (this->eventIDDisable == 0)
		return isEnabled;

	return isEnabled && !isDisabled;
}