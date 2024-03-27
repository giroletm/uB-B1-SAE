#include "A_TextBox.h"
#include "A_Joueur.h"
#include "A_Map.h"
#include "A_Camera.h"
#include "A_PNJ_1.h"
#include <res/debugfont.h>

CREATE_STATE(A_TextBox, Print);
CREATE_STATE(A_TextBox, Wait);

A_TextBox* A_TextBox::instance = NULL;

Actor* A_TextBox::make() {
	instance = new A_TextBox();
	return instance;
}

int A_TextBox::onCreate() {
	this->drawOrder = 0;

	texture_text = IMG_LoadTexture(Game::instance->renderer, "assets/UI/text.png");
	SDL_QueryTexture(texture_text, NULL, NULL, &texSize.x, &texSize.y);

	SDL_Surface* arr = WriteDebugText("->", false, 24);
	arrow = SDL_CreateTextureFromSurface(Game::instance->renderer, arr);
	arrowSize.x = arr->w;
	arrowSize.y = arr->h;
	SDL_FreeSurface(arr);

	talks.push_back(NULL);
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkText.wav"));
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkText2.wav"));
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkAlphys.wav"));
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkAsgore.wav"));
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkAsriel.wav"));
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkFlowey.wav"));
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkFloweyCreepy.wav"));
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkMTT.wav"));
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkPapyrus.wav"));
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkSans.wav"));
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkTem.wav"));
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkToriel.wav"));
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkUndyne.wav"));
	talks.push_back(Mix_LoadWAV("assets/Audio/TalkWindings.wav"));

	arrowSound = Mix_LoadWAV("assets/Audio/MenuMove.wav");
	selectSound = Mix_LoadWAV("assets/Audio/MenuSelect.wav");

	acState.setState(&StateID_Wait);

	return true;
}

int A_TextBox::onDelete() {
	SDL_DestroyTexture(texture_text);
	SDL_DestroyTexture(arrow);

	if (bufferedTextTex)
		SDL_DestroyTexture(bufferedTextTex);

	if (bufferedTitleTex)
		SDL_DestroyTexture(bufferedTitleTex);

	if (Mix_Playing(SOUNDCHANNEL_TEXTBOX))
		Mix_HaltChannel(SOUNDCHANNEL_TEXTBOX);

	for (Mix_Chunk* chunk : talks)
		if(chunk)
			Mix_FreeChunk(chunk);

	if(arrowSound)
		Mix_FreeChunk(arrowSound);

	if(selectSound)
		Mix_FreeChunk(selectSound);

	instance = NULL;

	return 1;
}

int A_TextBox::onExecute() {
	acState.execute();

	this->sizes[0].x = (int)((double)Game::instance->ww / 1.9);	// h
	this->sizes[0].y = (int)((double)Game::instance->ww / 7.0);	// w
	this->sizes[1].x = (int)(((double)40 * (double)Game::instance->ww) / 300.0);		// h
	this->sizes[1].y = (int)(((double)30 * (double)Game::instance->ww) / 300.0);		// w

	this->posText[0].x = (float)((Game::instance->ww / 2) - (this->sizes[0].x / 2));
	this->posText[0].y = (float)(Game::instance->wh - this->sizes[0].y - 20);
	this->posText[1].x = this->posText[0].x + this->sizes[0].x;
	this->posText[1].y = this->posText[0].y - this->sizes[1].y;

	if (conv != NULL) {
		if(conv->nbChoices > 0) {
			int nDir = Game::instance->inputHandler.getPressedDirection();

			switch (nDir) {
				case DIRECTION_DOWN:
					if (selChoice < (conv->nbChoices - 1)) {
						Mix_PlayChannel(SOUNDCHANNEL_TEXTBOX_SECONDARY, arrowSound, 0);
						selChoice++;
					}
					break;
				case DIRECTION_UP:
					if (selChoice > 0) {
						Mix_PlayChannel(SOUNDCHANNEL_TEXTBOX_SECONDARY, arrowSound, 0);
						selChoice--;
					}
					break;
				default:
					break;
			}
		}

		if(isSoundPlaying && !isActuallyPlaying) {
			if (conv->soundID != 0)
				Mix_PlayChannel(SOUNDCHANNEL_TEXTBOX, talks[conv->soundID], 0);

			isActuallyPlaying = true;
		}
	}

	return 1;
}

