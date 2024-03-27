#include "A_InvMgr.h"

#if defined(__ANDROID__) || defined(SIMULATE_ANDROID)
#include "A_AndroidCtrl.h"
#endif


A_InvMgr* A_InvMgr::instance = NULL;

Actor* A_InvMgr::make() {
	instance = new A_InvMgr();
	return instance;
}

int A_InvMgr::onCreate() {
	this->drawOrder = 2;

	tex = IMG_LoadTexture(Game::instance->renderer, "assets/UI/Inventory.png");
	texHover = IMG_LoadTexture(Game::instance->renderer, "assets/UI/InvHover.png");

	return true;
}

int A_InvMgr::onDelete() {
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(texHover);

	ClearInventories();

	instance = NULL;

	return true;
}

void A_InvMgr::ClearInventories() {
	for (Inventory* inv : inventories) {
		int totalItems = inv->nbCols * inv->nbRows;
		for (int i = 0; i < totalItems; i++) {
			delete inv->items[i];
		}

		delete[] inv->items;
		delete inv;
	}

	inventories.clear();
}

int A_InvMgr::onExecute() {
	int i = 0;

	bool isSingle = false;
	for (Inventory* inv : inventories) {
		if (inv->shown) {
			if (!isSingle)
				isSingle = true;
			else {
				isSingle = false;
				break;
			}
		}
	}

	double sizeRatio = Game::instance->wh / (isSingle ? 200.0 : 300.0);
	#if defined(__ANDROID__) || defined(SIMULATE_ANDROID)
	sizeRatio *= 1.2;
	#endif

	MouseHandle* mh = &Game::instance->inputHandler.mouseHandle;
	KeyHandle* leftKH = Game::instance->inputHandler.getKeyHandle(SDL_BUTTON_LEFT);

	for (Inventory* inv : inventories) {
		if (!inv->shown || i > 3)
			continue;

		int sizeW = (int)((6 + (18 * inv->nbCols)) * sizeRatio);
		int sizeH = (int)((17 + (18 * inv->nbRows)) * sizeRatio);

		int x, y;

		if (isSingle) {
			x = (Game::instance->ww / 2) - (sizeW / 2);
			y = (Game::instance->wh / 2) - (sizeH / 2);
		}
		else {
			x = ((i % 3) == 0) ? (Game::instance->ww - sizeW - 16) : 16;
			y = (i < 2) ? 16 : (Game::instance->wh - sizeH - 16);
		}

		#if defined(__ANDROID__) || defined(SIMULATE_ANDROID)
		if (!isSingle && (i < 2)) {
			SDL_Rect tmpRect;
			A_AndroidCtrl::instance->GetPauseRect(&tmpRect);
			y += tmpRect.y + tmpRect.h;
		}
		#endif

		int caseStartX = (int)(x + (3 * sizeRatio));
		int caseStartY = (int)(y + (14 * sizeRatio));
		int caseSize = (int)(18 * sizeRatio);
		
		int hoveredCaseX = (mh->x - caseStartX) / caseSize;
		int hoveredCaseY = (mh->y - caseStartY) / caseSize;

		if (hoveredCaseX >= 0 && hoveredCaseY >= 0 && hoveredCaseX < inv->nbCols && hoveredCaseY < inv->nbRows) {
			int hoveredCaseIdx = (hoveredCaseY * inv->nbCols) + hoveredCaseX;
			if (leftKH->pressed && clickedItem == NULL) {
				if (inv->items[hoveredCaseIdx]) {
					clickedItem = &inv->items[hoveredCaseIdx];
					clickedInv = inv;
				}
			}
			else if (leftKH->released && clickedItem != NULL) {
				InventoryItem* prevItem = inv->items[hoveredCaseIdx];
				inv->items[hoveredCaseIdx] = *clickedItem;
				*clickedItem = prevItem;

				inv->Save();
				clickedInv->Save();

				clickedItem = NULL;
				clickedInv = NULL;
			}
		}

		i++;
	}

	if(leftKH->released)
		clickedItem = NULL;

	return true;
}

