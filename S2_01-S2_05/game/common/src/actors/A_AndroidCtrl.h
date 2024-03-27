#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <common.h>

#include <core/Game.h>
#include <core/Actors.h>

class A_AndroidCtrl : public Actor {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	SDL_Texture* texPad = NULL;
	int tPadW = 0, tPadH = 0;

	SDL_Texture* texInt = NULL;
	int tIntW = 0, tIntH = 0;

	SDL_Texture* texBack = NULL;
	SDL_Texture* texPause = NULL;
	int tPauseW = 0, tPauseH = 0;


	int timer = 0;

	u8 pressed = 0;

	void GetPadRect(SDL_Rect* dstRect);
	void GetIntRect(SDL_Rect* dstRect);
	void GetPauseRect(SDL_Rect* dstRect);

	void ReduceRectForTimer(SDL_Rect* dstRect);

	static Actor* make();

	static A_AndroidCtrl* instance;
};
