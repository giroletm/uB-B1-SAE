#include <core/Game.h>
#include <res/debugfont.h>
#include "../actors/A_Map.h"
#include "../actors/A_Joueur.h"

Game* Game::instance = 0;

Game::Game(int argc, char* argv[]) : saveMgr(), textMgr() {
	instance = this;

	this->argc = argc;
	this->argv = argv;

	window = 0;
	renderer = 0;
}

Game::~Game() {
	instance = 0;
}

int Game::init(const char* title, int xpos, int ypos, int width, int height, u32 flags) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (!window) return -2; // Coudln't create window

		u32 rendererFlags = 
			#ifdef __SWITCH__
			SDL_RENDERER_SOFTWARE
			#else
			SDL_RENDERER_TARGETTEXTURE
			#endif
		;
		renderer = SDL_CreateRenderer(window, -1, rendererFlags);
		if (!renderer) return -3; // Coudln't create renderer

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		if (TTF_Init() != 0)
			return -4; // Couldn't init SDL2_ttf

		if (Mix_Init(0) != 0)
			return -5; // Couldn't init SDL2_mixer

		SDL_InitSubSystem(SDL_INIT_AUDIO);
		Mix_AllocateChannels(SOUNDCHANNEL_COUNT);

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0)
			return -6; // Couldn't open audio

		Mix_ChannelFinished(&ChannelFinished);
		Mix_HookMusicFinished(&MusicFinished);

		isRunning = true;
		ww = width;
		wh = height;

		this->actorMgr.createActorByID(9); // Titlescreen

		this->escKH = this->inputHandler.getKeyHandle(SDLK_ESCAPE);
		SDL_Surface* surf = WriteDebugText(TextMgr::instance->GetValue("pause.paused"), false, 48);
		this->pauseTxtTw = surf->w;
		this->pauseTxtTh = surf->h;
		this->pauseTxtTex = SDL_CreateTextureFromSurface(this->renderer, surf);
		SDL_FreeSurface(surf);

		SDL_Surface* surfgo = WriteDebugText(TextMgr::instance->GetValue("pause.gameover"), false, 48);
		this->goTxtTw = surfgo->w;
		this->goTxtTh = surfgo->h;
		this->goTxtText = SDL_CreateTextureFromSurface(this->renderer, surfgo);
		SDL_FreeSurface(surfgo);

		#if defined(__ANDROID__) || defined(SIMULATE_ANDROID)
		this->pauseRsmTex = IMG_LoadTexture(this->renderer, "assets/UI/ButtonResume.png");
		this->pauseRtyTex = IMG_LoadTexture(this->renderer, "assets/UI/ButtonRetry.png");
		SDL_QueryTexture(this->pauseRsmTex, NULL, NULL, &this->pauseRsmTw, &this->pauseRsmTh);
		#endif

		return 0;
	}
	else {
		return -1; // Couldn't init SDL2
	}
}


#define JOYCON_A     0
#define JOYCON_B     1
#define JOYCON_X     2
#define JOYCON_Y     3
#define JOYCON_PLUS  10
#define JOYCON_LEFT  12
#define JOYCON_UP    13
#define JOYCON_RIGHT 14
#define JOYCON_DOWN  15