void A_TextBox::ChannelFinished() {
	if (conv != NULL && isSoundPlaying && (conv->soundID != 0)) {
		Mix_PlayChannel(SOUNDCHANNEL_TEXTBOX, talks[conv->soundID], 0);
		isActuallyPlaying = true;
	}
	else
		isActuallyPlaying = false;
}

void A_TextBox::RenderTextBox(SDL_Rect* rectText) {
	int xcase = texSize.x / 3;
	int ycase = texSize.y / 3;
	SDL_Rect srcRectTL = { 0,         0,         xcase, ycase };
	SDL_Rect srcRectT = { xcase,      0,         xcase, ycase };
	SDL_Rect srcRectTR = { 2 * xcase, 0,         xcase, ycase };
	SDL_Rect srcRectL = { 0,          ycase,     xcase, ycase };
	SDL_Rect srcRectM = { xcase,      ycase,     xcase, ycase };
	SDL_Rect srcRectR = { 2 * xcase,  ycase,     xcase, ycase };
	SDL_Rect srcRectBL = { 0,         2 * ycase, xcase, ycase };
	SDL_Rect srcRectB = { xcase,      2 * ycase, xcase, ycase };
	SDL_Rect srcRectBR = { 2 * xcase, 2 * ycase, xcase, ycase };

	double sizeRatio = (double)Game::instance->ww / 300.0;
	SDL_Rect dstRectTL = {
		rectText->x,
		rectText->y,
		(int)(8 * sizeRatio),
		(int)(8 * sizeRatio)
	};
	SDL_Rect dstRectTR = {
		rectText->x + rectText->w - (int)(8 * sizeRatio),
		rectText->y,
		(int)(8 * sizeRatio),
		(int)(8 * sizeRatio)
	};
	SDL_Rect dstRectT = {
		rectText->x + dstRectTL.w,
		rectText->y,
		rectText->w - (dstRectTL.w + dstRectTR.w),
		(int)(8 * sizeRatio)
	};
	SDL_Rect dstRectBL = {
		rectText->x,
		rectText->y + rectText->h - (int)(8 * sizeRatio),
		(int)(8 * sizeRatio),
		(int)(8 * sizeRatio)
	};
	SDL_Rect dstRectBR = {
		rectText->x + rectText->w - (int)(8 * sizeRatio),
		rectText->y + rectText->h - (int)(8 * sizeRatio),
		(int)(8 * sizeRatio),
		(int)(8 * sizeRatio)
	};
	SDL_Rect dstRectB = {
		rectText->x + dstRectBL.w,
		rectText->y + rectText->h - (int)(8 * sizeRatio),
		rectText->w - (dstRectBL.w + dstRectBR.w),
		(int)(8 * sizeRatio)
	};
	SDL_Rect dstRectL = {
		rectText->x,
		rectText->y + dstRectTL.h,
		(int)(8 * sizeRatio),
		rectText->h - (dstRectTL.h + dstRectTR.h)
	};
	SDL_Rect dstRectR = {
		rectText->x + rectText->w - (int)(8 * sizeRatio),
		rectText->y + dstRectTR.h,
		(int)(8 * sizeRatio),
		rectText->h - (dstRectBL.h + dstRectBR.h)
	};
	SDL_Rect dstRectM = {
		rectText->x + dstRectL.w,
		rectText->y + dstRectT.h,
		rectText->w - (dstRectL.w + dstRectR.w),
		rectText->h - (dstRectT.h + dstRectB.h)
	};

	SDL_RenderCopy(Game::instance->renderer, texture_text, &srcRectTL, &dstRectTL);
	SDL_RenderCopy(Game::instance->renderer, texture_text, &srcRectT, &dstRectT);
	SDL_RenderCopy(Game::instance->renderer, texture_text, &srcRectTR, &dstRectTR);
	SDL_RenderCopy(Game::instance->renderer, texture_text, &srcRectBL, &dstRectBL);
	SDL_RenderCopy(Game::instance->renderer, texture_text, &srcRectB, &dstRectB);
	SDL_RenderCopy(Game::instance->renderer, texture_text, &srcRectBR, &dstRectBR);
	SDL_RenderCopy(Game::instance->renderer, texture_text, &srcRectL, &dstRectL);
	SDL_RenderCopy(Game::instance->renderer, texture_text, &srcRectM, &dstRectM);
	SDL_RenderCopy(Game::instance->renderer, texture_text, &srcRectR, &dstRectR);
}

