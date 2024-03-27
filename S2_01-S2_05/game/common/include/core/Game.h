#pragma once

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <common.h>

#include <core/Actors.h>
#include <core/InputHandler.h>
#include <core/SaveMgr.h>
#include <core/TextMgr.h>

#ifdef _DEBUG
#include <res/debugfont.h>
#endif


class Game {
public:
	Game(int argc, char* argv[]);
	~Game();

	int argc = 0;
	char** argv = NULL;

	InputHandler inputHandler;
	ActorsManager actorMgr;
	SaveMgr saveMgr;
	TextMgr textMgr;
	
	bool pausable = true;
	bool isPaused = false;
	bool gameover = false;
	KeyHandle* escKH = NULL;

	SDL_Texture* pauseTxtTex = NULL;
	int pauseTxtTw = 0, pauseTxtTh = 0;

	SDL_Texture* goTxtText = NULL;
	int goTxtTw = 0, goTxtTh = 0;

	#if defined(__ANDROID__) || defined(SIMULATE_ANDROID)
	SDL_Texture* pauseRsmTex = NULL;
	SDL_Texture* pauseRtyTex = NULL;
	int pauseRsmTw = 0, pauseRsmTh = 0;
	bool wasPaused = false;
	#endif

	int debugStep = 0;
	bool debugMode = false;

	int ww = 0, wh = 0;

	Mix_Music* bgm = NULL;
	int queuedMusicID = -1;
	bool fadeStarted = false;
	bool isQueuedLoop = true;

	static Game* instance;

	int init(const char* title, int xpos, int ypos, int width, int height, u32 flags);

	void handleEvents();
	void update();
	void render();
	void clean();

	void quit();
	void simulateKey(Uint32 keycode, bool isUp);

	bool running() { return isRunning; }
	u32 getFrameCount() { return frameCount; }
	void resetFrameCount() { frameCount = 0; }

	void playMusic(int musicID, bool noLoop=false);
	void stopMusic();

	static int RenderCopyF(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_FRect* dstRect);
	static int RenderCopy(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect);
	static int RenderCopyExF(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_FRect* dstRect, double angle, const SDL_FPoint* center, SDL_RendererFlip flip);
	static int RenderCopyEx(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect, double angle, const SDL_Point* center, SDL_RendererFlip flip);
	static int RenderFillRectF(const SDL_FRect* rect);
	static int RenderFillRect(const SDL_Rect* rect);
	static void Zoom(int value);

private:
	bool isRunning = false;
	bool isFullscreen = false;
	u32 frameCount = 0;

public:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
};

void ChannelFinished(int channel);
void MusicFinished();

#define SOUNDCHANNEL_TEXTBOX 0
#define SOUNDCHANNEL_TEXTBOX_SECONDARY 1
#define SOUNDCHANNEL_PICKUPITEM 2
#define SOUNDCHANNEL_PLAYER 3
#define SOUNDCHANNEL_CHEST 4
#define SOUNDCHANNEL_TITLESCREEN 5
#define SOUNDCHANNEL_GUI 6
#define SOUNDCHANNEL_COUNT 7 // To change on every new channel added to the list

#define MUSIC_MAIN 0
#define MUSIC_INSIDE 1
#define MUSIC_CREDITS 2