#include "A_PNJ_Alex.h"
#include "A_Joueur.h"
#include "A_Map.h"
#include "A_Camera.h"
#include "A_TextBox.h"

A_PNJ_Alex* A_PNJ_Alex::instance = NULL;

Actor* A_PNJ_Alex::make() {
	instance = new A_PNJ_Alex();
	return instance;
}

int A_PNJ_Alex::onCreate() {
	if (!A_Map::instance->isEventTriggered(this->settings & 0xFFFF))
		return 2;

	this->drawOrder = 0;

	A_Map::instance->actorsInt.push_back(this);

	texture = IMG_LoadTexture(Game::instance->renderer, "assets/Characters/alex.png");
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
	convoPrincipale->text = TextMgr::instance->GetValue("dlg.alex.a0");
	convoPrincipale->title = TextMgr::instance->GetValue("dlg.alex.title");
	convoPrincipale->color = { 0x00, 0xA8, 0x10, 0xFF };
	convoPrincipale->soundID = 14;
	convoPrincipale->nbChoices = 2;

	convoPrincipale->choices = new const char* [convoPrincipale->nbChoices];
	convoPrincipale->choicesResult = new PNJConvo * [convoPrincipale->nbChoices];
	convoPrincipale->choices[0] = TextMgr::instance->GetValue("dlg.yes");
	convoPrincipale->choices[1] = TextMgr::instance->GetValue("dlg.no");


	PNJConvo* convoRep1 = new PNJConvo;
	convoRep1->text = TextMgr::instance->GetValue("dlg.alex.rep1");
	convoRep1->title = convoPrincipale->title;
	convoRep1->color = convoPrincipale->color;
	convoRep1->soundID = 14;
	convoRep1->nbChoices = 0;

	PNJConvo* convoRep2 = new PNJConvo;
	convoRep2->text = TextMgr::instance->GetValue("dlg.alex.rep2");
	convoRep2->title = convoPrincipale->title;
	convoRep2->color = convoPrincipale->color;
	convoRep2->soundID = 14;
	convoRep2->nbChoices = 0;

	PNJConvo* convPort = new PNJConvo;
	convPort->text = TextMgr::instance->GetValue("dlg.alex.port");
	convPort->title = convoPrincipale->title;
	convPort->color = convoPrincipale->color;
	convPort->soundID = 14;
	convPort->nbChoices = 0;

	convoPrincipale->choicesResult[0] = convoRep1;
	convoPrincipale->choicesResult[1] = convoRep2;

	convos.push_back(convoPrincipale);
	convos.push_back(convPort);
	convos.push_back(convoRep1);
	convos.push_back(convoRep2);


	return true;
}

int A_PNJ_Alex::onDelete() {
	for (PNJConvo* convo : convos)
		delete convo;

	SDL_DestroyTexture(texture);

	instance = NULL;

	return 1;
}

int A_PNJ_Alex::onExecute() {
	return 1;
}

int A_PNJ_Alex::onDraw() {
	SDL_Rect rect = { (int)(this->pos.x - (32 / 2)), (int)(this->pos.y - (32 / 2) - 8), 32, 32 };

	SDL_Rect srcrect = { 0, direction * pTH, pTW, pTH };

	Game::RenderCopy(texture, &srcrect, &rect);

	return 1;
}





void A_PNJ_Alex::Interaction() {
	A_TextBox::instance->affichertext(convos[0]);
	this->direction = (A_Joueur::instance->direction + 2) % 4;
}

void A_PNJ_Alex::Portable() {
	A_TextBox::instance->affichertext(convos[1]);
}