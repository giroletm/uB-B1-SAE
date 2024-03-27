#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <common.h>

#include <core/Game.h>
#include <core/Actors.h>

struct PNJConvo {
	const char* text = NULL;
	const char* title = NULL;
	SDL_Color color = {255, 255, 255, 255};
	int fontStyle = TTF_STYLE_NORMAL;

	PNJConvo* followingConvo = NULL;
	int numTriggerEvent = 0;

	int soundID = 0;
	bool (*callback)() = NULL;

	int nbChoices = 0;
	const char** choices = NULL;
	PNJConvo** choicesResult = NULL;
};

class A_TextBox : public Actor {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	void affichertext(PNJConvo* convo);

	SDL_Point texSize = {0, 0};
	SDL_Texture* texture_text = 0;

	std::string bufferedText = "";
	SDL_Texture* bufferedTextTex = NULL;
	int txtW = 0, txtH = 0;

	const char* bufferedTitle = NULL;
	SDL_Texture* bufferedTitleTex = NULL;
	int ttlW = 0, ttlH = 0;

	SDL_Texture* arrow = NULL;
	SDL_Point arrowSize = {0, 0};

	Vec2 posText[2] = {{0.0f, 0.0f}, {0.0f, 0.0f}};
	Vec2 posf = {0.0f, 0.0f};

	bool afficher = false;

	SDL_Point sizes[2] = {{0, 0}, {0, 0}};

	u32 numChar = 0;
	int waitForChar = 0;

	bool showChoices = false;
	int selChoice = 0;

	std::vector<Mix_Chunk*> talks;
	bool isSoundPlaying = false;
	bool isActuallyPlaying = false;

	Mix_Chunk* arrowSound = NULL;
	Mix_Chunk* selectSound = NULL;

	PNJConvo* conv = NULL;

	void RenderTextBox(SDL_Rect* rectText);

	void ChannelFinished();

	static Actor* make();

	static A_TextBox* instance;

	USING_STATES(A_TextBox);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Print);

};
