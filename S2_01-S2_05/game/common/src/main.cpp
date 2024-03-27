#include <core/Game.h>

#ifdef __SWITCH__
#include <switch.h>
#include <unistd.h>
#endif

int main(int argc, char* argv[]) {
	#ifdef __ANDROID__
	SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
	#endif

	srand((u32)time((time_t*)NULL));

	#ifdef __SWITCH__
	romfsInit();
	chdir("romfs:/");
	#endif

	const int targetFPS = 60;
	const int frameDelay = 1000 / targetFPS;

	// Init
	Game* game = new Game(argc, argv);

	u32 SDLFlags = SDL_WINDOW_SHOWN;
	#if defined(__WIN32__)
	SDLFlags |= SDL_WINDOW_RESIZABLE;
    #elif defined(__ANDROID__)
    SDLFlags |= SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN;
    #endif

	int ww =
		#if defined(__SWITCH__)
		1280
		#else
		800
		#endif
	;

	int wh =
		#if defined(__SWITCH__)
		720
		#else
		600
		#endif
	;

	int errInit = game->init("SAE201205", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ww, wh, SDLFlags);

	if (errInit == 0) {
		#ifdef __SWITCH__
		SDL_InitSubSystem(SDL_INIT_JOYSTICK);
		SDL_JoystickEventState(SDL_ENABLE);
		SDL_JoystickOpen(0);
		#endif

		while (game->running()
			#ifdef __SWITCH__
			&& appletMainLoop()
			#endif
			) {
			u32 frameStart = SDL_GetTicks();

			// Events & Inputs
			game->handleEvents();

			// Updates
			game->update();

			// Render
			game->render();

			#ifdef _DEBUG
			if (game->debugMode) {
				u32 frameTime2 = SDL_GetTicks() - frameStart;
				if (frameTime2 == 0)
					frameTime2 = 1; // Évitons les divisions par 0

				char* dbgtext;
				SDL_asprintf(&dbgtext, "FPS: %d\nRendering time: %dms (eq %d FPS)", (frameDelay > frameTime2) ? targetFPS : (1000 / frameTime2), frameTime2, (1000 / frameTime2));

				SDL_Surface* txt = WriteDebugText(dbgtext);
				SDL_Rect dst = { 2, 0, txt->w, txt->h };

				SDL_Texture* text = SDL_CreateTextureFromSurface(game->renderer, txt);

				SDL_RenderCopy(game->renderer, text, NULL, &dst);
				SDL_FreeSurface(txt);

				SDL_DestroyTexture(text);
				SDL_free(dbgtext);

				SDL_RenderPresent(game->renderer);
			}
			#endif

			u32 frameTime = SDL_GetTicks() - frameStart;

			if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
		}

		game->clean();
	}
	#ifdef _DEBUG
	else printf("Couldn't initialize SDL2: Error %d\n", errInit);
	#endif

	delete game;

	#ifdef __SWITCH__
	romfsExit();
	#endif

	return 0;
}