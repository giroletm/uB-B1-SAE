#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <common.h>

#include <core/Game.h>

#include "A_Inter.h"

class A_PickupObj : public A_Inter {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	void Interaction();
	void ChannelFinished();

	int uniqueID = -1;
	int type = -1;
	int triggeringEventID = 0;

	bool available = true;

	SDL_Texture* texture = NULL;

	Mix_Chunk* grabbedSfx = NULL;

	static Actor* make();

	static A_PickupObj* latestGrabbed;

};
