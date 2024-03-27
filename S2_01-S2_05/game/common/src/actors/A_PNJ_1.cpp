#include "A_PNJ_1.h"
#include "A_Joueur.h"
#include "A_Map.h"
#include "A_Camera.h"
#include "A_TextBox.h"

Actor* A_PNJ_1::make() {
	return new A_PNJ_1();
}

int A_PNJ_1::onCreate() {
	int triggeringEventID = this->settings & 0xFFFF;
	if (!A_Map::instance->isEventTriggered(triggeringEventID))
		return 2;

	if(triggeringEventID == 0 && (A_Map::instance->isEventTriggered(EVENT_LOULOU_LIE) || A_Map::instance->isEventTriggered(EVENT_LOULOU_TRUTH)))
		return 2;

	this->drawOrder = 0;

	A_Map::instance->actorsInt.push_back(this);

	texture = IMG_LoadTexture(Game::instance->renderer, "assets/Characters/pnj1.png");
	SDL_QueryTexture(texture, NULL, NULL, &pTW, &pTH);
	pTW /= 3;
	pTH /= 4;

	int ww, wh;
	SDL_GetWindowSize(Game::instance->window, &ww, &wh);

	this->pos.x = (float)(this->tilePos.x * 32);
	this->pos.y = (float)(this->tilePos.y * 32);
	A_Map::instance->BindTilePos(&this->tilePos);

	this->direction = (this->settings >> 24) & 0xFF;

	if (triggeringEventID == 0) {
		PNJConvo* convoPrincipale = new PNJConvo;
		convoPrincipale->text = TextMgr::instance->GetValue("dlg.pnj1.1");
		convoPrincipale->title = TextMgr::instance->GetValue("dlg.pnj1.title");
		convoPrincipale->color = { 0xFF, 0xD0, 0x00, 0xFF };
		convoPrincipale->soundID = 4;
		convoPrincipale->nbChoices = 0;

		PNJConvo* convoSuite = new PNJConvo;
		convoSuite->text = TextMgr::instance->GetValue("dlg.pnj1.2");
		convoSuite->title = convoPrincipale->title;
		convoSuite->color = convoPrincipale->color;
		convoSuite->soundID = 4;
		convoSuite->nbChoices = 4;

		convoSuite->choices = new const char* [convoSuite->nbChoices];
		convoSuite->choicesResult = new PNJConvo * [convoSuite->nbChoices];

		convoSuite->choices[0] = TextMgr::instance->GetValue("dlg.pnj1.repN");
		convoSuite->choices[1] = TextMgr::instance->GetValue("dlg.pnj1.repS");
		convoSuite->choices[2] = TextMgr::instance->GetValue("dlg.pnj1.repW");
		convoSuite->choices[3] = TextMgr::instance->GetValue("dlg.pnj1.repE");

		PNJConvo* convoFinLie = new PNJConvo;
		convoFinLie->text = TextMgr::instance->GetValue("dlg.pnj1.3");
		convoFinLie->title = convoPrincipale->title;
		convoFinLie->color = convoPrincipale->color;
		convoFinLie->soundID = 4;
		convoFinLie->nbChoices = 0;
		convoFinLie->numTriggerEvent = EVENT_LOULOU_LIE;

		PNJConvo* convoFinTruth = new PNJConvo;
		convoFinTruth->text = convoFinLie->text;
		convoFinTruth->title = convoFinLie->title;
		convoFinTruth->color = convoFinLie->color;
		convoFinTruth->soundID = convoFinLie->soundID;
		convoFinTruth->nbChoices = convoFinLie->nbChoices;
		convoFinTruth->numTriggerEvent = EVENT_LOULOU_TRUTH;

		PNJConvo* convoFin = new PNJConvo;
		convoFin->text = convoFinLie->text;
		convoFin->title = convoFinLie->title;
		convoFin->color = convoFinLie->color;
		convoFin->soundID = convoFinLie->soundID;
		convoFin->nbChoices = convoFinLie->nbChoices;

		convoPrincipale->followingConvo = convoSuite;
		convoSuite->choicesResult[0] = convoFinLie;
		convoSuite->choicesResult[1] = convoFinTruth;
		convoSuite->choicesResult[2] = convoFinLie;
		convoSuite->choicesResult[3] = convoFinLie;

		convos.push_back(convoPrincipale);
		convos.push_back(convoFin);
		convos.push_back(convoSuite);
		convos.push_back(convoFinLie);
		convos.push_back(convoFinTruth);
	}
	else {
		PNJConvo* convo = new PNJConvo;
		convo->text = TextMgr::instance->GetValue((triggeringEventID == EVENT_LOULOU_LIE) ? "dlg.pnj1.4" : "dlg.pnj1.5");
		convo->title = TextMgr::instance->GetValue("dlg.pnj1.title");
		convo->color = { 0xFF, 0xD0, 0x00, 0xFF };
		convo->soundID = 4;
		convos.push_back(convo);
	}

	return true;
}

int A_PNJ_1::onDelete() {
	for (PNJConvo* convo : convos)
		delete convo;

	SDL_DestroyTexture(texture);

	return 1;
}

int A_PNJ_1::onExecute() {
	return 1;
}

int A_PNJ_1::onDraw() {
	SDL_Rect rect = { (int)(this->pos.x - (32 / 2)), (int)(this->pos.y - (32 / 2) - 8), 32, 32 };

	SDL_Rect srcrect = { 0, direction * pTH, pTW, pTH };

	Game::RenderCopy(texture, &srcrect, &rect);

	return 1;
}





void A_PNJ_1::Interaction() {
	bool isZero = ((this->settings & 0xFFFF) == 0);
	bool hasAnswered = (A_Map::instance->isEventTriggered(EVENT_LOULOU_LIE) || A_Map::instance->isEventTriggered(EVENT_LOULOU_TRUTH));

	A_TextBox::instance->affichertext(convos[(isZero && hasAnswered) ? 1 : 0]);
	this->direction = (A_Joueur::instance->direction + 2) % 4;
}