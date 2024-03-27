#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <common.h>

#include <core/Game.h>
#include <core/Actors.h>

class A_Titlescreen;

struct TSButton {
	std::string text;
	void (A_Titlescreen::* callback)();
	bool clicked;

	double x;
	double y;
	double w;
	double h;

	SDL_Texture* txtT;
};

class A_Titlescreen : public Actor {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	SDL_Texture* texLogo = NULL;
	SDL_Texture* texBG = NULL;
	SDL_Texture* texBtn = NULL;
	SDL_Texture* texBtnC = NULL;

	Actor* decoWM = NULL;

	std::vector<TSButton> buttons;

	bool deleted = false;

	int fadingOpacity = -1;
	int fadeTime = 60;
	bool reverseFade = false;

	Mix_Chunk* clickSfx = NULL;

	static Actor* make();

	void calculateButtonRects();

	void NewGame();
	void Continue();
	void Exit();

	USING_STATES(A_Titlescreen);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Fade);
};
