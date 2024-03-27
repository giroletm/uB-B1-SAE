
#include "A_Credits.h"
#include <res/debugfont.h>



Actor* A_Credits::make() {
	return new A_Credits();
}

int A_Credits::onCreate() {

	Game::instance->pausable = false;

	this->y = 1.1;
	this->lowestY = 1.0;
	
	this->tableauDeTexturesTexte = new SDL_Texture* [TOTAL_CREDITS_LINES];
	for (int i = 1; i <= TOTAL_CREDITS_LINES; i++) {
		if (i == 5) {
			tableauDeTexturesTexte[i-1] = IMG_LoadTexture(Game::instance->renderer, "assets/UI/Logo.png");
			continue;
		}

		char* ptr;
		SDL_asprintf(&ptr, "credits.line.%d", i - ((i > 5) ? 1 : 0));
		SDL_Surface* txt = WriteDebugText(TextMgr::instance->GetValue(ptr), false, 48, (((i % 2) == 1) && (i > 6)) ? TTF_STYLE_UNDERLINE : TTF_STYLE_NORMAL, TTF_WRAPPED_ALIGN_CENTER);
		tableauDeTexturesTexte[i-1] = SDL_CreateTextureFromSurface(Game::instance->renderer, txt);
		SDL_free(ptr);

		SDL_FreeSurface(txt);
	}

	Game::instance->playMusic(MUSIC_CREDITS, true);

	return 1;
}

int A_Credits::onDelete() {
	for (int i = 1; i <= TOTAL_CREDITS_LINES; i++)
		SDL_DestroyTexture(tableauDeTexturesTexte[i - 1]);

	delete[] this->tableauDeTexturesTexte;

	return 1;
}

int A_Credits::onExecute() {
	this->y -= 0.0025;

	bool interHeld = Game::instance->inputHandler.getHeldInteraction();
	KeyHandle* leftKH = Game::instance->inputHandler.getKeyHandle(SDL_BUTTON_LEFT);
	KeyHandle* escKH = Game::instance->inputHandler.getKeyHandle(SDLK_ESCAPE);

	if (interHeld || leftKH->held) {
		this->y -= 0.005;
		this->skippedFrames++;
	}

	if ((this->lowestY < 0.0 && ((skippedFrames > 30) || !Mix_PlayingMusic())) || escKH->pressed) {
		if (escKH->pressed)
			Game::instance->inputHandler.consumeKey(SDLK_ESCAPE);

		Game::instance->pausable = true;
		
		SaveMgr::instance->Save();

		Game::instance->actorMgr.createActorByID(9); // Titlescreen
		this->Delete();
	}

	return 1;
}
int A_Credits::onDraw() {
	renderCreditsText();


	return 1;
}
void A_Credits::renderCreditsText() {
	double slide = this->y;
	for (int i = 1; i <= TOTAL_CREDITS_LINES; i++) {
		int w, h;
		SDL_QueryTexture(tableauDeTexturesTexte[i-1], NULL, NULL, &w, &h);
		
		int nw = (int)(w * Game::instance->ww / 1920.0);
		int nh = (int)(h * (nw / (double)w));

		if (i == 5) {
			nw /= 2;
			nh /= 2;
		}

		SDL_Rect rect = { (Game::instance->ww / 2) - (nw / 2), (int)(slide * Game::instance->wh), nw, nh };
		SDL_RenderCopy(Game::instance->renderer, tableauDeTexturesTexte[i-1], NULL, &rect);
		slide += (nh + 10) / (double)Game::instance->wh;
	}

	lowestY = slide;
}

