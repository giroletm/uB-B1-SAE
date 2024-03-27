#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <common.h>

#include <core/Game.h>

#include "A_TextBox.h"

class A_Cactus : public Actor {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();


	SDL_Texture* texture = 0;
	int pTW = 0, pTH = 0;

	int direct = -1;

	bool regarde = 0;

	static Actor* make();

	USING_STATES(A_Cactus);
	DECLARE_STATE(Hit);
	DECLARE_STATE(Wait);

};
#pragma once
