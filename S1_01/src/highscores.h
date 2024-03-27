#pragma once

#include <string>

#include "common.h"
#include "io.h"

class GameScore;

class Highscores {
private:
	GameScore *data;
	size_t dataSize;

public:
	Highscores();
	~Highscores();
	
	void loadFile();
	void saveFile();

	void showHighscores();

	u32 getHSScoreForMinigame(u32 id);
	char* getHSNameForMinigame(u32 id);
	void setHSForMinigame(u32 id, u32 score);
};

class GameScore {
public:
	u32 score;
	char* name;
};