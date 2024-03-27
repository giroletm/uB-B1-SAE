#include "snake.h"


/* Début du snake */


const int sizeX = 32;
const int sizeY = 16;

static Vec2_8 headPos = (Vec2_8){0, 0};

static COORD consS = (COORD){0, 0};
static COORD consP = (COORD){0, 0};

static int direction = 0; // 0 = right; 1 = up; 2 = left; 3 = down;

static u32 score = 0;

static bool skipNextDeletion = false;
static bool dead = false;

static u8 snakeBoard[sizeX][sizeY];

static Vec2_8 bodyPos[sizeX * sizeY];

static bool isAI;
extern Highscores *highscores;


int snake::main() {
	while(true)
	{
		clearConsole();

		resetVars();

		std::cout << "Snake - Matthieu  -  ";
		getConsolePos(&consS);

		std::cout << "Score: 0  -  (ESC pour quitter)" << std::endl;
		getConsolePos(&consP);
	
		isAI = askOrdi(consP);

		snakeBoard[sizeX/2][sizeY/2] = 1;
		placeApple();

		while(!dead) {
			for(int i = 0; i < 4; i++)
			{
				keyPresses(); // On check les boutons 4 fois plus souvent que le snake n'est mise à jour
				if(direction == 5) {
					if(score > highscores->getHSScoreForMinigame(1)) highscores->setHSForMinigame(1, score);
					return 0; // On sort si ESC est pressé
				}

				if(i == 3) {
					if(updatePosition()) {
						takeConsoleToPos(consS);
						std::cout << "Score: " << score << "  -  (ESC pour quitter)" << std::endl;
						takeConsoleToPos(consP);
					}
				}

				Sleep(25);

				renderAll();
			}
		}
		
		if(!isAI && score > highscores->getHSScoreForMinigame(1)) highscores->setHSForMinigame(1, score);
		std::cout << "Perdu ! (SPACE pour recommencer)" << std::endl;

		while(!isKeyHeld(VK_SPACE));
	}

	return 0;
}


void snake::resetVars() {
	headPos = (Vec2_8){sizeX / 2, sizeY / 2};

	direction = (rand() % 4);

	score = 0;

	skipNextDeletion = false;
	dead = false;

	for(int x = 0; x < sizeX; x++)
		for(int y = 0; y < sizeY; y++)
			snakeBoard[x][y] = 0;

	for(int z = 0; z < sizeX*sizeY; z++) {
		bodyPos[z].x = 0;
		bodyPos[z].y = 0;
	}
}


void snake::keyPresses() {
	if(!isAI) {
		if (isKeyHeld(VK_RIGHT) && direction != 2)
			direction = 0;

		if (isKeyHeld(VK_UP) && direction != 3)
			direction = 1;

		if (isKeyHeld(VK_LEFT) && direction != 0)
			direction = 2;

		if (isKeyHeld(VK_DOWN) && direction != 1)
			direction = 3;

		if (isKeyHeld(VK_ESCAPE))
			direction = 5;
	}
	else {
		if (isKeyHeld(VK_ESCAPE)) {
			direction = 5;
			return;
		}

		if(headPos.y == 0) {
			if(headPos.x == 0) direction = 3;
			else direction = 2;
		}
		else if(headPos.y == 1) {
			if(headPos.x % 2 == 0) direction = 3;
			else				   direction = 0;
			if(headPos.x == sizeX - 1) direction = 1;
			if(sizeX % 2) {
				if(headPos.x == sizeX - 2) direction = 3;
				else if(headPos.x == sizeX - 3) direction = 0;
				else if(headPos.x == sizeX - 4) direction = 0;
			}
		}
		else if(headPos.y == sizeY - 1) {
			if((headPos.x % 2) == 0) direction = 0;
			else					 direction = 1;
			if(sizeX % 2) {
				if(headPos.x == sizeX - 1) direction = 1;
				else if(headPos.x == sizeX - 2) direction = 0;
			}
		}
		else {
			if(headPos.x % 2 == 0) direction = 3;
			else				   direction = 1;
			if(headPos.x == sizeX - 1) direction = 1;
			if(sizeX % 2) {
				if(headPos.y % 2) {
					if(headPos.x == sizeX - 2) direction = 2;
					else if(headPos.x == sizeX - 3) direction = 3;
				}
				else {
					if(headPos.x == sizeX - 2) direction = 3;
					else if(headPos.x == sizeX - 3) direction = 0;
				}
			}
		}


		// direction = iadir[headPos.y][headPos.x];
	}
}

void snake::placeApple() {
	while(true) {
		int randX = (rand() % sizeX);
		int randY = (rand() % sizeY);
		if(snakeBoard[randX][randY] == 0) {
			snakeBoard[randX][randY] = 2;
			break;
		}
	}
}


bool snake::updatePosition() {
	Vec2_8 oldPos = headPos;

	switch(direction) {
		case 0: // Right
		{
			if(headPos.x < sizeX-1) 
				headPos.x++;
			else
				headPos.x = 0;

			break;
		}
		case 1: // Up
		{
			if(headPos.y > 0) 
				headPos.y--;
			else
				headPos.y = sizeY-1;

			break;
		}
		case 2: // Left
		{
			if(headPos.x > 0) 
				headPos.x--;
			else
				headPos.x = sizeX-1;

			break;
		}
		case 3: // Down
		{
			if(headPos.y < sizeY-1) 
				headPos.y++;
			else
				headPos.y = 0;

			break;
		}
	}

	if(snakeBoard[headPos.x][headPos.y] == 1) {
		dead = true;
		return false;
	}

	if(snakeBoard[headPos.x][headPos.y] == 2) skipNextDeletion = true;
	snakeBoard[headPos.x][headPos.y] = 1;

	if(skipNextDeletion) {
		score++;

		memmove(&bodyPos[1], &bodyPos, sizeof(Vec2_8) * (score-1));
		bodyPos[0] = oldPos;

		placeApple();
		skipNextDeletion = false;
		return true;
	}

	if(score > 0) {
		snakeBoard[bodyPos[score - 1].x][bodyPos[score - 1].y] = 0;
		memmove(&bodyPos[1], &bodyPos, sizeof(Vec2_8) * score);
		bodyPos[0] = oldPos;
	}
	else {
		snakeBoard[oldPos.x][oldPos.y] = 0;
	}

	return false;
}


void snake::renderAll() {
	takeConsoleToPos(consP);

	for(int line = 0; line < sizeY; line++)
	{
		for(int col = 0; col < sizeX; col++)
		{
			switch(snakeBoard[col][line]) {
				default:
				case 0:
				{
					std::cout << " ";
					break;
				}
				case 1:
				{
					std::cout << "*";
					break;
				}
				case 2:
				{
					std::cout << "$";
					break;
				}
			}
		}

		std::cout << std::endl;
	}
}