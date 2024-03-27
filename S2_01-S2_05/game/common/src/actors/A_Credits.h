#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <common.h>
#include <core/Actors.h>
#include <core/Game.h>


#define TOTAL_CREDITS_LINES 40

class A_Credits : public Actor {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	void renderCreditsText();

	SDL_Texture** tableauDeTexturesTexte = NULL;

	bool pressed = false;
	double y = 0;
	double lowestY = 0;

	int skippedFrames = 0;

	static Actor* make();

	static A_Credits* instance;
};