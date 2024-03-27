#include "A_Titlescreen.h"
#include <res/debugfont.h>

CREATE_STATE(A_Titlescreen, Wait);
CREATE_STATE(A_Titlescreen, Fade);

Actor* A_Titlescreen::make() {
	return new A_Titlescreen();
}

int A_Titlescreen::onCreate() {
	this->drawOrder = 1;

	Game::instance->pausable = false;

	texLogo = IMG_LoadTexture(Game::instance->renderer, "assets/UI/Logo.png");
	texBG = IMG_LoadTexture(Game::instance->renderer, "assets/UI/TitleBG.png");
	texBtn = IMG_LoadTexture(Game::instance->renderer, "assets/UI/MultiButton.png");
	texBtnC = IMG_LoadTexture(Game::instance->renderer, "assets/UI/MultiButtonC.png");

	buttons.push_back({ TextMgr::instance->GetValue("titlescreen.newgame"), &A_Titlescreen::NewGame, false });
	if(SaveMgr::instance->WasFileLoaded())
		buttons.push_back({ TextMgr::instance->GetValue("titlescreen.continue"), &A_Titlescreen::Continue, false });
	buttons.push_back({ TextMgr::instance->GetValue("titlescreen.exit"), &A_Titlescreen::Exit, false });

	this->calculateButtonRects();

	if(!decoWM)
		decoWM = Game::instance->actorMgr.createActorByID(3, NULL, 99); // Map

	this->fadingOpacity = 60;
	this->reverseFade = true;

	this->clickSfx = Mix_LoadWAV("assets/Audio/MenuSelect.wav");

	Game::instance->playMusic(MUSIC_MAIN);

	acState.setState(&StateID_Fade);

	return 1;
}

int A_Titlescreen::onDelete() {
	Game::instance->pausable = true;

	u32 nbButtons = (u32)buttons.size();
	for (u32 i = 0; i < nbButtons; i++) {
		SDL_DestroyTexture(buttons[i].txtT);
	}

	SDL_DestroyTexture(texLogo);
	SDL_DestroyTexture(texBG);
	SDL_DestroyTexture(texBtn);
	SDL_DestroyTexture(texBtnC);

	decoWM->Delete();

	if (this->clickSfx)
		Mix_FreeChunk(clickSfx);

	Game::instance->stopMusic();

	Game::instance->actorMgr.createActorByID(3); // Map

	#ifdef _DEBUG
	Game::instance->actorMgr.createActorByID(0); // Souris
	#endif

	return 1;
}

void A_Titlescreen::calculateButtonRects() {
	double maxH = (1.0 / 2);
	u32 nbButtons = (u32)buttons.size();

	int bw, bh;
	SDL_QueryTexture(texBtn, NULL, NULL, &bw, &bh);
	double rbh = (maxH / nbButtons) - (maxH / (nbButtons * 4));
	double rbw = bw * (rbh / bh);

	double currY = 5 * (1.1 / 12);

	for (u32 i = 0; i < nbButtons; i++) {
		TSButton* button = &buttons[i];

		SDL_Surface* txt = WriteDebugText(button->text.c_str(), true);
		button->txtT = SDL_CreateTextureFromSurface(Game::instance->renderer, txt);
		SDL_FreeSurface(txt);

		//SDL_Rect dstRect = { (1.0 / 2) - (bw / 2), currY, bw, bh };

		button->x = (1.0 / 2) - (rbw / 2);
		button->y = currY;
		button->w = rbw;
		button->h = rbh;

		currY += maxH / nbButtons;
	}
}

int A_Titlescreen::onExecute() {
	acState.execute();

	if (deleted)
		return 1;

	u32 nbButtons = (u32)buttons.size();

	for(u32 i = 0; i < nbButtons; i++) {
		TSButton* button = &buttons[i];

		SDL_Rect dstRect = { (int)SDL_round(button->x * Game::instance->wh + ((Game::instance->ww - Game::instance->wh) / 2.0)),
							(int)SDL_round(button->y * Game::instance->wh),
							(int)SDL_round(button->w * Game::instance->wh),
							(int)SDL_round(button->h * Game::instance->wh) };

		MouseHandle* mh = &Game::instance->inputHandler.mouseHandle;
		KeyHandle* leftKH = Game::instance->inputHandler.getKeyHandle(SDL_BUTTON_LEFT);

		if (leftKH->pressed || leftKH->released) {
			if (IsPtInRect(dstRect.x, dstRect.y, dstRect.x + dstRect.w, dstRect.y + dstRect.h, mh->x, mh->y)) {
				if (leftKH->pressed) {
					button->clicked = true;
				}
				else if (leftKH->released && button->clicked) {
					(this->*(button->callback))();
					button->clicked = false;
					Mix_PlayChannel(SOUNDCHANNEL_TITLESCREEN, this->clickSfx, 0);
				}
			}
			else
				button->clicked = false;
		}
	}

	return 1;
}

