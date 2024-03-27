#include "A_Joueur.h"
#include "A_Map.h"
#include "A_Camera.h"
#include "A_PNJ_Alex.h"

CREATE_STATE(A_Joueur, Walk);
CREATE_STATE(A_Joueur, Wait);

A_Joueur* A_Joueur::instance = 0;

Actor* A_Joueur::make() {
	instance = new A_Joueur();
	return instance;
}

int A_Joueur::onCreate() {
	this->drawOrder = 1;

	texture = IMG_LoadTexture(Game::instance->renderer, "assets/Characters/player.png");
	SDL_QueryTexture(texture, NULL, NULL, &pTW, &pTH);
	pTW /= 3;
	pTH /= 4;

	int ww, wh;
	SDL_GetWindowSize(Game::instance->window, &ww, &wh);

	this->pos.x = (float)(this->tilePos.x * 32);
	this->pos.y = (float)(this->tilePos.y * 32);
	A_Map::instance->BindTilePos(&this->tilePos);

	this->direction = 1;
	this->count = 0;
	this->perso_pas = 0;

	if (!SaveMgr::instance->KeyExists("playerHP"))
		this->pv = PLAYER_MAX_HP;
	else {
		this->pv = *SaveMgr::instance->GetValue<int>("playerHP");
		if (this->pv <= 0)
			this->pv = PLAYER_MAX_HP;
	}

	this->hitSound = Mix_LoadWAV("assets/Audio/Hit.wav");
	this->healSound = Mix_LoadWAV("assets/Audio/Heal.wav");
	this->deathSound = Mix_LoadWAV("assets/Audio/Death.wav");

	if (this->settings != PLAYER_SETTINGS_TITLESCREEN) {
		this->inventory = A_InvMgr::instance->CreateInventory(TextMgr::instance->GetValue("inventory.player"), 9, 3, "playerInventory");

		if(!SaveMgr::instance->KeyExists("playerInventory")) {
			this->AddInventoryItem(0);
			this->AddInventoryItem(1);
		}
	}

	PNJConvo* convoNIS = new PNJConvo;
	convoNIS->text = TextMgr::instance->GetValue("dlg.flo.noinventoryspace");
	convoNIS->fontStyle = TTF_STYLE_ITALIC;
	convoNIS->soundID = 1;
	convoNIS->nbChoices = 0;

	convos.clear();
	convos.push_back(convoNIS);

	if (this->settings == PLAYER_SETTINGS_FIRSTTIME) {
		PNJConvo* convoFT = new PNJConvo;
		convoFT->text = TextMgr::instance->GetValue("dlg.flo.0");
		convoFT->title = TextMgr::instance->GetValue("dlg.flo.title");
		convoFT->color = { 0x00, 0x85, 0xFF, 0xFF };
		convoFT->soundID = 10;
		convoFT->nbChoices = 0;

		PNJConvo* convoFT1 = new PNJConvo;
		convoFT1->text = TextMgr::instance->GetValue("dlg.flo.1");
		convoFT1->title = convoFT->title;
		convoFT1->color = convoFT->color;
		convoFT1->soundID = 10;
		convoFT1->nbChoices = 0;
		convoFT->followingConvo = convoFT1;

		PNJConvo* convoFT2 = new PNJConvo;
		convoFT2->text = TextMgr::instance->GetValue("dlg.flo.2");
		convoFT2->title = convoFT->title;
		convoFT2->color = convoFT->color;
		convoFT2->soundID = 10;
		convoFT2->nbChoices = 0;
		convoFT1->followingConvo = convoFT2;

		PNJConvo* convoFT3 = new PNJConvo;
		convoFT3->text = TextMgr::instance->GetValue("dlg.tutorial.0");
		convoFT3->fontStyle = TTF_STYLE_ITALIC;
		convoFT3->soundID = 1;
		convoFT3->nbChoices = 0;
		convoFT2->followingConvo = convoFT3;

		PNJConvo* convoFT4 = new PNJConvo;
		convoFT4->text = TextMgr::instance->GetValue("dlg.tutorial.1");
		convoFT4->fontStyle = TTF_STYLE_ITALIC;
		convoFT4->soundID = 1;
		convoFT4->nbChoices = 0;
		convoFT3->followingConvo = convoFT4;

		PNJConvo* convoFT5 = new PNJConvo;
		convoFT5->text = TextMgr::instance->GetValue("dlg.tutorial.2");
		convoFT5->fontStyle = TTF_STYLE_ITALIC;
		convoFT5->soundID = 1;
		convoFT5->nbChoices = 0;
		convoFT4->followingConvo = convoFT5;

		#if !(defined(__ANDROID__) || defined(SIMULATE_ANDROID))
		PNJConvo* convoFT6 = new PNJConvo;
		convoFT6->text = TextMgr::instance->GetValue("dlg.tutorial.3");
		convoFT6->fontStyle = TTF_STYLE_ITALIC;
		convoFT6->soundID = 1;
		convoFT6->nbChoices = 0;
		convoFT5->followingConvo = convoFT6;
		#endif

		A_TextBox::instance->affichertext(convoFT);
		this->isInterracting = true;

		convos.push_back(convoFT);
		convos.push_back(convoFT1);
		convos.push_back(convoFT2);
		convos.push_back(convoFT3);
		convos.push_back(convoFT4);
		convos.push_back(convoFT5);

		#if !(defined(__ANDROID__) || defined(SIMULATE_ANDROID))
		convos.push_back(convoFT6);
		#endif
	}

	acState.setState(&StateID_Wait);

	return 1;
}

