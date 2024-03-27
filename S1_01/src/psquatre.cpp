#include "psquatre.h"


/* Début du puissance 4 */


static COORD consS = (COORD) {0, 0};
static COORD consP = (COORD) {0, 0};

static char tbl [6][7] ={	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
 							{' ', ' ', ' ', ' ', ' ', ' ', ' '},
 							{' ', ' ', ' ', ' ', ' ', ' ', ' '},
 							{' ', ' ', ' ', ' ', ' ', ' ', ' '},
 							{' ', ' ', ' ', ' ', ' ', ' ', ' '},
 							{' ', ' ', ' ', ' ', ' ', ' ', ' '}};



void psquatre::init (char tbl[6][7], int size,int size2) {
	for (int i =0; i<size; i++) {

		for (int j =0; j<size2; j++) {

			tbl[i][j]=' ';
		}
	}
}


void psquatre::affichertab() {

	for(int ligne = 0; ligne < 6; ligne++) {

		for(int colonne = 0; colonne < 7; colonne++) {

			std::cout << tbl[ligne][colonne] << "|";
		}
		std::cout << std::endl;
	}
}


bool psquatre::Victoire(char tbl[][7], int ligneSel, int colonneSel, char joueur) {
	// On regarde si il y a une victoire en ligne
	int vartemp = 0;
	for(int colonne = 0; colonne < 7; colonne++) {

		if (tbl[ligneSel][colonne] == joueur) {

			vartemp = vartemp + 1;
		}
		else {
			vartemp = 0;
		}

		if (vartemp == 4) {
			return true;
		}
	}

	// On regarde si il y a une victoire en colonne
	vartemp = 0;
	for(int ligne = 0; ligne < 6; ligne++) {

		if (tbl[ligne][colonneSel] == joueur) {

			vartemp = vartemp + 1;
		}
		else {
			vartemp = 0;
		}
		
		if (vartemp == 4) {
			return true;
		}
	}

	// On regarde s'il y a une victoire en diagonale vers la droite
	int ligne = ligneSel;
	int colonne = colonneSel;
	while(ligne > 0 && colonne > 0) {
		ligne--;
		colonne--;
	}
	vartemp = 0;
	while(ligne < 6 && colonne < 7) {

		if (tbl[ligne][colonne] == joueur) {

			vartemp = vartemp + 1;
		}
		else {
			vartemp = 0;
		}
		
		if (vartemp == 4) {
			return true;
		}

		ligne++;
		colonne++;
	}


	// On regarde s'il y a une victoire en digonale vers la gauche
	ligne = ligneSel;
	colonne = colonneSel;
	while(ligne > 0 && colonne < 6) {
		ligne--;
		colonne++;
	}
	vartemp = 0;
	while(ligne < 6 && colonne > 0) {
		if (tbl[ligne][colonne] == joueur) {

			vartemp = vartemp + 1;
		}
		else {
			vartemp = 0;
		}
		
		if (vartemp == 4) {
			return true;
		}

		ligne++;
		colonne--;
	}

	return false;
}
	

int psquatre::main() {
	clearConsole(); // Vider la console

	std::cout << "Puissance 4 - Théo\n";
	getConsolePos(&consS);
	
	std::cout<<" → Le puissance 4, le jeu de mon enfance. Quoi ?! Vous ne connaissez pas les règles ?!"<<std::endl;
	std::cout<<" → Le puissance 4 a des règles assez simples: le premier à aligner 4 pions gagne. Pour cela choisissez la colonne où vous voulez mettre votre pion. J'espère que vous avez compris. Sinon, ce n'est pas mon problème."<<std::endl;
	std::cout<<" → Bonne chance à vous et amusez-vous bien surtout."<<std::endl;
	std::cout<<std::endl;
	getConsolePos(&consP);
	
	
	int win = 0;
	int quelJoueur = 1;
	int size = 7;
	int size2 = 6;
	init (tbl, size, size2 );
	while(!win) {
		clearConsole(consP);
		affichertab();

		

		int colonneSel = 0;

		while(colonneSel < 1 || colonneSel > 7) {
			std :: cout << "Joueur "<< quelJoueur<< " choisissez votre colonne (1 à 7)"<< std::endl;
			std :: cin >> colonneSel;
			while(std::cin.fail()) {
				std::cout << "Ceci n'est pas un nombre" << std::endl << "Joueur "<< quelJoueur<< " choisissez votre colonne (1 à 7)"<< std::endl;
				std::cin.clear();
				std::cin.ignore(256,'\n');
				std::cin >> colonneSel;
			}
			if(colonneSel < 1 || colonneSel > 7) std::cout << "Numéro de colonne invalide !" << std::endl;
		}

		int ligneSel = 5;
		while(tbl[ligneSel][colonneSel-1] != ' ') {
			ligneSel = ligneSel - 1;
			if (ligneSel < 0) {
				break;
			}
		}

		if (ligneSel>=0) {
			char joueur = ' ';
			if (quelJoueur == 1) {
				joueur = 'O';
			}
			else {
				joueur = 'X';
			}

			tbl[ligneSel][colonneSel-1] = joueur;

			bool gagne = Victoire(tbl, ligneSel, colonneSel-1, joueur);
			if (gagne) {
				clearConsole(consP);
				affichertab();
				std::cout << "Le joueur " << quelJoueur << " a gagné" << std::endl;
				win = 1;
			}
		

			if (quelJoueur == 1) quelJoueur = 2;
			else quelJoueur = 1;
	
		}
		else {
			std::cout<<"La colonne n'est pas disponible."<<std::endl;
		}
		
		
	}
	

	while(isKeyHeld(VK_SPACE));
	std::cout << "Appuyez sur Espace pour continuer..."<<std::endl;
	while(!isKeyHeld(VK_SPACE));

	return 0; // "return 0;" termine notre jeu
}

// Faire que si 4 à cotés gagnent
// la win
// contre robot
// animation
// Déplacement entre colonne avec fleches
// couleurs