int A_Titlescreen::onDraw() {
	int lw, lh;
	SDL_QueryTexture(texLogo, NULL, NULL, &lw, &lh);
	/*
	int rlw = (int)((lw - 20) * (Game::instance->ww / 4800.0));
	int rlh = lh * ((float)rlw / (float)lw);
	lh = SDL_min(rlh, Game::instance->wh - 20);
	lw = rlw * ((float)lh / (float)rlh);
	*/
	double r = (5 * (1.1 / 12)) * Game::instance->wh;
	int rlh = (int)(r / 1.1);
	int rlw = (int)(lw * ((float)rlh/ (float)lh));
	lh = rlh;
	lw = rlw;

	/*
	double whereAmIinY = SDL_sin(Game::instance->getFrameCount() / 60.0);
	whereAmIinY *= ;
	whereAmIinY += ((5 * (1.1 / 12)) / 1.1);
	*/

	double whereAmIinY = (r - (r / 1.1)) / 2;

	SDL_Rect logoPos = { (Game::instance->ww / 2) - (lw / 2), (int)whereAmIinY, lw, lh };

	SDL_RenderCopy(Game::instance->renderer, texLogo, NULL, &logoPos);

	for (TSButton button : buttons) {
		SDL_Rect dstRect = { (int)SDL_round(button.x * Game::instance->wh + ((Game::instance->ww - Game::instance->wh) / 2.0)),
							(int)SDL_round(button.y * Game::instance->wh),
							(int)SDL_round(button.w * Game::instance->wh),
							(int)SDL_round(button.h * Game::instance->wh) };

		SDL_RenderCopy(Game::instance->renderer, button.clicked ? texBtnC : texBtn, NULL, &dstRect);

		int tw, th;
		SDL_QueryTexture(button.txtT, NULL, NULL, &tw, &th);

		SDL_Rect dstRect2 = { (int)(dstRect.x + (dstRect.w * 0.1086)),
								(int)(dstRect.y + (dstRect.h * 0.2653)),
								(int)(dstRect.w * 0.7826),
								(int)(dstRect.h * 0.4897) };

		double hscale = (double)dstRect2.w / (double)tw;
		double vscale = (double)dstRect2.h / (double)th;
		double scale = SDL_min(hscale, vscale);

		dstRect2.x += (int)((dstRect2.w - (tw * scale)) / 2);
		dstRect2.y += (int)((dstRect2.h - (th * scale)) / 2);
		dstRect2.w = (int)(tw * scale);
		dstRect2.h = (int)(th * scale);

		SDL_RenderCopy(Game::instance->renderer, button.txtT, NULL, &dstRect2);
	}

	if (this->fadingOpacity >= 0) {
		SDL_Color ogCol;
		SDL_GetRenderDrawColor(Game::instance->renderer, &ogCol.r, &ogCol.g, &ogCol.b, &ogCol.a);

		SDL_SetRenderDrawColor(Game::instance->renderer, 0, 0, 0, (u8)((float)((this->reverseFade) ? this->fadingOpacity : (this->fadeTime - this->fadingOpacity)) / this->fadeTime * 255.0f));
		SDL_RenderFillRect(Game::instance->renderer, NULL);

		SDL_SetRenderDrawColor(Game::instance->renderer, ogCol.r, ogCol.g, ogCol.b, ogCol.a);
	}


	return 1;
}

void A_Titlescreen::NewGame() {
	SaveMgr::instance->Reset();
	this->Continue();
}

void A_Titlescreen::Continue() {
	this->reverseFade = false;
	acState.setState(&StateID_Fade);
}

void A_Titlescreen::Exit() {
	Game::instance->quit();
}


void A_Titlescreen::beginState_Wait() {}

void A_Titlescreen::executeState_Wait() {}

void A_Titlescreen::endState_Wait() {}


void A_Titlescreen::beginState_Fade() {
	this->fadingOpacity = this->fadeTime;
}

void A_Titlescreen::executeState_Fade() {
	this->fadingOpacity--;

	if (this->fadingOpacity == 0) {
		if (!this->reverseFade) {
			this->Delete();
			deleted = true;
		}
		else
			acState.setState(&StateID_Wait);
	}
}

void A_Titlescreen::endState_Fade() {
}