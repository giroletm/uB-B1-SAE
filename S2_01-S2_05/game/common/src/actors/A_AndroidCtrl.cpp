#include "A_AndroidCtrl.h"
#include <res/debugfont.h>

A_AndroidCtrl* A_AndroidCtrl::instance = 0;

Actor* A_AndroidCtrl::make() {
	instance = new A_AndroidCtrl();
	return instance;
}

int A_AndroidCtrl::onCreate() {
	this->drawOrder = 2;

	texPad = IMG_LoadTexture(Game::instance->renderer, "assets/UI/pad.png");
	SDL_QueryTexture(texPad, NULL, NULL, &tPadW, &tPadH);

	texInt = IMG_LoadTexture(Game::instance->renderer, "assets/UI/ButtonInteract.png");
	SDL_QueryTexture(texInt, NULL, NULL, &tIntW, &tIntH);

	texBack = IMG_LoadTexture(Game::instance->renderer, "assets/UI/ButtonBack.png");
	texPause = IMG_LoadTexture(Game::instance->renderer, "assets/UI/ButtonPause.png");
	SDL_QueryTexture(texPause, NULL, NULL, &tPauseW, &tPauseH);

	return 1;
}

int A_AndroidCtrl::onDelete() {
	SDL_DestroyTexture(texPad);
	SDL_DestroyTexture(texInt);
	SDL_DestroyTexture(texBack);
	SDL_DestroyTexture(texPause);

	instance = 0;

	return 1;
}

void A_AndroidCtrl::GetPadRect(SDL_Rect* dstRect) {
	if (dstRect == NULL)
		return;

	int atPadH = Game::instance->wh / 3;
	int atPadW = (int)(tPadW * ((double)atPadH / (double)tPadH));

	*dstRect = { 10, Game::instance->wh - atPadH - 10, atPadW, atPadH };

	ReduceRectForTimer(dstRect);
}

void A_AndroidCtrl::GetIntRect(SDL_Rect* dstRect) {
	if (dstRect == NULL)
		return;

	int atIntH = Game::instance->wh / 3;
	int atIntW = (int)(tIntW * ((double)atIntH / (double)tIntH));

	*dstRect = { Game::instance->ww - atIntW - 10, Game::instance->wh - atIntH - 10, atIntW, atIntH };

	ReduceRectForTimer(dstRect);
}

void A_AndroidCtrl::GetPauseRect(SDL_Rect* dstRect) {
	if (dstRect == NULL)
		return;

	int atPauseH = Game::instance->wh / 6;
	int atPauseW = (int)(tPauseW * ((double)atPauseH / (double)tPauseH));

	*dstRect = { 10, 10, atPauseW, atPauseH };

	ReduceRectForTimer(dstRect);
}

void A_AndroidCtrl::ReduceRectForTimer(SDL_Rect* dstRect) {
	if (dstRect == NULL)
		return;

	if (this->timer < 60) {
		int nw = (int)SDL_round(dstRect->w * (this->timer / 60.0));
		int nh = (int)SDL_round(dstRect->h * (this->timer / 60.0));

		dstRect->x += (dstRect->w - nw) / 2;
		dstRect->y += (dstRect->h - nh) / 2;
		dstRect->w = nw;
		dstRect->h = nh;
	}
}