int A_Joueur::onDelete() {
	if(convos.size() > 0)
		for (PNJConvo* convo : convos)
			delete convo;

	SDL_DestroyTexture(texture);

	if (this->hitSound)
		Mix_FreeChunk(this->hitSound);

	if (this->healSound)
		Mix_FreeChunk(this->healSound);

	if (this->deathSound)
		Mix_FreeChunk(this->deathSound);

	instance = 0;

	return 1;
}

int A_Joueur::onExecute() {
	acState.execute();

	return 1;
}

int A_Joueur::onDraw() {

	SDL_Rect srcrect = { perso_pas * pTW, direction * pTH, pTW, pTH };

	SDL_FRect dstrect = { this->pos.x - (32 / 2), this->pos.y - 24, 32, 32 };

	Game::RenderCopyF(texture, &srcrect, &dstrect);

	return 1;
}


void A_Joueur::ToggleInventory(bool force) {
	if (A_PNJ_Alex::instance && !force) {
		this->isInterracting = true;
		A_PNJ_Alex::instance->Portable();
	}
	else {
		A_InvMgr::instance->ToggleInventory(inventory);
		Game::instance->pausable = !Game::instance->pausable;
		this->invOpened = !this->invOpened;
	}
}

InventoryItem* A_Joueur::GetInventoryItem(int x, int y) {
	return this->inventory->GetItem(x, y);
}

InventoryItem* A_Joueur::SetInventoryItem(int x, int y, int itemID, bool noSave) {
	InventoryItem* item = this->inventory->SetItem(x, y, itemID, noSave);

	return item;
}

InventoryItem* A_Joueur::AddInventoryItem(int itemID, bool noSave) {
	InventoryItem* added = this->inventory->AddItem(itemID, noSave);
	if (!added) {
		A_TextBox::instance->affichertext(convos[0]);
		this->isInterracting = true;
	}

	return added;
}


void A_Joueur::PointVie(int life) {
	this->pv += life;

	if (this->pv <= 0) {
		this->pv = 0;
	}
	else if (this->pv >= 100) {
		this->pv = 100;
	}

	SaveMgr::instance->AddValue("playerHP", this->pv);

	if(this->pv == 0)
		Mix_PlayChannel(SOUNDCHANNEL_PLAYER, this->deathSound, 0);
	else if (life < 0)
		Mix_PlayChannel(SOUNDCHANNEL_PLAYER, this->hitSound, 0);
	else if (life > 0)
		Mix_PlayChannel(SOUNDCHANNEL_PLAYER, this->healSound, 0);
}



