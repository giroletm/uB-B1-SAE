#include <core/Actors.h>

Actor::~Actor() {}

int Actor::onCreate() {
	return 1;
}
int Actor::onDelete() {
	return 1;
}
int Actor::onExecute() {
	return 1;
}
int Actor::onDraw() {
	return 1;
}

void Actor::Delete() {
	parentMgr->deleteActor(this);
}


ActorsManager::ActorsManager() { }

void ActorsManager::createActor(Actor* actor, SDL_Point* tilePos, u32 settings) {
	if(tilePos)
		actor->tilePos = *tilePos;

	actor->parentMgr = this;
	actor->settings = settings;

	int idx = -1;
	for (int i = 0; i < (int)actors.size(); i++) {
		if (actors[i] == 0) {
			actors[i] = actor;
			idx = i;
			break;
		}
	}
	if(idx >= 0) tasking[idx] = ACTOR_TASKING_CREATE;
	else {
		idx = (int)actors.size();
		actors.push_back(actor);
		tasking.push_back(ACTOR_TASKING_CREATE);
	}

	this->handleActor(idx);
}

#include "../actors/A_Mouse.h"
//#include "../actors/A_Personnage.h"
#include "../actors/A_Joueur.h"
#include "../actors/A_Map.h"
#include "../actors/A_Camera.h"
#include "../actors/A_PNJ_1.h"
#include "../actors/A_TextBox.h"
#include "../actors/A_InvMgr.h"
#include "../actors/A_PickupObj.h"
#include "../actors/A_Titlescreen.h"
#include "../actors/A_Chest.h"
#include "../actors/A_GUI.h"
#include "../actors/A_AndroidCtrl.h"
#include "../actors/A_PNJ_Max.h"
#include "../actors/A_PNJ_Alex.h"
#include "../actors/A_PNJ_2.h"
#include "../actors/A_PNJ_3.h"
#include "../actors/A_PNJ.h"
#include "../actors/A_Cactus.h"
#include "../actors/A_Credits.h"

static Actor*(*makes[])() = {
	&A_Mouse::make,				// 0
	NULL,						// 1
	&A_Joueur::make,			// 2
	&A_Map::make,				// 3
	&A_Camera::make,			// 4
	&A_PNJ_1::make,				// 5
	&A_TextBox::make,			// 6
	&A_InvMgr::make,			// 7
	&A_PickupObj::make,			// 8
	&A_Titlescreen::make,		// 9
	&A_Chest::make,				// 10
	&A_GUI::make,				// 11
	&A_AndroidCtrl::make,		// 12
	&A_PNJ_Max::make,			// 13
	&A_PNJ_Alex::make,			// 14
	&A_PNJ_2::make,				// 15
	&A_PNJ_3::make,				// 16
	&A_PNJ::make,				// 17
	&A_Cactus::make,			// 18
	&A_Credits::make,			// 19
};

Actor* ActorsManager::createActorByID(u32 id, SDL_Point * tilePos, u32 settings) {
	Actor* actor = makes[id]();
	createActor(actor, tilePos, settings);
	return actor;
}

void ActorsManager::deleteActor(Actor* actor) {
	int idx = indexOf(actors, actor);
	if (idx == -1) return;

	tasking[idx] = ACTOR_TASKING_DELETE;

	this->handleActor(idx);
}

void ActorsManager::deleteActors() {
	for (u32 i = 0; i < (u32)actors.size(); i++) {
		if(actors[i])
			this->deleteActor(actors[i]);
	}
}

void ActorsManager::handleActor(int actorIndex) {
	Actor* actor = actors[actorIndex];
	u32 task = tasking[actorIndex];

	if (actor == 0) return;

	if (task == ACTOR_TASKING_EXECUTE) {
		if (actor->activity & Actor::ActorActivity::ACTOR_ACTIVITY_EXECUTE)
			while (actor->onExecute() == 0) {}
	}
	else if (task == ACTOR_TASKING_CREATE) {
		int result = actor->onCreate();
		if (result == 1) tasking[actorIndex] = ACTOR_TASKING_EXECUTE;
		else if (result == 2) tasking[actorIndex] = ACTOR_TASKING_DELETE;
	}
	else if (task == ACTOR_TASKING_DELETE) {
		int result = actor->onDelete();
		if (result == 1) {
			delete actor;
			actors[actorIndex] = 0;
			tasking[actorIndex] = ACTOR_TASKING_NONE;
		}
	}
}

void ActorsManager::handleActors() {
	for (u32 i = 0; i < (u32)actors.size(); i++) {
		this->handleActor(i);
	}
}

void ActorsManager::renderActors() {
	int renderedActors = 0;
	u32 actorsCount = (u32)actors.size();

	int realActors = 0;
	for (u32 i = 0; i < actorsCount; i++) {
		if (actors[i] == 0)
			continue;

		realActors++;
	}

	int currMin = INT_MIN;

	while (renderedActors < realActors) {
		int minLayer = INT_MAX;

		for (u32 i = 0; i < actorsCount; i++) {
			if (actors[i] == 0)
				continue;

			int dO = actors[i]->drawOrder;
			if (dO < minLayer && dO >= currMin) minLayer = actors[i]->drawOrder;
		}

		currMin = minLayer + 1;

		for (u32 i = 0; i < actorsCount; i++) {
			Actor* actor = actors[i];

			if ((actor == 0) || (actor->drawOrder != minLayer))
				continue;

			renderedActors++;

			if (tasking[i] == ACTOR_TASKING_EXECUTE) {
				if (actor->activity & Actor::ActorActivity::ACTOR_ACTIVITY_DRAW)
					while (actor->onDraw() == 0) {}
			}
		}
	}
}