int A_AndroidCtrl::onExecute() {
	SDL_Rect padPos, intPos, pausePos;
	GetPadRect(&padPos);
	GetIntRect(&intPos);
	GetPauseRect(&pausePos);

	if (this->timer < 60) {
		this->timer++;
	}

	SDL_Rect topRect = { padPos.x, padPos.y, padPos.w, (int)SDL_round(padPos.h / 2.91) };
	SDL_Rect bottomRect = { padPos.x, padPos.y + (int)SDL_round(padPos.h / 1.52), padPos.w, topRect.h };
	SDL_Rect leftRect = { padPos.x, padPos.y, (int)SDL_round(padPos.w / 2.91), padPos.h };
	SDL_Rect rightRect = { padPos.x + (int)SDL_round(padPos.w / 1.52), padPos.y, leftRect.w, padPos.h };

	u8 previousPress = pressed;

	MouseHandle* mh = &Game::instance->inputHandler.mouseHandle;
	KeyHandle* leftKH = Game::instance->inputHandler.getKeyHandle(SDL_BUTTON_LEFT);

	if (IsPtInRect(topRect.x, topRect.y, topRect.x + topRect.w, topRect.y + topRect.h, mh->x, mh->y) && leftKH->held)
		pressed |= 1;
	else
		pressed &= ~1;

	if (IsPtInRect(bottomRect.x, bottomRect.y, bottomRect.x + bottomRect.w, bottomRect.y + bottomRect.h, mh->x, mh->y) && leftKH->held)
		pressed |= 2;
	else
		pressed &= ~2;

	if (IsPtInRect(leftRect.x, leftRect.y, leftRect.x + leftRect.w, leftRect.y + leftRect.h, mh->x, mh->y) && leftKH->held)
		pressed |= 4;
	else
		pressed &= ~4;

	if (IsPtInRect(rightRect.x, rightRect.y, rightRect.x + rightRect.w, rightRect.y + rightRect.h, mh->x, mh->y) && leftKH->held)
		pressed |= 8;
	else
		pressed &= ~8;

	if (IsPtInRect(intPos.x, intPos.y, intPos.x + intPos.w, intPos.y + intPos.h, mh->x, mh->y) && leftKH->held)
		pressed |= 16;
	else
		pressed &= ~16;

	if (IsPtInRect(pausePos.x, pausePos.y, pausePos.x + pausePos.w, pausePos.y + pausePos.h, mh->x, mh->y) && leftKH->released)
		pressed |= 32;
	else
		pressed &= ~32;

	if (!(previousPress & 1) && (pressed & 1))
		Game::instance->simulateKey(SDLK_UP, false);

	if (!(previousPress & 2) && (pressed & 2))
		Game::instance->simulateKey(SDLK_DOWN, false);

	if (!(previousPress & 4) && (pressed & 4))
		Game::instance->simulateKey(SDLK_LEFT, false);

	if (!(previousPress & 8) && (pressed & 8))
		Game::instance->simulateKey(SDLK_RIGHT, false);

	if (!(previousPress & 16) && (pressed & 16)) {
		Game::instance->simulateKey(SDL_GetKeyFromScancode(SDL_SCANCODE_Z), false);
		Game::instance->simulateKey(SDL_GetKeyFromScancode(SDL_SCANCODE_X), false);
		Game::instance->simulateKey(SDL_GetKeyFromScancode(SDL_SCANCODE_C), false);
	}

	if (!(previousPress & 32) && (pressed & 32))
		Game::instance->simulateKey(SDLK_ESCAPE, false);


	if ((previousPress & 1) && !(pressed & 1))
		Game::instance->simulateKey(SDLK_UP, true);

	if ((previousPress & 2) && !(pressed & 2))
		Game::instance->simulateKey(SDLK_DOWN, true);

	if ((previousPress & 4) && !(pressed & 4))
		Game::instance->simulateKey(SDLK_LEFT, true);

	if ((previousPress & 8) && !(pressed & 8))
		Game::instance->simulateKey(SDLK_RIGHT, true);

	if ((previousPress & 16) && !(pressed & 16)) {
		Game::instance->simulateKey(SDL_GetKeyFromScancode(SDL_SCANCODE_Z), true);
		Game::instance->simulateKey(SDL_GetKeyFromScancode(SDL_SCANCODE_X), true);
		Game::instance->simulateKey(SDL_GetKeyFromScancode(SDL_SCANCODE_C), true);
	}

	if ((previousPress & 32) && !(pressed & 32))
		Game::instance->simulateKey(SDLK_ESCAPE, true);





	return 1;
}

int A_AndroidCtrl::onDraw() {
	SDL_Rect rndPos;

	GetPadRect(&rndPos);
	SDL_RenderCopy(Game::instance->renderer, texPad, NULL, &rndPos);

	GetIntRect(&rndPos);
	SDL_RenderCopy(Game::instance->renderer, texInt, NULL, &rndPos);

	GetPauseRect(&rndPos);
	SDL_RenderCopy(Game::instance->renderer, Game::instance->pausable ? texPause : texBack, NULL, &rndPos);

	return 1;
}
