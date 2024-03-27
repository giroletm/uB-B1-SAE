#include "A_PNJ_Max.h"
#include "A_Joueur.h"
#include "A_Map.h"
#include "A_Camera.h"
#include "A_TextBox.h"

#if defined(__ANDROID__) || defined(SIMULATE_ANDROID)
#include "A_AndroidCtrl.h"
#endif

Actor* A_PNJ_Max::make() {
	return new A_PNJ_Max();
}

bool TheEnd() {
	A_Map::instance->Delete();
	
	#if defined(__ANDROID__) || defined(SIMULATE_ANDROID)
	A_AndroidCtrl::instance->Delete();
	#endif

	Game::instance->actorMgr.createActorByID(19); // Credits
	
	return false;
}

int A_PNJ_Max::onCreate() {
	if (!A_Map::instance->isEventTriggered(this->settings & 0xFFFF))
		return 2;

	this->drawOrder = 0;

	A_Map::instance->actorsInt.push_back(this);

	texture = IMG_LoadTexture(Game::instance->renderer, "assets/Characters/max.png");
	SDL_QueryTexture(texture, NULL, NULL, &pTW, &pTH);
	pTW /= 3;
	pTH /= 4;

	int ww, wh;
	SDL_GetWindowSize(Game::instance->window, &ww, &wh);

	this->pos.x = (float)(this->tilePos.x * 32);
	this->pos.y = (float)(this->tilePos.y * 32);
	A_Map::instance->BindTilePos(&this->tilePos);

	this->direction = (this->settings >> 24) & 0xFF;

	/******************************/
	/* Conversation première fois */
	/******************************/

	PNJConvo* convoP0T0 = new PNJConvo;
	convoP0T0->text = TextMgr::instance->GetValue("dlg.max.b0");
	convoP0T0->title = TextMgr::instance->GetValue("dlg.max.title");
	convoP0T0->color = { 0xFF, 0x00, 0x04, 0xFF };
	convoP0T0->soundID = 9;
	convoP0T0->nbChoices = 1;
	convoP0T0->numTriggerEvent = EVENT_MAX_1;

	convoP0T0->choices = new const char* [convoP0T0->nbChoices];
	convoP0T0->choices[0] = TextMgr::instance->GetValue("dlg.max.b0c");
	convoP0T0->choicesResult = new PNJConvo * [convoP0T0->nbChoices];

	PNJConvo* convoP0T1 = convoP0T0->choicesResult[0] = new PNJConvo;
	convoP0T1->text = TextMgr::instance->GetValue("dlg.max.b1");
	convoP0T1->title = convoP0T0->title;
	convoP0T1->color = convoP0T0->color;
	convoP0T1->soundID = 9;

	PNJConvo* convoP0T2 = convoP0T1->followingConvo = new PNJConvo;
	convoP0T2->text = TextMgr::instance->GetValue("dlg.max.b2");
	convoP0T2->title = convoP0T0->title;
	convoP0T2->color = convoP0T0->color;
	convoP0T2->soundID = 9;

	PNJConvo* convoP0T3 = convoP0T2->followingConvo = new PNJConvo;
	convoP0T3->text = TextMgr::instance->GetValue("dlg.max.b3");
	convoP0T3->title = convoP0T0->title;
	convoP0T3->color = convoP0T0->color;
	convoP0T3->soundID = 9;
	
	PNJConvo* convoP0T4 = convoP0T3->followingConvo = new PNJConvo;
	convoP0T4->text = TextMgr::instance->GetValue("dlg.max.b4");
	convoP0T4->title = convoP0T0->title;
	convoP0T4->color = convoP0T0->color;
	convoP0T4->soundID = 9;

	/***********************/
	/* Conversation de fin */
	/***********************/

	PNJConvo* convoP3T0 = new PNJConvo;
	convoP3T0->text = TextMgr::instance->GetValue("dlg.max.c0");
	convoP3T0->title = convoP0T0->title;
	convoP3T0->color = convoP0T0->color;
	convoP3T0->soundID = 9;

	PNJConvo* convoP3T1 = convoP3T0->followingConvo = new PNJConvo;
	convoP3T1->text = TextMgr::instance->GetValue("dlg.max.c1");
	convoP3T1->title = convoP0T0->title;
	convoP3T1->color = convoP0T0->color;
	convoP3T1->soundID = 9;

	PNJConvo* convoP3T2 = convoP3T1->followingConvo = new PNJConvo;
	convoP3T2->text = TextMgr::instance->GetValue("dlg.max.c2");
	convoP3T2->title = convoP0T0->title;
	convoP3T2->color = convoP0T0->color;
	convoP3T2->soundID = 9;

	PNJConvo* convoP3T3 = convoP3T2->followingConvo = new PNJConvo;
	convoP3T3->text = TextMgr::instance->GetValue("dlg.max.c3");
	convoP3T3->title = convoP0T0->title;
	convoP3T3->color = convoP0T0->color;
	convoP3T3->soundID = 9;
	convoP3T3->nbChoices = 1;

	convoP3T3->choices = new const char* [convoP0T0->nbChoices];
	convoP3T3->choices[0] = TextMgr::instance->GetValue("dlg.max.c3c");
	convoP3T3->choicesResult = new PNJConvo * [convoP3T3->nbChoices];

	PNJConvo* convoP3T4 = convoP3T3->choicesResult[0] = new PNJConvo;
	convoP3T4->text = TextMgr::instance->GetValue("dlg.max.c4");
	convoP3T4->title = convoP0T0->title;
	convoP3T4->color = convoP0T0->color;
	convoP3T4->soundID = 9;

	PNJConvo* convoP3T5 = convoP3T4->followingConvo = new PNJConvo;
	convoP3T5->text = TextMgr::instance->GetValue("dlg.max.c5");
	convoP3T5->title = convoP0T0->title;
	convoP3T5->color = convoP0T0->color;
	convoP3T5->soundID = 9;

	PNJConvo* convoP3T6 = convoP3T5->followingConvo = new PNJConvo;
	convoP3T6->text = TextMgr::instance->GetValue("dlg.max.c6");
	convoP3T6->title = convoP0T0->title;
	convoP3T6->color = convoP0T0->color;
	convoP3T6->callback = &TheEnd;
	convoP3T6->soundID = 9;

	/***********************/
	/* Conversations poème */
	/***********************/

	PNJConvo* convoPrincipale1 = new PNJConvo;
	convoPrincipale1->text = TextMgr::instance->GetValue("dlg.max.a0");
	convoPrincipale1->title = convoP0T0->title;
	convoPrincipale1->color = convoP0T0->color;
	convoPrincipale1->soundID = 9;
	convoPrincipale1->nbChoices = 2;

	PNJConvo* convoPrincipale2 = new PNJConvo;
	convoPrincipale2->text = TextMgr::instance->GetValue("dlg.max.a1");
	convoPrincipale2->title = convoP0T0->title;
	convoPrincipale2->color = convoP0T0->color;
	convoPrincipale2->soundID = 9;
	convoPrincipale2->nbChoices = 2;

	PNJConvo* convoOui = new PNJConvo;
	convoOui->text = TextMgr::instance->GetValue("dlg.max.o1.1");
	convoOui->title = convoP0T0->title;
	convoOui->color = convoP0T0->color;
	convoOui->soundID = 9;

	PNJConvo* convoOui2 = new PNJConvo;
	convoOui2->text = TextMgr::instance->GetValue("dlg.max.o1.2");
	convoOui2->title = convoP0T0->title;
	convoOui2->color = convoP0T0->color;
	convoOui2->soundID = 9;

	PNJConvo* convoOui21 = new PNJConvo;
	convoOui21->text = TextMgr::instance->GetValue("dlg.max.o2.1");
	convoOui21->title = convoP0T0->title;
	convoOui21->color = convoP0T0->color;
	convoOui21->soundID = 9;
	convoOui21->nbChoices = 0;

	PNJConvo* convoOui22 = new PNJConvo;
	convoOui22->text = TextMgr::instance->GetValue("dlg.max.o2.2");
	convoOui22->title = convoP0T0->title;
	convoOui22->color = convoP0T0->color;
	convoOui22->soundID = 9;
	convoOui22->nbChoices = 2;

	PNJConvo* convrep1 = new PNJConvo;
	convrep1->text = TextMgr::instance->GetValue("dlg.max.rep1");
	convrep1->title = convoP0T0->title;
	convrep1->color = convoP0T0->color;
	convrep1->soundID = 9;
	convrep1->nbChoices = 0;

	PNJConvo* convrep2 = new PNJConvo;
	convrep2->text = TextMgr::instance->GetValue("dlg.max.rep2");
	convrep2->title = convoP0T0->title;
	convrep2->color = convoP0T0->color;
	convrep2->soundID = 9;
	convrep2->nbChoices = 0;

	PNJConvo* convoOui3 = new PNJConvo;
	convoOui3->text = TextMgr::instance->GetValue("dlg.max.o3");
	convoOui3->title = convoP0T0->title;
	convoOui3->color = convoP0T0->color;
	convoOui3->soundID = 9;
	convoOui3->nbChoices = 0;

	PNJConvo* convoOui4 = new PNJConvo;
	convoOui4->text = TextMgr::instance->GetValue("dlg.max.o4");
	convoOui4->title = convoP0T0->title;
	convoOui4->color = convoP0T0->color;
	convoOui4->soundID = 9;
	convoOui4->nbChoices = 0;

	PNJConvo* convoOui5 = new PNJConvo;
	convoOui5->text = TextMgr::instance->GetValue("dlg.max.o5");
	convoOui5->title = convoP0T0->title;
	convoOui5->color = convoP0T0->color;
	convoOui5->soundID = 9;
	convoOui5->nbChoices = 0;

	PNJConvo* convoOui6 = new PNJConvo;
	convoOui6->text = TextMgr::instance->GetValue("dlg.max.o6");
	convoOui6->title = convoP0T0->title;
	convoOui6->color = convoP0T0->color;
	convoOui6->soundID = 9;
	convoOui6->nbChoices = 0;

	PNJConvo* convoOui7 = new PNJConvo;
	convoOui7->text = TextMgr::instance->GetValue("dlg.max.o7");
	convoOui7->title = convoP0T0->title;
	convoOui7->color = convoP0T0->color;
	convoOui7->soundID = 9;
	convoOui7->nbChoices = 0;

	PNJConvo* convoOui8 = new PNJConvo;
	convoOui8->text = TextMgr::instance->GetValue("dlg.max.o8");
	convoOui8->title = convoP0T0->title;
	convoOui8->color = convoP0T0->color;
	convoOui8->soundID = 9;
	convoOui8->nbChoices = 0;
	convoOui8->numTriggerEvent = EVENT_MAX_2;

	PNJConvo* convoNon = new PNJConvo;
	convoNon->text = TextMgr::instance->GetValue("dlg.max.n1");
	convoNon->title = convoP0T0->title;
	convoNon->color = convoP0T0->color;
	convoNon->soundID = 9;
	convoNon->nbChoices = 2;

	PNJConvo* convoNon1 = new PNJConvo;
	convoNon1->text = TextMgr::instance->GetValue("dlg.max.n2");
	convoNon1->title = convoP0T0->title;
	convoNon1->color = convoP0T0->color;
	convoNon1->soundID = 9;
	convoNon1->nbChoices = 0;


	convoPrincipale1->choices = new const char*[convoPrincipale1->nbChoices];
	convoPrincipale1->choicesResult = new PNJConvo*[convoPrincipale1->nbChoices];
	convoPrincipale1->choices[0] = TextMgr::instance->GetValue("dlg.yes");
	convoPrincipale1->choices[1] = TextMgr::instance->GetValue("dlg.no");

	convoNon->choices = new const char*[convoPrincipale1->nbChoices];
	convoNon->choicesResult = new PNJConvo*[convoPrincipale1->nbChoices];
	convoNon->choices[0] = convoPrincipale1->choices[0];
	convoNon->choices[1] = convoPrincipale1->choices[1]; 


	convoPrincipale2->choices = new const char*[convoPrincipale1->nbChoices];
	convoPrincipale2->choicesResult = new PNJConvo*[convoPrincipale1->nbChoices];
	convoPrincipale2->choices[0] = convoPrincipale1->choices[0];
	convoPrincipale2->choices[1] = convoPrincipale1->choices[1];

	convoOui22->choices = new const char* [convoPrincipale1->nbChoices];
	convoOui22->choicesResult = new PNJConvo * [convoPrincipale1->nbChoices];
	convoOui22->choices[0] = TextMgr::instance->GetValue("dlg.max.r1");
	convoOui22->choices[1] = TextMgr::instance->GetValue("dlg.max.r2");


	convoPrincipale1->choicesResult[0] = convoOui;
	convoPrincipale1->choicesResult[1] = convoNon;

	convoPrincipale2->choicesResult[0] = convoOui3;
	convoPrincipale2->choicesResult[1] = convoNon1;

	convoNon->choicesResult[0] = convoOui21;
	convoNon->choicesResult[1] = convoNon1;

	convoOui22->choicesResult[0] = convrep2;
	convoOui22->choicesResult[1] = convrep1;

	convoOui3->followingConvo = convoOui4;
	convoOui4->followingConvo = convoOui5;
	convoOui5->followingConvo = convoOui6;
	convoOui6->followingConvo = convoOui7;
	convoOui7->followingConvo = convoOui8;
	convoOui8->followingConvo = convoNon1;

	convoOui21->followingConvo = convoOui22;
	convrep2->followingConvo = convoOui3;
	convrep2->followingConvo = convoOui3;


	convos.push_back(convoP0T0);
	convos.push_back(convoPrincipale1);
	convos.push_back(convoPrincipale2);
	convos.push_back(convoP3T0);

	convos.push_back(convoP0T1);
	convos.push_back(convoP0T2);
	convos.push_back(convoP0T3);
	convos.push_back(convoP0T4);

	convos.push_back(convoP3T1);
	convos.push_back(convoP3T2);
	convos.push_back(convoP3T3);
	convos.push_back(convoP3T4);
	convos.push_back(convoP3T5);

	convos.push_back(convoOui);
	convos.push_back(convoOui2);
	convos.push_back(convoNon);
	convos.push_back(convoOui3);
	convos.push_back(convoOui4);
	convos.push_back(convoOui5);
	convos.push_back(convoOui6);
	convos.push_back(convoOui7);
	convos.push_back(convoNon1);

	return true;
}

