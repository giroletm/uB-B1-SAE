#pragma once

#include <core/States.h>

class ActorsManager;

class Actor {
public:
	Actor() : acState(this) {}
	virtual ~Actor();

	ActorsManager* parentMgr = NULL;

	SDL_Point tilePos = { 0, 0 };
	Vec2 pos = { 0.0f, 0.0f };
	u32 settings = 0;
	StateManager<Actor> acState;

	int drawOrder = 0;
	u32 activity = ACTOR_ACTIVITY_EXECUTE | ACTOR_ACTIVITY_DRAW;
	
	enum ActorActivity {
		ACTOR_ACTIVITY_NONE = 0,
		ACTOR_ACTIVITY_EXECUTE = 0b1,
		ACTOR_ACTIVITY_DRAW = 0b10,
	};

	virtual int onCreate();
	virtual int onDelete();
	virtual int onExecute();
	virtual int onDraw();

	void Delete();
};

class ActorsManager {
public:
	ActorsManager();

private:
	std::vector<Actor*> actors;
	std::vector<u32> tasking;

	enum ActorTasking {
		ACTOR_TASKING_NONE = 0,
		ACTOR_TASKING_CREATE = 1,
		ACTOR_TASKING_EXECUTE = 2,
		ACTOR_TASKING_DELETE = 3
	};

public:
	void createActor(Actor* actor, SDL_Point* tilePos=NULL, u32 settings=0);
	Actor* createActorByID(u32 id, SDL_Point* tilePos=NULL, u32 settings=0);
	void deleteActor(Actor* actor);
	void deleteActors();
	void handleActor(int actorIndex);
	void handleActors();
	void renderActors();
};