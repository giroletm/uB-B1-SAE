#include "A_GUI.h"
#include "A_Joueur.h"
#include <res/debugfont.h>

CREATE_STATE(A_GUI, Wait);
CREATE_STATE(A_GUI, AppearMenu);
CREATE_STATE(A_GUI, DisappearMenu);

A_GUI* A_GUI::instance = 0;

Actor* A_GUI::make() {
	instance = new A_GUI();
	return instance;
}

int A_GUI::onCreate() {
	this->drawOrder = 2;

	texBtn = IMG_LoadTexture(Game::instance->renderer, "assets/UI/ButtonMenu.png");
	SDL_QueryTexture(texBtn, NULL, NULL, &tBtnW, &tBtnH);

	texWin = IMG_LoadTexture(Game::instance->renderer, "assets/UI/PauseWindow.png");

	fnt = TTF_OpenFont("assets/Fonts/8bitOperatorPlus-Bold.ttf", 48);

	AddButton("menu.btn.inventory", &A_GUI::Inventory);
	AddButton("menu.btn.save", &A_GUI::Save);
	AddButton("menu.btn.quit", &A_GUI::Quit);

	texturePv = IMG_LoadTexture(Game::instance->renderer, "assets/UI/Barredevie.png");
	SDL_QueryTexture(texturePv, NULL, NULL, &tPvW, &tPvH);

	this->clickSfx = Mix_LoadWAV("assets/Audio/MenuSelect.wav");

	acState.setState(&StateID_Wait);

	return 1;
}

void A_GUI::AddButton(const char* text, void (A_GUI::* callback)(MNButton*)) {
	SDL_Surface* txt = TTF_RenderText_Blended(fnt, TextMgr::instance->GetValue(text), { 255, 255, 255, 255 });

	SDL_Texture* txtTex = SDL_CreateTextureFromSurface(Game::instance->renderer, txt);
	int tw, th;
	SDL_QueryTexture(txtTex, NULL, NULL, &tw, &th);

	SDL_FreeSurface(txt);

	buttons.push_back({ txtTex, tw, th, callback });
}

int A_GUI::onDelete() {
	SDL_DestroyTexture(texBtn);
	TTF_CloseFont(fnt);

	for (const MNButton &btn : buttons)
		SDL_DestroyTexture(btn.tex);

	if (this->clickSfx)
		Mix_FreeChunk(clickSfx);
	
	instance = 0;

	return 1;
}

void A_GUI::GetBtnRect(SDL_Rect* dstRect) {
	if (dstRect == NULL)
		return;

	int atBtnH = Game::instance->wh / 6;
	int atBtnW = (int)(tBtnW * ((double)atBtnH / (double)tBtnH));

	*dstRect = { Game::instance->ww - atBtnW - 10, 10, atBtnW, atBtnH };
}

int A_GUI::onExecute() {
	acState.execute();

	if (A_Joueur::instance->isInterracting) {
		if(isUnrolled)
			acState.setState(&StateID_DisappearMenu);

		return 1;
	}


	SDL_Rect btnPos;
	GetBtnRect(&btnPos);

	MouseHandle* mh = &Game::instance->inputHandler.mouseHandle;
	KeyHandle* leftKH = Game::instance->inputHandler.getKeyHandle(SDL_BUTTON_LEFT);

	released = 0;

	if (IsPtInRect(btnPos.x, btnPos.y, btnPos.x + btnPos.w, btnPos.y + btnPos.h, mh->x, mh->y)) {
		if (leftKH->pressed) {
			pressed |= 1;
		}
		if (leftKH->released && (pressed & 1)) {
			released |= 1;
			pressed &= ~1;
		}
	}

	if (released & 1) {
		if (isUnrolled)
			acState.setState(&StateID_DisappearMenu);
		else
			acState.setState(&StateID_AppearMenu);
	}


	SDL_Rect rndPos;

	GetBtnRect(&rndPos);

	if (this->isUnrolled) {
		int x = rndPos.x;
		int y = (rndPos.y) + (rndPos.h / 2);

		int maxH = INT_MIN;

		for (auto it = buttons.begin(); it != buttons.end(); ++it) {
			MNButton* btn = &(*it);

			int h = (int)(btn->th * ((rndPos.w * 0.8) / (double)btn->tw));

			if (h > maxH)
				maxH = h;
		}

		int i = 1;
		int by = y + (rndPos.h / 2);
		for (auto it = buttons.begin(); it != buttons.end(); ++it) {
			MNButton* btn = &(*it);

			int iW = (int)(rndPos.w * 0.8);
			//int iH = (int)(btn->th * ((double)iW / (double)btn->tw));

			int nx = x + (rndPos.w - iW) / 2;
			SDL_Rect iR = { nx, by, iW, maxH };

			if (IsPtInRect(iR.x, iR.y, iR.x + iR.w, iR.y + iR.h, mh->x, mh->y)) {
				if (leftKH->pressed) {
					pressed |= 1 << i;
				}
				if (leftKH->released && (pressed & (1 << i))) {
					(this->*(btn->callback))(btn);
					pressed &= ~(1 << i);
					Mix_PlayChannel(SOUNDCHANNEL_GUI, this->clickSfx, 0);
				}
			}

			by += maxH;
			++i;
		}
	}
	
	if (leftKH->released)
		pressed = 0;

	return 1;
}

