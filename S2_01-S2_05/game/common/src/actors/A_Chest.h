#pragma once

#include <core/Game.h>
#include <core/Actors.h>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>

#include "A_Inter.h"
#include "A_InvMgr.h"

class A_Chest : public A_Inter {
public:
    static Actor* make();

    int onCreate();
    int onDelete();
    int onExecute();
    int onDraw();

    void Interaction();
    void Toggle();

    SDL_Texture* tex = NULL;
    Inventory* inventory = NULL;

    Mix_Chunk* openSfx = NULL;
    Mix_Chunk* closeSfx = NULL;

    bool opened = false;
};
