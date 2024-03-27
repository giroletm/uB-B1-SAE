#include "common.h" // Typedefs, defines, structures et variables communes
#include "io.h" // Inclus <windows.h>
#include "highscores.h"

#include "pong.h"
#include "snake.h"
#include "morpion.h"
#include "batonnets.h"
#include "shifumi.h"
#include "psquatre.h"
#include "blackjack.h"
#include "pokemon.h"
#include "ba.h"


COORD mainScreenPos = (COORD){0, 0};

const char* minigames[] = {
	"Pong",
	"Snake",
	"Morpion",
	"Jeu des bâtonnets",
	"Pierre, Feuille, Ciseaux",
	"Puissance 4",
	"Blackjack",
	"Pokémon",
};
u8 minigameCount = sizeof(minigames) / sizeof(char*);

Highscores *highscores = 0;

void exiting() {
	std::cout << "Merci d'avoir joué !" << std::endl;
}


int main() {
	srand(time(NULL));
	std::atexit(exiting); 

	SetConsoleOutputCP(65001); // Mets la console en mode UTF-8 pour éviter de casser les accents. Merci à https://developercommunity.visualstudio.com/t/c-cincout-inconsistent-treatment-for-accented-char/122865

	getConsolePos(&mainScreenPos);

	highscores = new Highscores;

	while(true) {
		clearConsole();

		int game;
		for(int i = 0; i < minigameCount; i++) {
			std::cout << i << ". " << minigames[i] << std::endl;
		}
		std::cout << "100. Voir les meilleurs scores" << std::endl;

		#ifdef DEBUG
		std::cout << std::endl;
		std::cout << "99. BA!!" << std::endl;
		std::cout << std::endl;
		#endif

		std::cout << "Inv. Quitter" << std::endl;
		std::cout << std::endl << "-> ";

		while(isKeyHeld(VK_RETURN));
		
		std::cin >> game;
		if(std::cin.fail()) 
			return 0;

		if(game == 0) {
			pong *pg = new pong;
			pg->main();
			delete pg;
		}
		else if(game == 1) {
			snake *snk = new snake;
			snk->main();
			delete snk;
		}
		else if(game == 2) {
			morpion *mtpn = new morpion;
			mtpn->main();
			delete mtpn;
		}
		else if(game == 3) {
			batonnets *btnts = new batonnets;
			btnts->main();
			delete btnts;
		}
		else if(game == 4) {
			shifumi *sfm = new shifumi;
			sfm->main();
			delete sfm;
		}
		else if(game == 5) {
			psquatre *ps4 = new psquatre;
			ps4->main();
			delete ps4;
		}
		else if(game == 6) {
			blackjack *blj = new blackjack;
			blj->main();
			delete blj;
		}
		else if(game == 7) {
			pokemon *pkm = new pokemon;
			pkm->main();
			delete pkm;
		}
		else if(game == 100) {
			highscores->showHighscores();
		}
		else if(game == 99) {
			bapple *ba = new bapple;
			ba->main();
			delete ba;
		}
		else {
			return 0;
		}
	}


	return 0;
}