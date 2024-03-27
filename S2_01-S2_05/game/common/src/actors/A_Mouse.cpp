#include "A_Mouse.h"


Actor* A_Mouse::make() {
	return new A_Mouse();
}


int A_Mouse::onCreate() {
	tex = IMG_LoadTexture(Game::instance->renderer, "assets/UI/cursor.png");
	tex2 = IMG_LoadTexture(Game::instance->renderer, "assets/UI/cursor2.png");
	tex3 = IMG_LoadTexture(Game::instance->renderer, "assets/UI/cursor3.png");
	tex4 = IMG_LoadTexture(Game::instance->renderer, "assets/UI/cursor4.png");

	return true;
}

int A_Mouse::onDelete() {
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(tex2);
	SDL_DestroyTexture(tex3);
	SDL_DestroyTexture(tex4);

	return 1;
}

int A_Mouse::onExecute() {
	if (this->settings == 0) {
		MouseHandle* mh = &Game::instance->inputHandler.mouseHandle;
		this->pos.x = (float)mh->x;
		this->pos.y = (float)mh->y;
	}

	return 1;
}

int A_Mouse::onDraw() {
	SDL_Rect rect = { (int)(this->pos.x - 2), (int)(this->pos.y - 2), 5, 5 };

	KeyHandle* khL = Game::instance->inputHandler.getKeyHandle(SDL_BUTTON_LEFT);
	KeyHandle* khR = Game::instance->inputHandler.getKeyHandle(SDL_BUTTON_RIGHT);
	KeyHandle* khM = Game::instance->inputHandler.getKeyHandle(SDL_BUTTON_MIDDLE);

	SDL_Texture* t = 0;
	if (khL->held) t = tex2;
	else if (khR->held) t = tex3;
	else if (khM->held) t = tex4;
	else t = tex;

	SDL_RenderCopy(Game::instance->renderer, t, NULL, &rect);


	return 1;
}