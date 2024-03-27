#include "shifumi.h"


/* Début du shifumi */


static COORD consS = (COORD){0, 0};
static COORD consP = (COORD){0, 0};

static bool isAI;


int shifumi::main() {
	clearConsole(); // Vider la console

	std::cout << "Pierre Feuille Ciseaux - Anthony" << std::endl;
	getConsolePos(&consS);
	
	isAI = askOrdi(consS);

	std::cout << "0 / 0" << std::endl;
	getConsolePos(&consP);
	
	int score1 = 0;
	int score2 = 0;
	
	while(true) {
		clearConsole(consS);
		std::cout<<score1<<" / "<<score2<<std::endl;

		int player1 = 0;
		while (player1 != 1 && player1 != 2 && player1 != 3) {
			std::cout<<"1 = pierre      2 = feuille     3 = ciseaux" << std::endl;

			std::cout<<"JOUEUR 1 : " <<std::endl;
			std::cin>>player1;
			while(std::cin.fail()) {
				std::cout << "Ceci n'est pas un nombre" << std::endl << "JOUEUR 1 : ";
				std::cin.clear();
				std::cin.ignore(256,'\n');
				std::cin >> player1;
			}

			clearConsole(consP);
		}

		int player2 = 0;
		if(isAI) {
			player2 = (rand() % 3) + 1;
			std::cout<<"L'ordinateur joue..."<<std::endl;
			Sleep(1000);
		}
		else {
			while (player2 != 1 && player2 != 2 && player2 != 3) {
				std::cout<<"1 = pierre      2 = feuille     3 = ciseaux" << std::endl;	

				std::cout<<"JOUEUR 2 : " <<std::endl;
				std::cin>>player2;
				while(std::cin.fail()) {
					std::cout << "Ceci n'est pas un nombre" << std::endl << "JOUEUR 2 : ";
					std::cin.clear();
					std::cin.ignore(256,'\n');
					std::cin >> player1;
				}

				clearConsole(consS);
			}
		}

		std::cout<<"1..."<<std::endl;
		Sleep(500);
		std::cout<<"2..."<<std::endl;
		Sleep(500);
		std::cout<<"3..."<<std::endl;
		Sleep(500);
		std::cout<<"Pierre..."<<std::endl;
		Sleep(500);
		std::cout<<"Feuille..."<<std::endl;
		Sleep(500);
		std::cout<<"Ciseaux!..."<<std::endl;
		if (player1 == 1 && player2 == 1) {
			std::cout<<"Il y a égalité ! Pierre VS Pierre !"<<std::endl;
		}

		if (player1 == 1 && player2 == 2) {
			std::cout<<"Le joueur 2 gagne! Pierre VS Feuille !"<<std::endl;
			score2++;   
		}

		if (player1 == 1 && player2 == 3) {
			std::cout<<"Le joueur 1 gagne! Pierre VS Ciseaux !"<<std::endl;
			score1++;   
		}

		if (player1 == 2 && player2 == 1) {
			std::cout<<"Le joueur 1 gagne! Feuille VS Pierre !"<<std::endl;
			score1++;   
		}

		if (player1 == 2 && player2 == 2) {
			std::cout<<"Il y a égalité! Feuille VS Feuille!"<<std::endl;
		}

		if (player1 == 2 && player2 == 3) {
			std::cout<<"Le joueur 2 gagne! Feuille VS Ciseaux!"<<std::endl;
			score2++;   
		}

		if (player1 == 3 && player2 == 1) {
			std::cout<<"Le joueur 2 gagne! Ciseaux VS Pierre !"<<std::endl;
			score2++;   
		}

		if (player1 == 3 && player2 == 2) {
			std::cout<<"Le joueur 1 gagne! Ciseaux VS Feuille !"<<std::endl;
			score2++;   
		}

		if (player1 == 3 && player2 == 3) {
			std::cout<<"Il y a égalité! Ciseaux VS Ciseaux!"<<std::endl;  
		}

		while(isKeyHeld(VK_RETURN));
		while(!isKeyHeld(VK_RETURN));
		while(isKeyHeld(VK_RETURN));
	}

	return 0; // "return 0;" termine notre jeu
}

