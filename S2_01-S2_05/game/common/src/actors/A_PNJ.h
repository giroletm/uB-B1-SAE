#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <common.h>

#include <core/Game.h>

class A_PNJ : public Actor {
public:
	int onCreate();

	static Actor* make();
};