int A_InvMgr::onDraw() {
	int i = 0;

	bool isSingle = false;
	for (Inventory* inv : inventories) {
		if (inv->shown) {
			if (!isSingle)
				isSingle = true;
			else {
				isSingle = false;
				break;
			}
		}
	}

	double sizeRatio = Game::instance->wh / (isSingle ? 200.0 : 300.0);
	#if defined(__ANDROID__) || defined(SIMULATE_ANDROID)
	sizeRatio *= 1.2;
	#endif

	MouseHandle* mh = &Game::instance->inputHandler.mouseHandle;

	for (Inventory* inv : inventories) {
		if (!inv->shown || i > 3)
			continue;

		int sizeW = (int)((6 + (18 * inv->nbCols)) * sizeRatio);
		int sizeH = (int)((17 + (18 * inv->nbRows)) * sizeRatio);

		int x, y;

		if (isSingle) {
			x = (Game::instance->ww / 2) - (sizeW / 2);
			y = (Game::instance->wh / 2) - (sizeH / 2);
		}
		else {
			x = ((i % 3) == 0) ? (Game::instance->ww - sizeW - 16) : 16;
			y = (i < 2) ? 16 : (Game::instance->wh - sizeH - 16);
		}

		#if defined(__ANDROID__) || defined(SIMULATE_ANDROID)
		if (!isSingle && (i < 2)) {
			SDL_Rect tmpRect;
			A_AndroidCtrl::instance->GetPauseRect(&tmpRect);
			y += tmpRect.y + tmpRect.h;
		}
		#endif

		SDL_Rect invBG = {
			x,
			y,
			sizeW,
			sizeH
		};
		SDL_Rect bg = { 0, 0, 18, 1 };
		SDL_RenderCopy(Game::instance->renderer, tex, &bg, &invBG);

		SDL_Rect dstTxt = {
			(int)(x + (4 * sizeRatio)),
			(int)(y + (2 * sizeRatio)),
			(int)((inv->nameW * (9.0 / inv->nameH)) * sizeRatio),
			(int)(9 * sizeRatio),
		};
		SDL_RenderCopy(Game::instance->renderer, inv->name, NULL, &dstTxt);

		for (int row = 0; row < inv->nbRows; row++) {
			for (int col = 0; col < inv->nbCols; col++) {
				SDL_Rect casePos = {
					(int)(x + ((3 + (col * 18)) * sizeRatio)),
					(int)(y + ((14 + (row * 18)) * sizeRatio)),
					(int)(18 * sizeRatio),
					(int)(18 * sizeRatio)
				};

				SDL_Rect srcCase = {
					0,
					1,
					18,
					18
				};

				SDL_RenderCopy(Game::instance->renderer, tex, &srcCase, &casePos);
			}
		}

		for (int row = 0; row < inv->nbRows; row++) {
			for (int col = 0; col < inv->nbCols; col++) {
				SDL_Rect casePos = {
					(int)(x + ((3 + (col * 18)) * sizeRatio)),
					(int)(y + ((14 + (row * 18)) * sizeRatio)),
					(int)(18 * sizeRatio),
					(int)(18 * sizeRatio)
				};

				int i = (row * inv->nbCols) + col;
				if (inv->items[i] && inv->items[i]->tex)
				{
					if(clickedItem == NULL || inv->items[i] != *clickedItem)
						SDL_RenderCopy(Game::instance->renderer, inv->items[i]->tex, NULL, &casePos);
				}
			}
		}

		if(clickedItem == NULL) {
			int caseStartX = (int)(x + (3 * sizeRatio));
			int caseStartY = (int)(y + (14 * sizeRatio));
			int caseSize = (int)(18 * sizeRatio);

			int hoveredCaseX = (mh->x - caseStartX) / caseSize;
			int hoveredCaseY = (mh->y - caseStartY) / caseSize;

			if (hoveredCaseX >= 0 && hoveredCaseY >= 0 && hoveredCaseX < inv->nbCols && hoveredCaseY < inv->nbRows) {
				int hoveredCaseIdx = (hoveredCaseY * inv->nbCols) + hoveredCaseX;
				InventoryItem* hoveredItem = inv->items[hoveredCaseIdx];

				if (hoveredItem != NULL) {
					SDL_Texture* itemNameTex = hoveredItem->GetNameTex();
					SDL_Point itemNameTexSize = hoveredItem->GetNameTexSize();
					SDL_Texture* itemDescTex = hoveredItem->GetDescTex();
					SDL_Point itemDescTexSize = hoveredItem->GetDescTexSize();

					itemNameTexSize.x = (int)(itemNameTexSize.x * (sizeRatio / 2.0));
					itemNameTexSize.y = (int)(itemNameTexSize.y * (sizeRatio / 2.0));

					itemDescTexSize.x = (int)(itemDescTexSize.x * (sizeRatio / 2.0));
					itemDescTexSize.y = (int)(itemDescTexSize.y * (sizeRatio / 2.0));

					SDL_Rect behindRect = {
						mh->x,
						mh->y,
						SDL_max(itemNameTexSize.x, itemDescTexSize.x) + (int)(8.0 * sizeRatio),
						itemNameTexSize.y + itemDescTexSize.y + (int)(6.0 * sizeRatio)
					};

					SDL_Rect tlHover = { 0,0, 3, 3 };
					SDL_Rect tHover = { 3, 0, 1, 3 };
					SDL_Rect trHover = { 4, 0, 3, 3 };
					SDL_Rect lHover = { 0, 3, 3, 1 };
					SDL_Rect mHover = { 3, 3, 1, 1 };
					SDL_Rect rHover = { 4, 3, 3, 1 };
					SDL_Rect blHover = { 0, 4, 3, 3 };
					SDL_Rect bHover = { 3, 4, 1, 3 };
					SDL_Rect brHover = { 4, 4, 3, 3 };

					SDL_Rect dstTL = {
						behindRect.x,
						behindRect.y,
						(int)(3 * sizeRatio),
						(int)(3 * sizeRatio)
					};
					SDL_Rect dstTR = {
						behindRect.x + behindRect.w - (int)(3 * sizeRatio),
						behindRect.y,
						(int)(3 * sizeRatio),
						(int)(3 * sizeRatio)
					};
					SDL_Rect dstT = {
						behindRect.x + dstTL.w,
						behindRect.y,
						behindRect.w - dstTL.w - dstTR.w,
						(int)(3 * sizeRatio)
					};

					SDL_Rect dstBL = {
						behindRect.x,
						behindRect.y + behindRect.h - (int)(3 * sizeRatio),
						(int)(3 * sizeRatio),
						(int)(3 * sizeRatio)
					};
					SDL_Rect dstBR = {
						behindRect.x + behindRect.w - (int)(3 * sizeRatio),
						behindRect.y + behindRect.h - (int)(3 * sizeRatio),
						(int)(3 * sizeRatio),
						(int)(3 * sizeRatio)
					};
					SDL_Rect dstB = {
						behindRect.x + dstBL.w,
						behindRect.y + behindRect.h - (int)(3 * sizeRatio),
						behindRect.w - dstBL.w - dstBR.w,
						(int)(3 * sizeRatio)
					};

					SDL_Rect dstL = {
						behindRect.x,
						behindRect.y + dstTL.h,
						(int)(3 * sizeRatio),
						behindRect.h - dstTL.h - dstBL.h
					};

					SDL_Rect dstR = {
						behindRect.x + behindRect.w - (int)(3 * sizeRatio),
						behindRect.y + dstTR.h,
						(int)(3 * sizeRatio),
						behindRect.h - dstTR.h - dstBR.h
					};

					SDL_Rect dstM = {
						dstL.x + dstL.w,
						dstT.y + dstT.h,
						dstT.w,
						dstL.h
					};

					SDL_RenderCopy(Game::instance->renderer, texHover, &tlHover, &dstTL);
					SDL_RenderCopy(Game::instance->renderer, texHover, &trHover, &dstTR);
					SDL_RenderCopy(Game::instance->renderer, texHover, &tHover, &dstT);
					SDL_RenderCopy(Game::instance->renderer, texHover, &blHover, &dstBL);
					SDL_RenderCopy(Game::instance->renderer, texHover, &brHover, &dstBR);
					SDL_RenderCopy(Game::instance->renderer, texHover, &bHover, &dstB);
					SDL_RenderCopy(Game::instance->renderer, texHover, &lHover, &dstL);
					SDL_RenderCopy(Game::instance->renderer, texHover, &rHover, &dstR);
					SDL_RenderCopy(Game::instance->renderer, texHover, &mHover, &dstM);

					SDL_Rect namePos = {
						(int)(mh->x + (4.0 * sizeRatio)),
						(int)(mh->y + (2.5 * sizeRatio)),
						itemNameTexSize.x,
						itemNameTexSize.y
					};

					SDL_Rect descPos = {
						namePos.x,
						namePos.y + namePos.h,
						itemDescTexSize.x,
						itemDescTexSize.y
					};

					SDL_RenderCopy(Game::instance->renderer, itemNameTex, NULL, &namePos);
					SDL_RenderCopy(Game::instance->renderer, itemDescTex, NULL, &descPos);
				}
			}
		}

		i++;
	}

	if (clickedItem) {
		MouseHandle* mh = &Game::instance->inputHandler.mouseHandle;
		SDL_Rect mousePos = {
			(int)(mh->x - ((18 * sizeRatio) / 2)),
			(int)(mh->y - ((18 * sizeRatio) / 2)),
			(int)((18 * sizeRatio)),
			(int)((18 * sizeRatio))
		};
		SDL_RenderCopy(Game::instance->renderer, (*clickedItem)->tex, NULL, &mousePos);
	}

	return true;
}

