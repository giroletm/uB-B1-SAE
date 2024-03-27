#include "A_PickupObj.h"
#include "A_Joueur.h"
#include "A_Map.h"
#include "A_Camera.h"
#include "A_TextBox.h"

A_PickupObj* A_PickupObj::latestGrabbed = NULL;

Actor* A_PickupObj::make() {
	return new A_PickupObj();
}

int A_PickupObj::onCreate() {
	this->uniqueID = this->settings & 0xFFFF;
	this->type = (this->settings >> 16) & 0xFF;
	this->triggeringEventID = (this->settings >> 24) & 0xFF;

	this->available = true;

	char* saveKey;
	SDL_asprintf(&saveKey, "collectedObj%d", this->uniqueID);

	if (SaveMgr::instance->KeyExists(saveKey) && *SaveMgr::instance->GetValue<bool>(saveKey))
		return 2;

	SDL_free(saveKey);

	if (!A_Map::instance->isEventTriggered(this->triggeringEventID))
		return 2;

	this->drawOrder = 0;

	A_Map::instance->actorsInt.push_back(this);

	std::string texName;
	switch (this->type) {
		default:
		case 0:
			texName = "pq.png";
			break;
		case 1:
			texName = "brokenwine.png";
			break;
		case 2:
			texName = "DS.png";
			break;
	}

	texture = IMG_LoadTexture(Game::instance->renderer, ("assets/Items/" + texName).c_str());

	grabbedSfx = Mix_LoadWAV("assets/Audio/ItemGrabbed.wav");

	this->pos.x = (float)(this->tilePos.x * 32);
	this->pos.y = (float)(this->tilePos.y * 32);
	A_Map::instance->BindTilePos(&this->tilePos);

	return true;
}

int A_PickupObj::onDelete() {
	if (texture)
		SDL_DestroyTexture(texture);

	if (grabbedSfx)
		Mix_FreeChunk(grabbedSfx);

	A_Map::instance->UnbindTilePos(&this->tilePos);
	for (auto it = A_Map::instance->actorsInt.begin(); it != A_Map::instance->actorsInt.end(); ++it) {
		if (*it == this) {
			A_Map::instance->actorsInt.erase(it);
			break;
		}
	}

	return 1;
}

int A_PickupObj::onExecute() {

	return 1;
}

int A_PickupObj::onDraw() {
	if (this->available) {
		SDL_Rect rect = { (int)(this->pos.x - (32 / 2)), (int)(this->pos.y - (32 / 2)), 32, 32 };
		Game::RenderCopy(texture, NULL, &rect);
	}

	return 1;
}


void A_PickupObj::Interaction() {
	A_Joueur::instance->isInterracting = false;

	if (!this->available)
		return;

	if (A_Joueur::instance->AddInventoryItem(this->type)) {
		char* saveKey;
		SDL_asprintf(&saveKey, "collectedObj%d", this->uniqueID);
		SaveMgr::instance->AddValue(saveKey, true);
		SDL_free(saveKey);

		latestGrabbed = this;
		this->available = false;

		Mix_PlayChannel(SOUNDCHANNEL_PICKUPITEM, grabbedSfx, 0);
	}
}

void A_PickupObj::ChannelFinished() {
	this->Delete();
}