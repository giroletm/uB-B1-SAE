#include "morpion.h"


/* Début du morpion */


static COORD consS = (COORD){0, 0};
static COORD consP = (COORD){0, 0};

static Vec2_8 cursorPos;
static u8 **morpionBoard;
static int morpionSize = -1;

static u8 nowHeld = 0;
static u8 waitForRelease = 0;
static bool turn = false;
static u8 turns = 0;


int morpion::main() {
	while(true)
	{
		clearConsole();

		resetVars();

		std::cout << "Morpion - Matthieu" << std::endl;
		getConsolePos(&consS);


		while(true) {
			std::cout << "Taille de la grille ?: ";
			std::cin >> morpionSize;
			while(std::cin.fail()) {
				std::cout << "Ceci n'est pas un nombre." << std::endl << "Taille de la grille ?: ";
				std::cin.clear();
				std::cin.ignore(256,'\n');
				std::cin >> morpionSize;
			}
			if(morpionSize >= 3) break;
		}
		clearConsole(consS);

		morpionBoard = new u8*[morpionSize];
		for(int y = 0; y < morpionSize; y++) {
		    morpionBoard[y] = new u8[morpionSize];
		    for(int x = 0; x < morpionSize; x++) {
		    	(morpionBoard[y])[x] = 0;
		    }
		}


		std::cout << "Alignez " << ((morpionSize > 4) ? (morpionSize - 1) : morpionSize) << " pions !" << std::endl;
		getConsolePos(&consS);

		std::cout << "Au tour des " << (turn ? "O" : "X") << "  -  (ESC pour quitter)" << std::endl;
		getConsolePos(&consP);

		/*
		    ---------
		 -> | || || |
		    | || || |
		    | || || |
		    ---------
		    /|\
		     |
		*/
		std::cout << "   ---------" << std::endl;

		for(int y = 0; y < morpionSize; y++) {
			if(y == 0) std::cout << "-> ";
			else 	   std::cout << "   ";
			for(int x = 0; x < morpionSize; x++)
				std::cout << "| |";
			std::cout << std::endl;
		}

		std::cout << "   ---------" << std::endl << "   /|\\" << std::endl << "    |" << std::endl;

		renderAll();

		while(isKeyHeld(VK_SPACE) || isKeyHeld(VK_RETURN));

		while(true) {
			keyPresses();
			if(nowHeld & 0x10) return 0; // On sort si ESC est pressé

			if(nowHeld) {
				if(nowHeld & 1) { // Right
					if(cursorPos.x < morpionSize-1) cursorPos.x++;
					nowHeld &= ~1;
				}
				if(nowHeld & 2) { // Up
					if(cursorPos.y > 0) cursorPos.y--;
					nowHeld &= ~2;
				}
				if(nowHeld & 4) { // Left
					if(cursorPos.x > 0) cursorPos.x--;
					nowHeld &= ~4;
				}
				if(nowHeld & 8) { // Down
					if(cursorPos.y < morpionSize-1) cursorPos.y++;
					nowHeld &= ~8;
				}

				if(nowHeld & 0x20) {
					if(morpionBoard[cursorPos.x][cursorPos.y] == 0) {
						morpionBoard[cursorPos.x][cursorPos.y] = turn + 1;

						turn = !turn;

						takeConsoleToPos(consS);
						std::cout << "Au tour des " << (turn ? "O" : "X") << "  -  (ESC pour quitter)";
						takeConsoleToPos(consP);

						turns++;
					}

					nowHeld &= ~0x20;
				}

				renderAll();

				if(isVictory()) {
					break;
				}
				else if(turns == morpionSize*morpionSize) {
					std::cout << "Égalité !" << std::endl;
					break;
				}
			}

			Sleep(25);
		}
		std::cout << "Partie terminée ! (SPACE pour recommencer)" << std::endl;
		while(waitForRelease) keyPresses();
		while(!isKeyHeld(VK_SPACE));
	}

	return 0;
}


void morpion::resetVars() {
	cursorPos = (Vec2_8){0, 0};

	turn = rand() % 2;
	turns = 0;

	nowHeld = 0;

	if(morpionSize > 0) {
		for(int y = 0; y < morpionSize; y++)
			delete[] morpionBoard[y];
		delete[] morpionBoard;
	}

	
	morpionSize = -1;
}