/// <summary>
///  Etat Avancer
/// </summary>

void A_Joueur::beginState_Walk() {
	this->pos = { (float)(this->tilePos.x * 32), (float)(this->tilePos.y * 32) };

	if (!A_Map::instance->IsTileAvailable(&this->targetPos)) {
		acState.setState(&StateID_Wait);
		return;
	}

	this->count = 0;
	this->perso_pas = 1;

	float targetLength = distance(this->tilePos.x, this->tilePos.y, this->targetPos.x, this->targetPos.y);
	this->targetVector = { (this->targetPos.x - this->tilePos.x) / targetLength, (this->targetPos.y - this->tilePos.y) / targetLength };
	
	this->tilePos = this->targetPos;
}

void A_Joueur::executeState_Walk() {
	this->pos = { this->pos.x + (this->targetVector.x * (32.0f / 13.0f)), this->pos.y + (this->targetVector.y * (32.0f / 13.0f)) };

	if (this->count == 4) {
		this->perso_pas = 0;
	}
	else if (this->count == 8) {
		this->perso_pas = 2;
	}
	else if (this->count == 12) {
		acState.setState(&StateID_Wait);
	}

	this->count++;

	// Sprint
	KeyHandle* khShift = Game::instance->inputHandler.getKeyHandle(SDLK_LSHIFT);
	if ((khShift->held) && ((this->count % 2) == 0)) {
		this->executeState_Walk();
	}
}

void A_Joueur::endState_Walk() {
	this->pos = { (float)(this->tilePos.x * 32), (float)(this->tilePos.y * 32) };

	Warp* potentialWarp = A_Map::instance->IsTileWarp(&this->tilePos);
	if (potentialWarp) {
		A_Map::instance->Load(potentialWarp->filename, potentialWarp->entranceID);
	}

	perso_pas = 0;
}




/// <summary>
/// Etat Attente
/// </summary>

void A_Joueur::beginState_Wait() {
}

void A_Joueur::executeState_Wait() {
	bool inter = Game::instance->inputHandler.getPressedInteraction();
	KeyHandle* ikey = Game::instance->inputHandler.getKeyHandle(SDLK_i);
	KeyHandle* esckey = Game::instance->inputHandler.getKeyHandle(SDLK_ESCAPE);
	int nDir = Game::instance->inputHandler.getHeldDirection();

	if (!this->isInterracting) {
		if (nDir != DIRECTION_NONE && !this->invOpened) {
			direction = nDir;

			switch (direction) {
			case DIRECTION_RIGHT:
				this->targetPos = { this->tilePos.x + 1, this->tilePos.y };
				break;
			case DIRECTION_DOWN:
				this->targetPos = { this->tilePos.x, this->tilePos.y + 1 };
				break;
			case DIRECTION_LEFT:
				this->targetPos = { this->tilePos.x - 1, this->tilePos.y };
				break;
			case DIRECTION_UP:
				this->targetPos = { this->tilePos.x, this->tilePos.y - 1 };
				break;
			}

			acState.setState(&StateID_Walk);
		}
		
		if (ikey->pressed || (this->invOpened && esckey->pressed)) {
			ToggleInventory();
		}

		if (inter && !this->invOpened) {
			SDL_Point ptRegarde = this->tilePos;

			if (direction == 0)
				ptRegarde.x++;

			else if (direction == 1)
				ptRegarde.y++;

			else if (direction == 2)
				ptRegarde.x--;

			else if (direction == 3)
				ptRegarde.y--;

			this->isInterracting = true;


			if (!A_Map::instance->InteragirAvecTile(&ptRegarde)) {
				this->isInterracting = false;
			}
		}
	}
	
}

void A_Joueur::endState_Wait() {
}