void Game::handleEvents() {
	inputHandler.updateKeys();
	inputHandler.updateMouse();

	SDL_Event event;
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT: {
				isRunning = false;
				break;
			}
			case SDL_KEYDOWN: {
				inputHandler.keyStateChanged(event.key.keysym.sym, false);

				if (event.key.keysym.sym == SDLK_F11) {
					u32 flags = isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP;
					SDL_SetWindowFullscreen(window, flags);
					isFullscreen = !isFullscreen;
				}

				break;
			}
			case SDL_KEYUP: {
				inputHandler.keyStateChanged(event.key.keysym.sym, true);
				break;
			}
			case SDL_MOUSEBUTTONDOWN: {
				u32 buttonState = SDL_GetMouseState(NULL, NULL);

				if (buttonState & SDL_BUTTON(SDL_BUTTON_LEFT))
					inputHandler.keyStateChanged(SDL_BUTTON_LEFT, false);
				if (buttonState & SDL_BUTTON(SDL_BUTTON_MIDDLE))
					inputHandler.keyStateChanged(SDL_BUTTON_MIDDLE, false);
				if (buttonState & SDL_BUTTON(SDL_BUTTON_RIGHT))
					inputHandler.keyStateChanged(SDL_BUTTON_RIGHT, false);

				break;
			}
			case SDL_MOUSEBUTTONUP: {
				u32 buttonState = SDL_GetMouseState(NULL, NULL);

				if (!(buttonState & SDL_BUTTON(SDL_BUTTON_LEFT)))
					inputHandler.keyStateChanged(SDL_BUTTON_LEFT, true);
				if (!(buttonState & SDL_BUTTON(SDL_BUTTON_MIDDLE)))
					inputHandler.keyStateChanged(SDL_BUTTON_MIDDLE, true);
				if (!(buttonState & SDL_BUTTON(SDL_BUTTON_RIGHT)))
					inputHandler.keyStateChanged(SDL_BUTTON_RIGHT, true);

				break;
			}
			case SDL_MOUSEWHEEL: {
				Zoom(event.wheel.y);

				break;
			}
			case SDL_WINDOWEVENT: {
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
					SDL_GetRendererOutputSize(this->renderer, &ww, &wh);

				break;
			}
			#ifdef __SWITCH__
			case SDL_JOYBUTTONDOWN: {
				switch (event.jbutton.button) {
					case JOYCON_UP: {
						inputHandler.keyStateChanged(SDLK_UP, false);
						break;
					}
					case JOYCON_DOWN: {
						inputHandler.keyStateChanged(SDLK_DOWN, false);
						break;
					}
					case JOYCON_LEFT: {
						inputHandler.keyStateChanged(SDLK_LEFT, false);
						break;
					}
					case JOYCON_RIGHT: {
						inputHandler.keyStateChanged(SDLK_RIGHT, false);
						break;
					}
					case JOYCON_PLUS: {
						inputHandler.keyStateChanged(SDLK_ESCAPE, false);
						break;
					}
					case JOYCON_A: {
						inputHandler.keyStateChanged(SDL_GetKeyFromScancode(SDL_SCANCODE_Z), false);
						inputHandler.keyStateChanged(SDL_GetKeyFromScancode(SDL_SCANCODE_X), false);
						inputHandler.keyStateChanged(SDL_GetKeyFromScancode(SDL_SCANCODE_C), false);
						inputHandler.keyStateChanged(SDLK_RETURN, false);
						break;
					}
					case JOYCON_X: {
						inputHandler.keyStateChanged(SDLK_i, false);
						break;
					}
				}
				break;
			}
			case SDL_JOYBUTTONUP: {
				switch (event.jbutton.button) {
					case JOYCON_UP: {
						inputHandler.keyStateChanged(SDLK_UP, true);
						break;
					}
					case JOYCON_DOWN: {
						inputHandler.keyStateChanged(SDLK_DOWN, true);
						break;
					}
					case JOYCON_LEFT: {
						inputHandler.keyStateChanged(SDLK_LEFT, true);
						break;
					}
					case JOYCON_RIGHT: {
						inputHandler.keyStateChanged(SDLK_RIGHT, true);
						break;
					}
					case JOYCON_PLUS: {
						inputHandler.keyStateChanged(SDLK_ESCAPE, true);
						break;
					}
					case JOYCON_A: {
						inputHandler.keyStateChanged(SDL_GetKeyFromScancode(SDL_SCANCODE_X), true);
						break;
					}
					case JOYCON_X: {
						inputHandler.keyStateChanged(SDLK_i, true);
						break;
					}
				}
				break;
			}
			case SDL_FINGERDOWN: {
				inputHandler.mouseHandle.x = event.tfinger.x * ww;
				inputHandler.mouseHandle.y = event.tfinger.y * wh;
				inputHandler.keyStateChanged(SDL_BUTTON_LEFT, false);
				break;
			}
			case SDL_FINGERUP: {
				inputHandler.mouseHandle.x = event.tfinger.x * ww;
				inputHandler.mouseHandle.y = event.tfinger.y * wh;
				inputHandler.keyStateChanged(SDL_BUTTON_LEFT, true);
				break;
			}
			case SDL_FINGERMOTION: {
				inputHandler.mouseHandle.x = event.tfinger.x * ww;
				inputHandler.mouseHandle.y = event.tfinger.y * wh;
				break;
			}
			#endif
			default: {
				break;
			}
		}
	}
}

