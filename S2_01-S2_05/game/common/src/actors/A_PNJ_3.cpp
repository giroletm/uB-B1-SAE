#include "A_PNJ_3.h"
#include "A_Joueur.h"
#include "A_Map.h"
#include "A_TextBox.h"

Actor* A_PNJ_3::make() {
	return new A_PNJ_3();
}

bool GiveDSToPlayer() {
	A_Joueur::instance->AddInventoryItem(2);
	return true;
}

int A_PNJ_3::onCreate() {
	if (!A_Map::instance->isEventTriggered(this->settings & 0xFFFF))
		return 2;

	this->drawOrder = 0;

	A_Map::instance->actorsInt.push_back(this);

	texture = IMG_LoadTexture(Game::instance->renderer, "assets/Characters/pnj3.png");
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
	convoPrincipale->text = TextMgr::instance->GetValue("dlg.pnj3.0");
	convoPrincipale->title = TextMgr::instance->GetValue("dlg.pnj3.title");
	convoPrincipale->color = { 0xFF, 0xD0, 0x00, 0xFF };
	convoPrincipale->soundID = 6;
	convoPrincipale->nbChoices = 2;
	convoPrincipale->choices = new const char* [convoPrincipale->nbChoices];
	convoPrincipale->choicesResult = new PNJConvo * [convoPrincipale->nbChoices];

	convoPrincipale->choices[0] = TextMgr::instance->GetValue("dlg.pnj3.repDS");
	convoPrincipale->choices[1] = TextMgr::instance->GetValue("dlg.pnj3.repN");

	PNJConvo* convoOui = new PNJConvo;
	convoOui->text = TextMgr::instance->GetValue("dlg.pnj3.1");
	convoOui->title = convoPrincipale->title;
	convoOui->color = { 0xFF, 0xD0, 0x00, 0xFF };
	convoOui->soundID = 6;
	convoOui->nbChoices = 0;

	PNJConvo* convoOui2 = new PNJConvo;
	convoOui2->text = TextMgr::instance->GetValue("dlg.pnj3.2");
	convoOui2->soundID = 1;
	convoOui2->fontStyle = TTF_STYLE_ITALIC;
	convoOui2->nbChoices = 0;
	convoOui->followingConvo = convoOui2;

	PNJConvo* convoOui3 = new PNJConvo;
	convoOui3->text = TextMgr::instance->GetValue("dlg.pnj3.3");
	convoOui3->title = convoPrincipale->title;
	convoOui3->color = { 0xFF, 0xD0, 0x00, 0xFF };
	convoOui3->soundID = 6;
	convoOui3->callback = &GiveDSToPlayer;
	convoOui3->nbChoices = 0;
	convoOui3->numTriggerEvent = EVENT_RIRI;
	convoOui2->followingConvo = convoOui3;

	PNJConvo* convoAfter = new PNJConvo;
	convoAfter->text = convoOui3->text;
	convoAfter->title = convoOui3->title;
	convoAfter->color = convoOui3->color;
	convoAfter->soundID = convoOui3->soundID;
	convoAfter->nbChoices = 0;

	convoPrincipale->choicesResult[0] = convoOui;
	convoPrincipale->choicesResult[1] = NULL;

	convos.push_back(convoAfter);
	convos.push_back(convoPrincipale);
	convos.push_back(convoOui);
	convos.push_back(convoOui2);
	convos.push_back(convoOui3);

	return true;
}

int A_PNJ_3::onDelete() {
	for (PNJConvo* convo : convos)
		delete convo;

	SDL_DestroyTexture(texture);

	return 1;
}

int A_PNJ_3::onExecute() {
	return 1;
}

int A_PNJ_3::onDraw() {
	SDL_Rect rect = { (int)(this->pos.x - (32 / 2)), (int)(this->pos.y - (32 / 2) - 8), 32, 32 };

	SDL_Rect srcrect = { 0, direction * pTH, pTW, pTH };

	Game::RenderCopy(texture, &srcrect, &rect);

	return 1;
}


void A_PNJ_3::Interaction() {
	A_TextBox::instance->affichertext(convos[A_Map::instance->isEventTriggered(EVENT_RIRI) ? 0 : 1]);
	this->direction = (A_Joueur::instance->direction + 2) % 4;
}