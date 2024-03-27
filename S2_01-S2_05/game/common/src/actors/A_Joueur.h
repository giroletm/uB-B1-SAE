#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <common.h>

#include <core/Game.h>
#include <core/Actors.h>

#include "A_InvMgr.h"
#include "A_TextBox.h"

#define PLAYER_MAX_HP 50

class A_Joueur : public Actor {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	SDL_Texture* texture = 0;
	int pTW = 0, pTH = 0;

	SDL_Point targetPos = {0, 0};
	Vec2 targetVector = {0.0f, 0.0f};

	int direction = 1;		 // 0 = droite, 1 = bas, 2 = gauche, 3 = haut;
	int count = 0;
	int perso_pas = 0;
	int pv = PLAYER_MAX_HP;

	bool isInterracting = false;
	bool fin = false;

	Mix_Chunk* hitSound = NULL;
	Mix_Chunk* healSound = NULL;
	Mix_Chunk* deathSound = NULL;

	std::vector<PNJConvo*> convos;

	Inventory* inventory = NULL;
	bool invOpened = false;

	void ToggleInventory(bool force=false);
	InventoryItem* GetInventoryItem(int x, int y);
	InventoryItem* SetInventoryItem(int x, int y, int itemID, bool noSave=false);
	InventoryItem* AddInventoryItem(int itemID, bool noSave=false);

	void PointVie(int life);

	static Actor* make();

	static A_Joueur* instance;

	USING_STATES(A_Joueur);
	DECLARE_STATE(Walk);
	DECLARE_STATE(Wait);

	enum PlayerSettings {
		PLAYER_SETTINGS_NONE = 0,
		PLAYER_SETTINGS_FIRSTTIME = 1,
		PLAYER_SETTINGS_TITLESCREEN = 2
	};

};