int A_PNJ_Max::onDelete() {
	for (PNJConvo* convo : convos)
		delete convo;

	SDL_DestroyTexture(texture);

	return 1;
}

int A_PNJ_Max::onExecute() {
	return 1;
}

int A_PNJ_Max::onDraw() {
	SDL_Rect rect = { (int)(this->pos.x - (32 / 2)), (int)(this->pos.y - (32 / 2) - 8), 32, 32 };

	SDL_Rect srcrect = { 0, direction * pTH, pTW, pTH };

	Game::RenderCopy(texture, &srcrect, &rect);

	return 1;
}





void A_PNJ_Max::Interaction() {
	Inventory* playerInv = A_Joueur::instance->inventory;
	int nbDS = 0;

	for (int y = 0; y < playerInv->nbRows; y++) {
		for (int x = 0; x < playerInv->nbCols; x++) {
			InventoryItem* item = playerInv->GetItem(x, y);
			if (item != NULL && item->id == 2)
				nbDS++;
		}
	}

	bool isEnd = nbDS >= 10;
	bool isEvent1 = A_Map::instance->isEventTriggered(EVENT_MAX_1);
	bool isEvent2 = A_Map::instance->isEventTriggered(EVENT_MAX_2);


	A_TextBox::instance->affichertext(convos[isEnd ? 3 : (isEvent1 ? (isEvent2 ? 2 : 1) : 0)]);
	this->direction = (A_Joueur::instance->direction + 2) % 4;
}