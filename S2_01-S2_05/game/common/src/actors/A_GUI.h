#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <common.h>

#include <core/Game.h>
#include <core/Actors.h>

class A_GUI;

struct MNButton {
	SDL_Texture* tex;
	int tw, th;

	void (A_GUI::* callback)(MNButton*);
};

class A_GUI : public Actor {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	SDL_Texture* texturePv = NULL;
	int tPvW = 0, tPvH = 0;

	SDL_Texture* texBtn = NULL;
	int tBtnW = 0, tBtnH = 0;

	SDL_Texture* texWin = NULL;
	SDL_Rect winTL = {0, 0, 5, 5},
		winTR = {6, 0, 5, 5},
		winBL = {0, 6, 5, 5},
		winBR = {6, 6, 5, 5},
		winT = {5, 0, 1, 5},
		winB = {5, 6, 1, 5 },
		winL = {0, 5, 5, 1},
		winR = {6, 5, 5, 1},
		winM = {5, 5, 1, 1},
		winITL = {11, 1, 5, 5},
		winITR = {15, 1, 5, 5},
		winIBL = {11, 5, 5, 5},
		winIBR = {15, 5, 5, 5};

	TTF_Font* fnt = NULL;

	u8 pressed = 0;
	u8 released = 0;

	bool isUnrolled = false;

	int animTimer = 0;
	int animTime = 30;
	int animHalfTime = animTime/2;

	std::vector<MNButton> buttons;

	Mix_Chunk* clickSfx = NULL;

	void AddButton(const char* text, void (A_GUI::* callback)(MNButton*));
	void GetBtnRect(SDL_Rect* dstRect);

	void Inventory(MNButton* btn);
	void Save(MNButton* btn);
	void Quit(MNButton* btn);

	static Actor* make();

	static A_GUI* instance;

	USING_STATES(A_GUI);
	DECLARE_STATE(Wait);
	DECLARE_STATE(AppearMenu);
	DECLARE_STATE(DisappearMenu);
};
