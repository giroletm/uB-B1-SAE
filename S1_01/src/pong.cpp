#include "pong.h"


/* Début du pong */


const int sizeX = 32;
const int sizeY = 16;

static Vec2 ballPos = (Vec2){0, 0};
static Vec2 leftPos = (Vec2){0, 0};
static Vec2 rightPos = (Vec2){0, 0};

static COORD consS = (COORD){0, 0};
static COORD consP = (COORD){0, 0};

static bool isUp = false;
static bool isRight = false;

static int won = 0;
static int score1 = 0;
static int score2 = 0;

static u8 nowHeld = 0;

static bool isAI;
extern Highscores *highscores;


int pong::main() {
	clearConsole();

	resetVars();

	std::cout << "Pong - Matthieu  -  ";
	getConsolePos(&consS);

	std::cout << "0-0  -  (ESC pour quitter)" << std::endl;
	getConsolePos(&consP);
	
	isAI = askOrdi(consP);

	while(true)
	{
		if (won > 0)
		{
			if (won == 1) score1++;
			else if (won == 2) score2++;

			won = 0;

			resetVars();

			takeConsoleToPos(consS);
			std::cout << score1 << "-" << score2 << "  -  (ESC pour quitter)" << std::endl;
			takeConsoleToPos(consP);
		}

		nowHeld = 0;
		for(int i = 0; i < 2; i++)
		{
			keyPresses(); // On check les boutons 4 fois plus souvent que la balle n'est mise à jour
			if(nowHeld & 0x10) {
				u32 score = isAI ? score1 : std::max(score1, score2);
				if(score > highscores->getHSScoreForMinigame(0)) highscores->setHSForMinigame(0, score);
				return 0; // On sort si ESC est pressé
			}
			updatePosition(i == 1);
			Sleep(50);

			renderAll();
		}
	}

	return 0;
}


void pong::resetVars() {
	ballPos = (Vec2){sizeX / 2, rand() % sizeY};
	leftPos = (Vec2){0, sizeY / 2 - 2};
	rightPos = (Vec2){sizeX - 1, sizeY / 2 - 2};

	isUp = ((rand() % 2) == 0) ? false : true;
	isRight = ((rand() % 2) == 0) ? false : true;
}


void pong::keyPresses() {
	if(!isAI) {
		if (isKeyHeld(VK_UP))
			nowHeld |= 1;

		if (isKeyHeld(VK_DOWN))
			nowHeld |= 2;
	}

	if (isKeyHeld(VK_Z))
		nowHeld |= 4;

	if (isKeyHeld(VK_S))
		nowHeld |= 8;

	if (isKeyHeld(VK_ESCAPE))
		nowHeld |= 0x10;
}


void pong::updatePosition(bool updateBall) {
	if(isAI) {
		nowHeld &= ~3;
		if(isRight) {
			if(rightPos.y > ballPos.y - 2) nowHeld |= 1;
			if(rightPos.y < ballPos.y - 2) nowHeld |= 2;
		}
	}



	if(nowHeld & 1) // Up
		if (rightPos.y > 0)
			rightPos.y--;

	if(nowHeld & 2) // Down
		if (rightPos.y < sizeY - 4)
			rightPos.y++;

	if(nowHeld & 4) // Z
		if (leftPos.y > 0)
			leftPos.y--;

	if(nowHeld & 8) // S
		if (leftPos.y < sizeY - 4)
			leftPos.y++;

	if(updateBall) {
		if (ballPos.y == 0 || ballPos.y == sizeY - 1) isUp = !isUp;

		if (isUp) ballPos.y++;
		else ballPos.y--;


		if (isRight) ballPos.x++;
		else ballPos.x--;

		if (isRight && ballPos.x == rightPos.x - 1)
		{
			if (ballPos.y >= rightPos.y && ballPos.y < rightPos.y + 4)
			{
				isRight = false;
			}
			else won = 1;
		}

		else if (!isRight && ballPos.x == leftPos.x + 1)
		{
			if (ballPos.y >= leftPos.y && ballPos.y < leftPos.y + 4)
			{
				isRight = true;
			}
			else won = 2;
		}
	}
}


void pong::renderAll() {
	takeConsoleToPos(consP);

	for(int line = 0; line < sizeY; line++)
	{
		for(int col = 0; col < sizeX; col++)
		{
			if (col == leftPos.x)
			{
				if (line >= leftPos.y && line < leftPos.y + 4) std::cout << "|";
				else std::cout << " ";
			}

			else if (col == rightPos.x)
			{
				if (line >= rightPos.y && line < rightPos.y + 4) std::cout << "|";
				else std::cout << " ";
			}

			else if (col == ballPos.x && line == ballPos.y) std::cout << "*";

			else std::cout << " ";
		}

		std::cout << std::endl;
	}
}