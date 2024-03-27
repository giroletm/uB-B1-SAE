#include "A_PNJ_2.h"
#include "A_Joueur.h"
#include "A_Map.h"
#include "A_Camera.h"
#include "A_TextBox.h"

Actor* A_PNJ_2::make() {
	return new A_PNJ_2();
}

int A_PNJ_2::onCreate() {
	if (!A_Map::instance->isEventTriggered(this->settings & 0xFFFF))
		return 2;

	this->drawOrder = 0;

	A_Map::instance->actorsInt.push_back(this);

	texture = IMG_LoadTexture(Game::instance->renderer, "assets/Characters/pnj2.png");
	SDL_QueryTexture(texture, NULL, NULL, &pTW, &pTH);
	pTW /= 3;
	pTH /= 4;

	int ww, wh;
	SDL_GetWindowSize(Game::instance->window, &ww, &wh);

	this->pos.x = (float)(this->tilePos.x * 32);
	this->pos.y = (float)(this->tilePos.y * 32);
	A_Map::instance->BindTilePos(&this->tilePos);

	this->direction = (this->settings >> 24) & 0xFF;


	PNJConvo* convoPrincipale = new PNJConvo;
	convoPrincipale->text = TextMgr::instance->GetValue("dlg.pnj2.0");
	convoPrincipale->title = TextMgr::instance->GetValue("dlg.pnj2.title");
	convoPrincipale->color = { 0xFF, 0xD0, 0x00, 0xFF };
	convoPrincipale->soundID = 7;
	convoPrincipale->nbChoices = 1;

	PNJConvo* convo1 = new PNJConvo;
	convo1->text = TextMgr::instance->GetValue("dlg.pnj2.1");
	convo1->title = convoPrincipale->title;
	convo1->color = convoPrincipale->color;
	convo1->soundID = 7;
	convo1->nbChoices = 0;

	convoPrincipale->choices = new const char* [convoPrincipale->nbChoices];
	convoPrincipale->choicesResult = new PNJConvo * [convoPrincipale->nbChoices];

	convoPrincipale->choices[0] = TextMgr::instance->GetValue("dlg.pnj2.rep");

	convoPrincipale->choicesResult[0] = convo1;

	convos.push_back(convoPrincipale);
	convos.push_back(convo1);

	return true;
}

int A_PNJ_2::onDelete() {
	for (PNJConvo* convo : convos)
		delete convo;

	SDL_DestroyTexture(texture);

	return 1;
}

int A_PNJ_2::onExecute() {
	return 1;
}

int A_PNJ_2::onDraw() {
	SDL_Rect rect = { (int)(this->pos.x - (32 / 2)), (int)(this->pos.y - (32 / 2) - 8), 32, 32 };

	SDL_Rect srcrect = { 0, direction * pTH, pTW, pTH };

	Game::RenderCopy(texture, &srcrect, &rect);

	return 1;
}





void A_PNJ_2::Interaction() {
	A_TextBox::instance->affichertext(convos[0]);
	this->direction = (A_Joueur::instance->direction + 2) % 4;
}