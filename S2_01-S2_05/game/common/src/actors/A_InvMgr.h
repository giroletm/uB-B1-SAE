#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <common.h>

#include <core/Game.h>
#include <core/Actors.h>

class InventoryItem {
public:
    InventoryItem(int itemID);
    ~InventoryItem();

    int id;
    SDL_Texture* tex;

    const char* GetName();
    const char* GetDesc();
    SDL_Texture* GetNameTex();
    SDL_Point GetNameTexSize() { return nameTexSize; }
    SDL_Texture* GetDescTex();
    SDL_Point GetDescTexSize() { return descTexSize; }

private:
    SDL_Texture* nameTex = NULL;
    SDL_Point nameTexSize = { -1, -1 };

    SDL_Texture* descTex = NULL;
    SDL_Point descTexSize = { -1, -1 };
};

class Inventory {
public:
    Inventory(const char* name, int nbCols, int nbRows, InventoryItem** items, const char* saveKey);
    ~Inventory();

    int nbRows, nbCols;
    InventoryItem** items;

    SDL_Texture* name;
    int nameW, nameH;

    bool shown;
    const char* saveKey;

    InventoryItem* GetItem(int x, int y);
    InventoryItem* SetItem(int x, int y, int itemID, bool noSave=false);
    InventoryItem* AddItem(int itemID, bool noSave=false);
    void Save();
};

class A_InvMgr : public Actor {
public:
    int onCreate();
    int onDelete();
    int onExecute();
    int onDraw();

    SDL_Texture* tex = NULL;
    SDL_Texture* texHover = NULL;

    Inventory* clickedInv = NULL;
    InventoryItem** clickedItem = NULL;
    std::vector<Inventory*> inventories;

    Inventory* CreateInventory(const char* name, int nbCols, int nbRows, const char* saveKey);
    void ToggleInventory(Inventory* inv);

    void ClearInventories();

    static Actor* make();

    static A_InvMgr* instance;
};

