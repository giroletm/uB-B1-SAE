#include "A_Cactus.h"
#include "A_Joueur.h"
#include "A_Map.h"
#include "A_Camera.h"
#include "A_TextBox.h"

CREATE_STATE(A_Cactus, Hit);
CREATE_STATE(A_Cactus, Wait);

Actor* A_Cactus::make() {
	return new A_Cactus();
}

int A_Cactus::onCreate() {
	this->drawOrder = 0;

	texture = IMG_LoadTexture(Game::instance->renderer, "assets/Misc/Cactus.png");
	SDL_QueryTexture(texture, NULL, NULL, &pTW, &pTH);
	pTW /= 3;
	pTH /= 4;

	int ww, wh;
	SDL_GetWindowSize(Game::instance->window, &ww, &wh);

	this->pos.x = (float)(this->tilePos.x * 32);
	this->pos.y = (float)(this->tilePos.y * 32);

	A_Map::instance->BindTilePos(&this->tilePos);

	acState.setState(&StateID_Wait);

	return true;
}

int A_Cactus::onDelete() {
	SDL_DestroyTexture(texture);

	return 1;
}

int A_Cactus::onExecute() {
	this->direct = A_Joueur::instance->direction;
	Vec2 posJ = {
		A_Joueur::instance->pos.x / 32,
		A_Joueur::instance->pos.y / 32,
	};

	if ((posJ.x == tilePos.x - 1 && posJ.y == tilePos.y && direct == 0)
		|| (posJ.x == tilePos.x + 1 && posJ.y == tilePos.y && direct == 2)
		|| (posJ.x == tilePos.x && posJ.y == tilePos.y - 1 && direct == 1)
		|| (posJ.x == tilePos.x && posJ.y == tilePos.y + 1 && direct == 3))
		regarde = 1;
	else
		regarde = 0;

	acState.execute();

	return 1;
}

int A_Cactus::onDraw() {
	SDL_Rect rect = { (int)(this->pos.x - (32 / 2)), (int)(this->pos.y - (32 / 2) - 8), 32, 32 };

	Game::RenderCopy(texture, NULL, &rect);

	return 1;
}


/// <summary>
/// Etat Parler
/// </summary>

void A_Cactus::beginState_Hit() {
	A_Joueur::instance->PointVie(-10);
}

void A_Cactus::executeState_Hit() {
	if (regarde == 0)
		acState.setState(&StateID_Wait);
}

void A_Cactus::endState_Hit() {
}


/// <summary>
/// Etat Attente
/// </summary>

void A_Cactus::beginState_Wait() {
}

void A_Cactus::executeState_Wait() {
	if (regarde == 1)
		acState.setState(&StateID_Hit);
}

void A_Cactus::endState_Wait() {
}