void morpion::keyPresses() {
	if (isKeyHeld(VK_RIGHT) && !(waitForRelease & 1)) {
		nowHeld |= 1;
		waitForRelease |= 1;
	}

	if (isKeyHeld(VK_UP) && !(waitForRelease & 2)) {
		nowHeld |= 2;
		waitForRelease |= 2;
	}

	if (isKeyHeld(VK_LEFT) && !(waitForRelease & 4)) {
		nowHeld |= 4;
		waitForRelease |= 4;
	}

	if (isKeyHeld(VK_DOWN) && !(waitForRelease & 8)) {
		nowHeld |= 8;
		waitForRelease |= 8;
	}

	if (isKeyHeld(VK_ESCAPE)) {
		nowHeld |= 0x10;
	}

	if ((isKeyHeld(VK_SPACE) || isKeyHeld(VK_RETURN)) && !(waitForRelease & 0x20)) {
		nowHeld |= 0x20;
		waitForRelease |= 0x20;
	}


	if (!isKeyHeld(VK_RIGHT) && (waitForRelease & 1)) {
		waitForRelease &= ~1;
	}

	if (!isKeyHeld(VK_UP) && (waitForRelease & 2)) {
		waitForRelease &= ~2;
	}

	if (!isKeyHeld(VK_LEFT) && (waitForRelease & 4)) {
		waitForRelease &= ~4;
	}

	if (!isKeyHeld(VK_DOWN) && (waitForRelease & 8)) {
		waitForRelease &= ~8;
	}

	if (!(isKeyHeld(VK_SPACE) || isKeyHeld(VK_RETURN)) && (waitForRelease & 0x20)) {
		waitForRelease &= ~0x20;
	}
}


bool morpion::isVictory() {
	int victory = 0;

	int toAlign = (morpionSize > 4) ? (morpionSize - 1) : morpionSize;

	while(true) {
		// Vérification d'une victoire par ligne
		for(int ligne = 0; ligne < morpionSize; ligne++) {
			for(int pad = 0; pad <= morpionSize - toAlign; pad++) {
				bool checked = true;
				int vic = morpionBoard[pad][ligne];
				if(vic) {
					for(int cell = pad+1; cell < pad+toAlign; cell++) {
						if(morpionBoard[cell][ligne] != vic) {
							checked = false;
							break;
						}
					}
					if(checked) {
						victory = vic;
						break;
					}
				}
			}
			if(victory) break;
		}
		if(victory) break;

		// Vérification d'une victoire par colonne
		for(int colonne = 0; colonne < morpionSize; colonne++) {
			for(int pad = 0; pad <= morpionSize - toAlign; pad++) {
				bool checked = true;
				int vic = morpionBoard[colonne][pad];
				if(vic) {
					for(int cell = pad+1; cell < pad+toAlign; cell++) {
						if(morpionBoard[colonne][cell] != vic) {
							checked = false;
							break;
						}
					}
					if(checked) {
						victory = vic;
						break;
					}
				}
			}
			if(victory) break;
		}
		if(victory) break;

		// Vérification d'une victoire par diagonale LR
		for(int pad = 0; pad <= morpionSize - toAlign; pad++) {
			bool checked = true;
			int vic = morpionBoard[pad][pad];
			if(vic) {
				for(int i = pad+1; i < pad+toAlign; i++) {
					if(morpionBoard[i][i] != vic) {
						checked = false;
						break;
					}
				}
				if(checked) {
					victory = vic;
					break;
				}
			}
		}
		if(victory) break;

		// Vérification d'une victoire par diagonale RL
		for(int pad = 0; pad <= morpionSize - toAlign; pad++) {
			bool checked = true;
			int vic = morpionBoard[pad][pad];
			if(vic) {
				for(int i = pad+1; i < pad+toAlign; i++) {
					if(morpionBoard[(pad+toAlign)-i-1][i] != vic) {
						checked = false;
						break;
					}
				}
				if(checked) {
					victory = vic;
					break;
				}
			}
		}
		break;
	}

	if(victory) {
		std::cout << "Les " << ((victory == 2) ? "O" : "X") << " gagnent!" << std::endl;
		return true;
	}

	return false;
}


void morpion::renderAll() {
	takeConsoleToPos(consP);

	std::string str = "   ";
	for(int x = 0; x < morpionSize; x++) str += "---";
	str += "\n";

	for(int y = 0; y < morpionSize; y++) {
		if(y == cursorPos.y) str += "-> ";
		else 				 str += "   ";
		for(int x = 0; x < morpionSize; x++)
			str += "| |";
		str += "\n";
	}

	str += "   ";
	for(int x = 0; x < morpionSize; x++) str += "---";
	str += "\n   ";

	for(int x = 0; x < morpionSize; x++) {
		if(x == cursorPos.x) str += "/|\\";
		else				 str += "   ";
	}
	str += "\n   ";
	for(int x = 0; x < morpionSize; x++) {
		if(x == cursorPos.x) str += " | ";
		else				 str += "   ";
	}


	int lineF = 4 + (3 * morpionSize);
	for(int y = 0; y < morpionSize; y++) {
		for(int x = 0; x < morpionSize; x++) {
			int coordY = lineF + (lineF * y);
			int coordX = 4 + (3 * x);
			int coordT = coordY + coordX;
			if(morpionBoard[x][y] == 1) 	 str[coordT] = 'X';
			else if(morpionBoard[x][y] == 2) str[coordT] = 'O';
		}
	}

	std::cout << str << std::endl;
}