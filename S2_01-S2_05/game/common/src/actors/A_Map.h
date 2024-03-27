#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <common.h>

#include <core/Game.h>
#include <core/Actors.h>

#include "A_Inter.h"

struct Tileset;
struct Tile;
struct TileInfo;
struct SpecialTile;
struct Warp;
class CollOverride;
struct Entrance;

class A_Map : public Actor {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	ActorsManager actorMgr;

	Tileset* tilesets = NULL;
	SDL_Point originPoint = {0, 0};
	SDL_Rect bounds = { -1, -1, -1, -1 };
	
	bool mapLoaded = false;
	int loadTime = 60;
	int loadFade = -1;
	std::string currentFilename = "";
	int currentEntranceID = -1;

	int w = 256;
	int h = 256;
	int tilesize = 32;
	u32 layerCount = 0;
	u32* tileCounts = NULL;
	TileInfo** tilemaps = NULL;

	std::vector<SDL_Point*> occupiedTiles;
	std::vector<A_Inter*> actorsInt;

	std::vector<Warp*> warps;
	std::vector<CollOverride*> collOverrides;
	std::vector<Entrance*> entrances;

	u64 events = 0;

	bool Load(std::string filename, int entranceID);
	void DeleteElems();

	TileInfo* GetTileAtPos(const SDL_Point* tilePos, int layer);

	bool IsTileAvailable(const SDL_Point* tilePos);
	Warp* IsTileWarp(const SDL_Point* tilePos);
	void BindTilePos(SDL_Point* tilePos);
	void UnbindTilePos(SDL_Point* tilePos);

	void TriggerEvent(int eventID);
	void UntriggerEvent(int eventID);
	bool isEventTriggered(int eventID);

	bool InteragirAvecTile(const SDL_Point* tilePos);


	static Actor* make();

	static A_Map* instance;

private:
	void Render(bool isOver);
};

/* Map data */

struct Tile {
	u16 x = 0;
	u16 y = 0;
	u16 w = 32;
	u16 h = 32;

	bool solid = false;
	bool overPlayer = false;
	bool overrideColl = false;
};

struct Tileset {
	SDL_Texture *texture = NULL;

	Tile* tiles = NULL;
};

struct TileInfo {
	Tileset* tileset = NULL;
	Tile* tile = NULL;
};

/* Special tiles */

struct SpecialTile {
	SDL_Point tilePos = {-1, -1};
};

struct Warp : public SpecialTile {
	std::string filename = "";
	int entranceID = -1;
};

class CollOverride : public SpecialTile {
public:
	int eventIDEnable = 0;
	int eventIDDisable = 0;
	bool solid = false;

	bool IsActive();
};

struct Entrance : public SpecialTile {
	int id = -1;
	int direction = -1;
};

#define SPETILE_WARP 0
#define SPETILE_COLLOVERRIDE 1
#define SPETILE_ENTRANCE 2
#define SPETILE_PICKUPITEM 3
#define SPETILE_PNJ 4
#define SPETILE_CACTUS 5
#define SPETILE_CHEST 6

#define EVENT_RIRI 1
#define EVENT_MAX_1 2
#define EVENT_MAX_2 3
#define EVENT_LOULOU_TRUTH 4
#define EVENT_LOULOU_LIE 5