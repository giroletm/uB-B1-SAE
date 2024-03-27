#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <common.h>

#include <core/Game.h>

#include "A_Inter.h"
#include "A_TextBox.h"

class A_PNJ_Alex : public A_Inter {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	void Interaction();
	void Portable();

	int direction = 1;		 // 0 = droite, 1 = bas, 2 = gauche, 3 = haut;

	SDL_Texture* texture = 0;
	int pTW = 0, pTH = 0;

	Vec2 posText = {0.0f, 0.0f};
	Vec2 posc = {0.0f, 0.0f};

	std::vector<PNJConvo*> convos;

	static Actor* make();

	static A_PNJ_Alex* instance;

};
#pragma once
