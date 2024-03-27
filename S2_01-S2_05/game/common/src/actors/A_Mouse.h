#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <common.h>

#include <core/Game.h>
#include <core/Actors.h>

class A_Mouse : public Actor {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();
	
	SDL_Texture* tex = NULL;
	SDL_Texture* tex2 = NULL;
	SDL_Texture* tex3 = NULL;
	SDL_Texture* tex4 = NULL;

	static Actor* make();
};