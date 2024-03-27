#include "batonnets.h"


/* Début du jeu des bâtonnets */


static COORD consS = (COORD){0, 0};
static COORD consP = (COORD){0, 0};

static bool turn = false;

static int nbBatonnets = -1;
static int batonnetsRetires = 0;

static bool isAI;


int batonnets::main() {
	clearConsole();

	std::cout << "Jeu des bâtonnets - Matthieu" << std::endl;
	getConsolePos(&consS);
	
	isAI = askOrdi(consS);

	while(true)
	{
		resetVars();

		while(true) {
			std::cout << "Combien de bâtonnets ?: ";
			std::cin >> nbBatonnets;
			while(std::cin.fail()) {
				std::cout << "Ceci n'est pas un nombre" << std::endl << "Combien de bâtonnets ?: ";
				std::cin.clear();
				std::cin.ignore(256,'\n');
				std::cin >> nbBatonnets;
			}

			if(nbBatonnets > 1) {
				break;
			}
			else {
				std::cout << "Nombre invalide. Une partie doit commencer par minimum 2 bâtonnets." << std::endl;
			}
		}

		clearConsole(consS);
		std::cout << "Au tour du Joueur " << (turn + 1) /*<< "  -  (ESC pour quitter)"*/ << std::endl;
		getConsolePos(&consP);


		while(true) {
			renderAll();
			// keyPresses();
			// if(nowHeld & 0x10) return 0; // On sort si ESC est pressé

			bool perdu = false;
			int tour = -1;
			while(true) {
					if(!isAI || !turn) {
						std::cout << "Combien de bâtonnets voulez-vous retirer ? (1, 2, 3): ";
						std::cin >> tour;
						while(std::cin.fail()) {
							std::cout << "Ceci n'est pas un nombre" << std::endl << "Combien de bâtonnets voulez-vous retirer ? (1, 2, 3): ";
							std::cin.clear();
							std::cin.ignore(256,'\n');
							std::cin >> tour;
						}

						if(tour < 1 || tour > 3) {
							std::cout << "Nombre invalide ! Vous ne pouvez retirer que 1, 2 ou 3 bâtonnets." << std::endl;
							continue;
						}

						int bLeft = nbBatonnets - batonnetsRetires - tour;
						if(bLeft < 0) {
							std::cout << "Vous ne pouvez pas retirer autant de bâtonnets: il n'y en a pas assez sur le plateau." << std::endl;
							continue;
						}
						else if(bLeft == 0) {
							std::cout << "Perdu! Le joueur " << (!turn + 1) << " gagne !" << std::endl;
							perdu = true;
							break;
						}
					}
					else {
						tour = (nbBatonnets - batonnetsRetires - 1) % 4;
						if(tour < 1) tour = 1;
						if(tour > 3) tour = 3;
					}

					batonnetsRetires += tour;
					turn = !turn;
					break;
			}

			if(perdu) {
				std::cout << "Partie terminée ! (SPACE pour recommencer)" << std::endl;
				while(!isKeyHeld(VK_SPACE));
				clearConsole(consS);
				break;
			}

			clearConsole(consS);
			std::cout << "Au tour du Joueur " << (turn + 1) /*<< "  -  (ESC pour quitter)"*/ << std::endl;
		}
	}

	return 0;
}


void batonnets::resetVars() {
	turn = rand() % 2;

	nbBatonnets = -1;
	batonnetsRetires = 0;
}


void batonnets::renderAll() {
	clearConsole(consP);

	for(int i = 0; i < nbBatonnets; i++) {
		if(i < (nbBatonnets - batonnetsRetires)) std::cout << "|";
		else									 std::cout << " ";
	}
	std::cout << std::endl << "Il reste " << (nbBatonnets - batonnetsRetires) << " bâtonnets sur le plateau" << std::endl << std::endl;
}