Inventory* A_InvMgr::CreateInventory(const char* name, int nbCols, int nbRows, const char* saveKey) {
	int totalItems = nbCols * nbRows;
	InventoryItem** items = new InventoryItem*[totalItems];
	for (int i = 0; i < totalItems; i++)
		items[i] = NULL;

	Inventory* inv = new Inventory(name, nbCols, nbRows, items, saveKey);

	inventories.push_back(inv);

	return inv;
}

void A_InvMgr::ToggleInventory(Inventory* inv) {
	inv->shown = !inv->shown;
}


Inventory::Inventory(const char* name, int nbCols, int nbRows, InventoryItem** items, const char* saveKey) {
	this->nbCols = nbCols;
	this->nbRows = nbRows;
	this->items = items;

	TTF_Font* fnt = TTF_OpenFont("assets/Fonts/8bitOperatorPlus-Bold.ttf", 12);
	SDL_Surface* txt = TTF_RenderText_Blended(fnt, name, { 65, 65, 65, 255 });
	this->name = SDL_CreateTextureFromSurface(Game::instance->renderer, txt);
	this->nameW = txt->w;
	this->nameH = txt->h;
	SDL_FreeSurface(txt);
	TTF_CloseFont(fnt);

	this->shown = false;

	this->saveKey = NULL;
	if (saveKey) {
		u32 saveKeySize = (u32)SDL_strlen(saveKey)+1;
		this->saveKey = new char[saveKeySize];
		for (u32 i = 0; i < saveKeySize; i++)
			((char*)this->saveKey)[i] = saveKey[i];

		if (SaveMgr::instance->KeyExists(this->saveKey)) {
			char* items = SaveMgr::instance->GetValue<char>(this->saveKey);
			int valueSize = SaveMgr::instance->GetValueSize(this->saveKey);

			for (int i = 0; i < valueSize; i++) {
				int x = (i % this->nbCols);
				int y = (i / this->nbCols);

				// Android ne convertit pas les char correctement, donc il faut le faire à la main
				int actualValue = ((u8)items[i] > 0x80) ? (int)(0xFFFFFF00 | (u32)items[i]) : items[i];

				this->SetItem(x, y, actualValue, true);
			}
		}
	}
}