int A_GUI::onDraw() {
	SDL_Rect rndPos;

	GetBtnRect(&rndPos);
	double scale = rndPos.w / tBtnW;

	if (this->animTimer >= animHalfTime) {
		int x = rndPos.x;
		int y = (rndPos.y) + (rndPos.h / 2);

		int maxWin = rndPos.h / 2;
		int maxH = INT_MIN;

		for (auto it = buttons.begin(); it != buttons.end(); ++it) {
			MNButton* btn = &(*it);

			int h = (int)(btn->th * ((rndPos.w * 0.8) / (double)btn->tw));

			if (h > maxH)
				maxH = h;
		}

		maxWin += maxH * (int)buttons.size();


		maxWin = (int)(maxWin * ((this->animTimer - animHalfTime) / (double)animHalfTime));

		SDL_Rect dWinL = { x, y, (int)(winL.w * scale), maxWin };
		SDL_Rect dWinR = { x + rndPos.w - (int)(winR.w * scale), y, (int)(winR.w * scale), maxWin };
		SDL_Rect dWinM = { dWinL.x + dWinL.w, y, rndPos.w - dWinL.w - dWinR.w, maxWin };
		SDL_RenderCopy(Game::instance->renderer, texWin, &winL, &dWinL);
		SDL_RenderCopy(Game::instance->renderer, texWin, &winR, &dWinR);
		SDL_RenderCopy(Game::instance->renderer, texWin, &winM, &dWinM);

		SDL_Rect dWinBL = { dWinL.x, y + maxWin, (int)(winBL.w * scale), (int)(winBL.h * scale) };
		SDL_Rect dWinBR = { dWinL.x + rndPos.w - (int)(winBR.w * scale), y + maxWin, (int)(winBR.w * scale), (int)(winBR.h * scale) };
		SDL_Rect dWinB = { dWinBL.x + dWinBL.w, y + maxWin, rndPos.w - dWinBL.w - dWinBR.w, (int)(winB.h * scale) };
		SDL_RenderCopy(Game::instance->renderer, texWin, &winBL, &dWinBL);
		SDL_RenderCopy(Game::instance->renderer, texWin, &winBR, &dWinBR);
		SDL_RenderCopy(Game::instance->renderer, texWin, &winB, &dWinB);

		int by = y + maxWin;
		for (auto it = buttons.rbegin(); it != buttons.rend(); ++it) {
			MNButton* btn = &(*it);

			int iW = (int)(rndPos.w * 0.8);
			int iH = (int)(btn->th * ((double)iW / (double)btn->tw));

			int nx = x + (rndPos.w - iW) / 2;
			by -= maxH;
			SDL_Rect iR = { nx, by + ((maxH - iH) / 2), iW, iH};

			if (iR.y > y)
				SDL_RenderCopy(Game::instance->renderer, btn->tex, NULL, &iR);
		}
	}
	else {
		double scale2 = ((animHalfTime - this->animTimer) / (double)animHalfTime) + 1.0;

		int ow = rndPos.w;
		int oh = rndPos.h;

		rndPos.w = (int)(rndPos.w / scale2);
		rndPos.h = (int)(rndPos.h / scale2);
		rndPos.x += (ow - rndPos.w) / 2;
		rndPos.y += (oh - rndPos.h) / 2;
	}

	SDL_RenderCopy(Game::instance->renderer, texBtn, NULL, &rndPos);

	SDL_Rect pvPos = {
		rndPos.x - 25 - (int)(tPvW * 4 * (Game::instance->wh / 600.0)),
		45,
		(int)(tPvW * 4 * (Game::instance->wh / 600.0)),
		(int)(tPvH * 4 * (Game::instance->wh / 600.0))
	};

	SDL_RenderCopy(Game::instance->renderer, texturePv, NULL, &pvPos);

	int pv = A_Joueur::instance->pv;

	int px = pvPos.w / 30;

	SDL_Rect pvPosvie = {
		pvPos.x + px,
		pvPos.y + px,
		(pvPos.w - (2 * px)) * pv / PLAYER_MAX_HP,
		pvPos.h - (2 * px)
	};

	SDL_Color col;
	SDL_GetRenderDrawColor(Game::instance->renderer, &col.r, &col.g, &col.b, &col.a);
	SDL_SetRenderDrawColor(Game::instance->renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(Game::instance->renderer, &pvPosvie);
	SDL_SetRenderDrawColor(Game::instance->renderer, col.r, col.g, col.b, col.a);

	return 1;
}

void A_GUI::Inventory(MNButton* btn) {
	A_Joueur::instance->ToggleInventory();
}

void A_GUI::Save(MNButton* btn) {
	if(SaveMgr::instance->Save())
		SDL_SetTextureColorMod(btn->tex, 0, 255, 0);
	else
		SDL_SetTextureColorMod(btn->tex, 255, 0, 0);
}

void A_GUI::Quit(MNButton* btn) {
	Game::instance->quit();
}


void A_GUI::beginState_Wait() {}
void A_GUI::executeState_Wait() {}
void A_GUI::endState_Wait() {}


void A_GUI::beginState_AppearMenu()  {
	this->isUnrolled = true;

	if (this->animTimer <= animHalfTime)
		for (auto it = buttons.rbegin(); it != buttons.rend(); ++it)
			SDL_SetTextureColorMod((*it).tex, 255, 255, 255);

	Mix_PlayChannel(SOUNDCHANNEL_GUI, this->clickSfx, 0);
}

void A_GUI::executeState_AppearMenu()  {
	if (animTimer >= animTime) {
		acState.setState(&StateID_Wait);
		return;
	}

	animTimer++;
}

void A_GUI::endState_AppearMenu()  {

}


void A_GUI::beginState_DisappearMenu()  {
	this->isUnrolled = false;

	Mix_PlayChannel(SOUNDCHANNEL_GUI, this->clickSfx, 0);
}

void A_GUI::executeState_DisappearMenu()  {
	if (animTimer <= 0) {
		acState.setState(&StateID_Wait);
		return;
	}

	animTimer--;
}

void A_GUI::endState_DisappearMenu()  {

}
