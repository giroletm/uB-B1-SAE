#include "A_Chest.h"
#include "A_Map.h"
#include "A_Joueur.h"


Actor* A_Chest::make() {
    return new A_Chest();
}

int A_Chest::onCreate() {
    this->drawOrder = 0;

    A_Map::instance->actorsInt.push_back(this);

    A_Map::instance->BindTilePos(&this->tilePos);

    switch (this->settings) {
        case 0:
            this->tex = IMG_LoadTexture(Game::instance->renderer, "assets/Misc/ChestWood.png");
            break;
        case 1:
            this->tex = IMG_LoadTexture(Game::instance->renderer, "assets/Misc/ChestRed.png");
            break;
        case 2:
            this->tex = IMG_LoadTexture(Game::instance->renderer, "assets/Misc/ChestBlue.png");
            break;
        default:
            break;
    }

    this->openSfx = Mix_LoadWAV("assets/Audio/ChestOpen.wav");
    this->closeSfx = Mix_LoadWAV("assets/Audio/ChestClose.wav");

    char* keyName;
    SDL_asprintf(&keyName, "chestInventory%d", this->settings);

    if (A_InvMgr::instance) {
        this->inventory = A_InvMgr::instance->CreateInventory(TextMgr::instance->GetValue("inventory.chest"), 9, 3, keyName);

        if (!SaveMgr::instance->KeyExists(keyName)) {
            switch (this->settings) {
                case 0:
                    this->inventory->SetItem(4, 1, 4);
                    break;

                case 1:
                    this->inventory->SetItem(4, 1, 2);
                    break;

                case 2:
                    for(int i = 0; i < (this->inventory->nbCols * this->inventory->nbRows); i++)
                        this->inventory->SetItem(i % this->inventory->nbCols, i / this->inventory->nbCols, 3);

                    break;

                default:
                    break;
            }
        }
    }

    SDL_free(keyName);

    return true;
}

int A_Chest::onDelete() {
    SDL_DestroyTexture(this->tex);

    if (this->openSfx)
        Mix_FreeChunk(this->openSfx);

    if (this->closeSfx)
        Mix_FreeChunk(this->closeSfx);

    return true;
}

int A_Chest::onExecute() {
    if (this->opened) {
        KeyHandle* escKH = Game::instance->inputHandler.getKeyHandle(SDLK_ESCAPE);
        if (escKH->pressed) {
            this->Toggle();
            A_Joueur::instance->isInterracting = false;
            Game::instance->inputHandler.consumeKey(SDLK_ESCAPE);
        }
    }

    return true;
}

int A_Chest::onDraw() {
    SDL_Rect dstrect = { (int)((this->tilePos.x * 32) - (32 / 2)), (int)((this->tilePos.y * 32) - (32 / 2) - 8), 32, 32 };
    SDL_Rect srcrect = { this->opened ? 48 : 0, 0, 48, 48 };

    Game::RenderCopy(this->tex, &srcrect, &dstrect);

    return true;
}

void A_Chest::Interaction() {
    this->Toggle();
}


void A_Chest::Toggle() {
    A_Joueur::instance->ToggleInventory(true);
    A_InvMgr::instance->ToggleInventory(inventory);
    this->opened = !this->opened;

    Mix_PlayChannel(SOUNDCHANNEL_CHEST, this->opened ? this->openSfx : this->closeSfx, 0);
}