Inventory::~Inventory() {
	SDL_DestroyTexture(this->name);

	if (this->saveKey)
		delete[] this->saveKey;
}

InventoryItem* Inventory::GetItem(int x, int y) {
	int i = (y * this->nbCols) + x;
	return this->items[i];
}

InventoryItem* Inventory::SetItem(int x, int y, int itemID, bool noSave) {
	int i = (y * this->nbCols) + x;

	if (itemID < 0) {
		if (this->items[i])
			delete this->items[i];

		if (!noSave)
			Save();

		return this->items[i] = NULL;
	}
	else {
		if (this->items[i] != NULL)
			delete this->items[i];
		
		this->items[i] = new InventoryItem(itemID);

		if (!noSave)
			Save();

		return this->items[i];
	}

}

InventoryItem* Inventory::AddItem(int itemID, bool noSave) {
	int totalItems = nbCols * nbRows;
	for (int i = 0; i < totalItems; i++) {
		if (this->items[i] == NULL) {
			this->items[i] = new InventoryItem(itemID);

			if (!noSave)
				Save();

			return this->items[i];
		}
	}

	return NULL;
}

void Inventory::Save() {
	if (!this->saveKey)
		return;

	int totSize = this->nbCols * this->nbRows;
	char* invData = new char[totSize];

	for (int i = 0; i < totSize; i++) {
		invData[i] = (this->items[i]) ? this->items[i]->id : -1;
	}

	SaveMgr::instance->AddValue(this->saveKey, invData, totSize);

	delete[] invData;
}