int A_TextBox::onDraw() {
	if (afficher == true && conv != NULL) {
		SDL_Rect rectText = { (int)this->posText[0].x, (int)this->posText[0].y, sizes[0].x, sizes[0].y };
		RenderTextBox(&rectText);

		double winRatio = (double)Game::instance->ww / 1200.0;

		if (conv->text != NULL) {
			std::string textSubstr = std::string(conv->text);
			textSubstr = textSubstr.substr(0, numChar);

			if (textSubstr.size() > 0) {
				if (SDL_strcmp(textSubstr.c_str(), bufferedText.c_str()) != 0) {
					SDL_Surface* txt = WriteDebugText(textSubstr.c_str(), false, 24, conv->fontStyle);
					txtW = txt->w;
					txtH = txt->h;

					if (bufferedTextTex)
						SDL_DestroyTexture(bufferedTextTex);

					bufferedTextTex = SDL_CreateTextureFromSurface(Game::instance->renderer, txt);
					SDL_FreeSurface(txt);

					bufferedText = textSubstr;
				}

				SDL_Rect dsttext = {
					rectText.x + (int)(28.0 * winRatio),
					rectText.y + (int)(28.0 * winRatio),
					(int)((double)txtW * winRatio),
					(int)((double)txtH * winRatio)
				};

				SDL_RenderCopy(Game::instance->renderer, bufferedTextTex, NULL, &dsttext);
			}
		}

		if (conv->title != NULL) {
			if (conv->title != bufferedTitle) {
				SDL_Surface* ttl = WriteDebugText(conv->title, false, 20, conv->fontStyle);
				ttlW = ttl->w;
				ttlH = ttl->h;
				
				if (bufferedTitleTex)
					SDL_DestroyTexture(bufferedTitleTex);

				bufferedTitleTex = SDL_CreateTextureFromSurface(Game::instance->renderer, ttl);
				SDL_FreeSurface(ttl);

				bufferedTitle = conv->title;
			}

			rectText = {
				(int)this->posText[0].x,
				(int)(this->posText[0].y - ((ttlH + 40.0 - 10.0) * winRatio)),
				(int)((ttlW + 40.0) * winRatio),
				(int)((ttlH + 40.0) * winRatio)
			};

			RenderTextBox(&rectText);


			SDL_Rect dsttitle = {
				rectText.x + (int)(20.0 * winRatio),
				rectText.y + (int)(20.0 * winRatio),
				(int)((double)ttlW * winRatio),
				(int)((double)ttlH * winRatio)
			};


			SDL_RenderCopy(Game::instance->renderer, bufferedTitleTex, NULL, &dsttitle);
		}

		if (showChoices) {
			SDL_Surface** choiceSurfs = new SDL_Surface*[conv->nbChoices];

			int maxChoiceW = 0;
			int totalChoiceH = 0;
			for (int i = 0; i < conv->nbChoices; i++) {
				choiceSurfs[i] = WriteDebugText(conv->choices[i], false, 24);

				if (choiceSurfs[i]->w > maxChoiceW)
					maxChoiceW = choiceSurfs[i]->w;

				totalChoiceH += choiceSurfs[i]->h;
			}

			int rtW = (int)((maxChoiceW + arrowSize.x + 50.0) * winRatio);
			int rtH = (int)((totalChoiceH + 50.0) * winRatio);

			rectText = {
				(int)(this->posText[0].x + this->sizes[0].x),
				(int)(this->posText[0].y - rtH),
				rtW,
				rtH
			};

			RenderTextBox(&rectText);

			double y = 24.0;
			for (int i = 0; i < conv->nbChoices; i++) {
				SDL_Surface* txt = choiceSurfs[i];

				SDL_Rect choiceRect = {
					rectText.x + (int)(60.0 * winRatio),
					rectText.y + (int)(y * winRatio),
					(int)((double)txt->w * winRatio),
					(int)((double)txt->h * winRatio)
				};

				y += txt->h;

				SDL_Texture* text = SDL_CreateTextureFromSurface(Game::instance->renderer, txt);

				SDL_RenderCopy(Game::instance->renderer, text, NULL, &choiceRect);
				SDL_FreeSurface(txt);

				SDL_DestroyTexture(text);

				if (i == selChoice) {
					SDL_Rect arrowRect = {
						rectText.x + (int)(20.0 * winRatio),
						choiceRect.y,
						(int)((double)arrowSize.x * winRatio),
						(int)((double)arrowSize.y * winRatio)
					};

					SDL_RenderCopy(Game::instance->renderer, arrow, NULL, &arrowRect);
				}
			}

			delete[] choiceSurfs;
		}
	}

	return 1;
}