void Game::update() {
	frameCount++;

	if (this->escKH->pressed && this->pausable) {
		if (A_Map::instance) {
			if (this->isPaused)
				A_Map::instance->activity |= Actor::ACTOR_ACTIVITY_EXECUTE;
			else
				A_Map::instance->activity &= ~Actor::ACTOR_ACTIVITY_EXECUTE;
		}

		this->isPaused = !this->isPaused;
	}
	
	if (this->isPaused || (A_Joueur::instance && A_Joueur::instance->pv <= 0)) {
		if (A_Joueur::instance->pv <= 0) {
			KeyHandle* returnShift = this->inputHandler.getKeyHandle(SDLK_RETURN);

			if (!gameover) {
				A_Map::instance->activity &= ~Actor::ACTOR_ACTIVITY_EXECUTE;
				gameover = true;
			}

			if (returnShift->pressed) {
				A_Map::instance->activity |= Actor::ACTOR_ACTIVITY_EXECUTE;
				A_Map::instance->Load(A_Map::instance->currentFilename.c_str(), A_Map::instance->currentEntranceID);
			}
		}
		
		#if defined(__ANDROID__) || defined(SIMULATE_ANDROID)
		if ((this->isPaused && this->pausable) || this->gameover) {
			MouseHandle* mh = &this->inputHandler.mouseHandle;
			KeyHandle* leftKH = this->inputHandler.getKeyHandle(SDL_BUTTON_LEFT);

			int atPauseH = this->wh / 6;
			int atPauseW = (int)(this->pauseRsmTw * ((double)atPauseH / (double)this->pauseRsmTh));
			SDL_Rect pausePos = { 10, 10, atPauseW, atPauseH };

			if (IsPtInRect(pausePos.x, pausePos.y, pausePos.x + pausePos.w, pausePos.y + pausePos.h, mh->x, mh->y)) {
				if (this->isPaused) {
					if (leftKH->pressed)
						this->wasPaused = true;
					else if (this->wasPaused && leftKH->released) {
						this->wasPaused = false;
						this->simulateKey(SDLK_ESCAPE, false);
						this->simulateKey(SDLK_ESCAPE, true);
					}
				}
				else if (leftKH->pressed) {
					this->simulateKey(SDLK_RETURN, false);
					this->simulateKey(SDLK_RETURN, true);
				}
			}
		}

		return;

		#endif
	}

	this->actorMgr.handleActors();

	#ifdef _DEBUG
	if (inputHandler.getKeyHandle(SDLK_p)->pressed)
		debugMode = !debugMode;

	if (inputHandler.getKeyHandle(SDLK_o)->pressed && A_Map::instance)
		A_Map::instance->Load(A_Map::instance->currentFilename.c_str(), A_Map::instance->currentEntranceID);

	if (inputHandler.getKeyHandle(SDLK_1)->pressed)
		A_Map::instance->TriggerEvent(1);

	if (inputHandler.getKeyHandle(SDLK_2)->pressed)
		A_Map::instance->TriggerEvent(2);

	if (inputHandler.getKeyHandle(SDLK_3)->pressed)
		A_Joueur::instance->AddInventoryItem(2);
	#endif
}

void Game::render() {
	SDL_RenderClear(renderer);

	this->actorMgr.renderActors();
	
	if (isPaused || gameover) {
		SDL_Color col;
		SDL_GetRenderDrawColor(this->renderer, &col.r, &col.g, &col.b, &col.a);

		SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 128);
		SDL_RenderFillRect(this->renderer, NULL);

		SDL_SetRenderDrawColor(this->renderer, col.r, col.g, col.b, col.a);

		SDL_Rect dstR;
		if (isPaused) {
			dstR = { (this->ww / 2) - (pauseTxtTw / 2), (this->wh / 2) - (pauseTxtTh / 2), pauseTxtTw, pauseTxtTh };
		}
		else {
			dstR = { (this->ww / 2) - (goTxtTw / 2), (this->wh / 2) - (goTxtTh / 2), goTxtTw, goTxtTh };
		}

		SDL_RenderCopy(this->renderer, isPaused ? this->pauseTxtTex : this->goTxtText, NULL, &dstR);

		#if defined(__ANDROID__) || defined(SIMULATE_ANDROID)
		int atPauseH = this->wh / 6;
		int atPauseW = (int)(this->pauseRsmTw * ((double)atPauseH / (double)this->pauseRsmTh));
		dstR = { 10, 10, atPauseW, atPauseH };
		SDL_RenderCopy(this->renderer, this->isPaused ? this->pauseRsmTex : this->pauseRtyTex, NULL, &dstR);
		#endif
	}

	if (!this->debugMode) // If we're in debug mode, RenderPresent will be called in main
		SDL_RenderPresent(renderer);
}