InventoryItem::InventoryItem(int itemID) {
	this->id = itemID;

	switch (this->id) {
		case 0:
			this->tex = IMG_LoadTexture(Game::instance->renderer, "assets/Items/pq.png");
			break;
		case 1:
			this->tex = IMG_LoadTexture(Game::instance->renderer, "assets/Items/brokenwine.png");
			break;
		case 2:
			this->tex = IMG_LoadTexture(Game::instance->renderer, "assets/Items/DS.png");
			break;
		case 3:
			this->tex = IMG_LoadTexture(Game::instance->renderer, "assets/Items/chainsaw.png");
			break;
		case 4:
			this->tex = IMG_LoadTexture(Game::instance->renderer, "assets/Items/coffee.png");
			break;
		default:
			this->tex = NULL;
			break;
	}
}

InventoryItem::~InventoryItem() {
	if (this->tex)
		SDL_DestroyTexture(this->tex);

	if (this->nameTex)
		SDL_DestroyTexture(this->nameTex);

	if (this->descTex)
		SDL_DestroyTexture(this->descTex);
}

const char* InventoryItem::GetName() {
	char* itemName;
	SDL_asprintf(&itemName, "item.%d.name", this->id);

	const char* actIName = TextMgr::instance->GetValue(itemName);

	SDL_free(itemName);

	return actIName;
}

SDL_Texture* InventoryItem::GetNameTex() {
	if (this->nameTex)
		return this->nameTex;
	
	const char* name = this->GetName();

	TTF_Font* fnt = TTF_OpenFont("assets/Fonts/8bitOperatorPlus-Regular.ttf", 12);
	SDL_Surface* txt = TTF_RenderUTF8_Solid(fnt, name, { 255, 255, 255, 255 });
	this->nameTex = SDL_CreateTextureFromSurface(Game::instance->renderer, txt);
	this->nameTexSize.x = txt->w;
	this->nameTexSize.y = txt->h;
	SDL_FreeSurface(txt);
	TTF_CloseFont(fnt);

	return this->nameTex;
}

const char* InventoryItem::GetDesc() {
	char* itemDesc;
	SDL_asprintf(&itemDesc, "item.%d.desc", this->id);

	const char* actIDesc = TextMgr::instance->GetValue(itemDesc);

	SDL_free(itemDesc);

	return actIDesc;
}

SDL_Texture* InventoryItem::GetDescTex() {
	if (this->descTex)
		return this->descTex;

	const char* name = this->GetDesc();

	TTF_Font* fnt = TTF_OpenFont("assets/Fonts/8bitOperatorPlus-Regular.ttf", 12);
	TTF_SetFontStyle(fnt, TTF_STYLE_ITALIC);
	SDL_Surface* txt = TTF_RenderUTF8_Solid(fnt, name, { 170, 170, 170, 255 });
	this->descTex = SDL_CreateTextureFromSurface(Game::instance->renderer, txt);
	this->descTexSize.x = txt->w;
	this->descTexSize.y = txt->h;
	SDL_FreeSurface(txt);
	TTF_CloseFont(fnt);

	return this->descTex;
}