void A_TextBox::affichertext(PNJConvo* conv) {
	if (conv == NULL)
		return;

	this->conv = conv;
	acState.setState(&StateID_Print);
}


/// <summary>
/// Etat Imprimer
/// </summary>

void A_TextBox::beginState_Print() {
	afficher = true;

	numChar = 0;
	waitForChar = 0;

	showChoices = false;
	selChoice = 0;

	A_Map::instance->TriggerEvent(conv->numTriggerEvent);
	SDL_SetTextureColorMod(texture_text, conv->color.r, conv->color.g, conv->color.b);
}

void A_TextBox::executeState_Print() {
	bool inter = (acState.stateFrameCount > 0) ? Game::instance->inputHandler.getPressedInteraction() : false;
	u32 txtLen = (u32)SDL_strlen(conv->text);
	
	if (numChar < txtLen) {
		if (waitForChar <= 0) {
			char chr = conv->text[numChar];
			switch (chr) {
				case '.':
				case '?':
				case '!':
				case ';':
					waitForChar = 20;
					break;

				case ',':
					waitForChar = 10;
					break;

				default:
					break;
			}

			if((chr & 0b10000000) == 0)
				numChar++;
			else if ((chr & 0b11100000) == 0b11000000)
				numChar += 2;
			else if ((chr & 0b11110000) == 0b11100000)
				numChar += 3;
			else if ((chr & 0b11111000) == 0b11110000)
				numChar += 4;

			if(numChar >= txtLen)
				isSoundPlaying = false;
			else
				isSoundPlaying = true;
		}
		else {
			isSoundPlaying = false;
			waitForChar--;
		}

		if (inter) {
			numChar = txtLen;
			isSoundPlaying = false;
		}
	}
	else {
		if (conv->nbChoices > 0) {
			if(!showChoices)
				showChoices = true;

			if (inter) {
				Mix_PlayChannel(SOUNDCHANNEL_TEXTBOX_SECONDARY, selectSound, 0);

				conv = conv->choicesResult[selChoice];

				bool proceed = true;
				if (conv && conv->callback != NULL) {
					proceed = (*conv->callback)();
				}

				if (conv && proceed) {
					numChar = 0;
					showChoices = false;
					acState.setState(&StateID_Print);
				}
				else {
					conv = NULL;
					afficher = false;
					Game::instance->inputHandler.consumeInteraction();
					acState.setState(&StateID_Wait);
				}
			}
		}
		else if (inter) {
			if (conv->followingConvo) {
				conv = conv->followingConvo;

				bool proceed = true;
				if (conv->callback != NULL) {
					proceed = (*conv->callback)();
				}

				if (!proceed) {
					conv = NULL;
					afficher = false;
					Game::instance->inputHandler.consumeInteraction();
					acState.setState(&StateID_Wait);
				}
				else {
					numChar = 0;
					acState.setState(&StateID_Print);
				}
			}
			else {
				afficher = false;
				Game::instance->inputHandler.consumeInteraction();
				acState.setState(&StateID_Wait);
			}
		}
	}


}

void A_TextBox::endState_Print() {
}




/// <summary>
/// Etat Attente
/// </summary>

void A_TextBox::beginState_Wait() {
	conv = NULL;
	A_Joueur::instance->isInterracting = false;
}

void A_TextBox::executeState_Wait() {

}

void A_TextBox::endState_Wait() {
}