void Game::clean() {
	this->actorMgr.deleteActors();
	
	IMG_Quit();
	Mix_Quit();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::quit() {
	SDL_Event event;
	event.type = event.quit.type = SDL_QUIT;
	event.quit.timestamp = SDL_GetTicks();

	SDL_PushEvent(&event);
}

void Game::simulateKey(Uint32 keycode, bool isUp) {
	SDL_Event event;
	event.type = event.key.type = isUp ? SDL_KEYUP : SDL_KEYDOWN;
	event.key.keysym.mod = 0;
	event.key.keysym.sym = keycode;
	event.key.keysym.scancode = SDL_GetScancodeFromKey(event.key.keysym.sym);
	event.key.windowID = SDL_GetWindowID(this->window);
	event.key.state = isUp ? SDL_RELEASED : SDL_PRESSED;
	event.key.timestamp = SDL_GetTicks();

	SDL_PushEvent(&event);
}

void Game::playMusic(int musicID, bool noLoop) {
	this->queuedMusicID = musicID;
	this->isQueuedLoop = !noLoop;

	if (bgm) {
		if (!fadeStarted) {
			Mix_FadeOutMusic(1000);
			fadeStarted = true;
		}
	}
	else
		MusicFinished();

}

void Game::stopMusic() {
	this->playMusic(-1);
}

#include "../actors/A_TextBox.h"
#include "../actors/A_PickupObj.h"

void ChannelFinished(int channel) {
	switch (channel) {
		case SOUNDCHANNEL_TEXTBOX:
			A_TextBox::instance->ChannelFinished();
			break;
			
		case SOUNDCHANNEL_PICKUPITEM:
			A_PickupObj::latestGrabbed->ChannelFinished();
			break;

		default:
			break;
	}
}

void MusicFinished() {
	Game::instance->fadeStarted = false;

	if (Game::instance->bgm) {
		Mix_FreeMusic(Game::instance->bgm);
		Game::instance->bgm = NULL;
	}

	if (Game::instance->queuedMusicID >= 0) {
		switch (Game::instance->queuedMusicID) {
			case 0:
				Game::instance->bgm = Mix_LoadMUS("assets/Audio/MusicMain.wav");
				break;
			case 1:
				Game::instance->bgm = Mix_LoadMUS("assets/Audio/MusicInside.wav");
				break;
			case 2:
				Game::instance->bgm = Mix_LoadMUS("assets/Audio/MusicCredits.wav");
				break;
		}

		Mix_FadeInMusic(Game::instance->bgm, Game::instance->isQueuedLoop ? -1 : 0, 1000);
	}

	Game::instance->queuedMusicID = -1;
}

#include "../actors/A_Camera.h"

int Game::RenderCopyF(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_FRect* dstRect) {
	return A_Camera::instance->RenderCopyF(texture, srcRect, dstRect);
}

int Game::RenderCopy(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect) {
	return A_Camera::instance->RenderCopy(texture, srcRect, dstRect);
}

int Game::RenderCopyExF(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_FRect* dstRect, double angle, const SDL_FPoint* center, SDL_RendererFlip flip) {
	return A_Camera::instance->RenderCopyExF(texture, srcRect, dstRect, angle, center, flip);
}

int Game::RenderCopyEx(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect, double angle, const SDL_Point* center, SDL_RendererFlip flip) {
	return A_Camera::instance->RenderCopyEx(texture, srcRect, dstRect, angle, center, flip);
}

int Game::RenderFillRectF(const SDL_FRect* rect) {
	return A_Camera::instance->RenderFillRectF(rect);
}

int Game::RenderFillRect(const SDL_Rect* rect) {
	return A_Camera::instance->RenderFillRect(rect);
}

void Game::Zoom(int value) {
	A_Camera::instance->Zoom(value);
}