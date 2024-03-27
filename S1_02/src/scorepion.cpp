#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <vector>
#include <time.h>


/**************/
/* Structures */
/**************/

struct Point {
	int x;
	int y;
};

struct Pion {
	Point coords;
	int score;
	bool mort;
};

struct Possibilite {
	Point pion;
	Point attaque;
	double chance;
};


/****************/
/* Déclarations */
/****************/

// Fonctions IO
COORD getConsolePos(); // Obtiens la position du curseur de la console dans un objet COORD
void takeConsoleToPos(COORD coords); // Mets le curseur de la console à la position d'un object COORD
void clearConsole(COORD screenPos); // Vide la console à partir de la position d'un curseur donné
bool isKeyHeld(int key); // Renvoie true si une touche donnée est appuyée, false sinon

// Jeu - Commun
void Init(); // Alloue le tableau
void Reset(); // Réinitialise le tableau à son état d'origine
void Afficher(Point pionPos, Point attaquePos); // Affiche le tableau avec une case pionPos en vert, une case attaquePos surlignée
void AjusterSel(int &selX, int &selY); // Si une case sélectionnée dépasse les bords, on l'ajuste pour qu'elle ne les dépasse plus

// Jeu - Joueur
bool CoupValide(Point pionPos, Point attaquePos, bool tour); // Retourne true si le coup à la case attaquePos d'un pion à la case pionPos est valide
bool PionJouable(Point pionPos, bool tour); // Retourne true si un pion peut jouer sur au moins une case
void ObtenirTour(Point &pionPos, Point &attaquePos, bool tour); // Fais jouer le joueur actuel
void Joue(Point pionPos, Point attaquePos); // Faire jouer un pion à une case donnée
char Victoire(); // Checker s'il y a eu une victoire. Si oui, on renvoie le char du gagnant. On renvoie un tiret dans le cas d'une égalité

// Jeu - IA
void InitPionsIA(); // Alloue les instances des pions de chaque joueur
void ResetPionsIA(bool resetScore); // Réinitialise les tableaux de pions à leurs coordonnées d'origine, et leur score si demandé
Pion* TrouverPion(int x, int y, bool tour); // Retourne le pointeur d'un pion à des coordonnées
void ObtenirTourIA(Point &pionPos, Point &attaquePos, bool tour, bool contreJoueur); // Obtiens le coup que l'IA veux jouer


/****************/
/* Fonctions IO */
/****************/

// Obtiens la position du curseur de la console dans un objet COORD
COORD getConsolePos() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
	{
		return csbi.dwCursorPosition;
	}

	return {0, 0};
}

// Mets le curseur de la console à la position d'un object COORD
void takeConsoleToPos(COORD coords) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coords);
}

// Vide la console à partir de la position d'un curseur donné
void clearConsole(COORD screenPos) { // Merci internet: https://learn.microsoft.com/en-us/windows/console/clearing-the-screen
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;

	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, screenPos, &cCharsWritten))
		return;

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;

	if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, screenPos, &cCharsWritten))
		return;

	SetConsoleCursorPosition(hConsole, screenPos);
}

// Renvoie true si une touche donnée est appuyée, false sinon
bool isKeyHeld(int key) {
	return GetKeyState(key) & 0x8000;
}


/****************/
/* Jeu - Commun */
/****************/

char** tableau;
int longueur;
int hauteur;
COORD tableauCursor;

// Alloue le tableau
void Init() {
	tableau = new char*[hauteur];
	for(int y = 0; y < hauteur; y++) {
		tableau[y] = new char[longueur];
	}
}

// Réinitialise le tableau à son état d'origine
void Reset() {
	for(int y = 0; y < hauteur; y++) {
		for(int x = 0; x < longueur; x++) {
			if(y == 0) tableau[y][x] = 'O';
			else if(y == (hauteur-1)) tableau[y][x] = 'X';
			else tableau[y][x] = ' ';
		}
	}
}

// Affiche le tableau avec une case pionPos en vert, une case attaquePos surlignée
void Afficher(Point pionPos, Point attaquePos) {
	takeConsoleToPos(tableauCursor);
	for(int y = 0; y < hauteur; y++) {
		for(int x = 0; x < longueur; x++) {
			std::cout << "|";

			if(x == pionPos.x && y == pionPos.y)
				std::cout << "\x1B[92m";

			if(x == attaquePos.x && y == attaquePos.y)
				std::cout << "\x1B[7m";

			std::cout << tableau[y][x];

			if((x == pionPos.x && y == pionPos.y) || (x == attaquePos.x && y == attaquePos.y))
				std::cout << "\x1B[0m";
		}
		std::cout << "|" << std::endl;
	}
}

// Si une case sélectionnée dépasse les bords, on l'ajuste pour qu'elle ne les dépasse plus
void AjusterSel(int &selX, int &selY) {
	if(selX >= longueur) {
		selX = 0;
		selY++;
	}
	if(selX < 0) {
		selX = longueur - 1;
		selY--;
	}

	if(selY >= hauteur) {
		selY = 0;
	}
	if(selY < 0) {
		selY = hauteur - 1;	
	}
}


/****************/
/* Jeu - Joueur */
/****************/

// Retourne true si le coup à la case attaquePos d'un pion à la case pionPos est valide
bool CoupValide(Point pionPos, Point attaquePos, bool tour) {
	char joueurQuiNeJouePas = tour ? 'O' : 'X';

	int derriere = (tour) ? 1 : -1;

	// Si on attaque sur sa ligne de départ alors impossible qu'il y ait son pion derrière la case
	if(!tour && attaquePos.y <= 0) return false; 
	if(tour && attaquePos.y >= hauteur-1) return false; 

	// Obtention & racourcissement des noms pour pas que ce soit trop moche par la suite
	int pX = pionPos.x;
	int pY = pionPos.y;
	int aX = attaquePos.x;
	int aY = attaquePos.y;
	char CaseAttq = tableau[aY][aX];

	// Si la case attaquée est vide, alors le coup est valide seulement si il y a notre pion derrière
	if(CaseAttq == ' ' && ((pX == aX) && (pY == aY+derriere))) return true;

	// Si la case attaquée est prise par l'adversaire, alors le coup est valide seulement si notre pion est à l'une des diagonales arrières
	if(CaseAttq == joueurQuiNeJouePas && (pY == aY+derriere) && ((pX == aX+1) || (pX == aX-1))) return true;

	return false;
}

// Retourne true si un pion peut jouer sur au moins une case
bool PionJouable(Point pionPos, bool tour) {
	int derriere = (tour) ? 1 : -1;

	// Un pion sur la ligne de départ adverse ne peut plus jouer
	if(!tour && pionPos.y >= hauteur-1) return false; 
	if(tour && pionPos.y <= 0) return false; 

	// Si le pion peut jouer devant lui, alors true
	if(CoupValide(pionPos, {pionPos.x, pionPos.y-derriere}, tour)) return true;

	// Si le pion n'est pas tout à gauche du tableau, alors s'il peut jouer à sa gauche, true
	if(pionPos.x > 0)
		if(CoupValide(pionPos, {pionPos.x-1, pionPos.y-derriere}, tour)) return true;

	// Si le pion n'est pas tout à droite du tableau, alors s'il peut jouer à sa droite, true
	if(pionPos.x < longueur-1)
		if(CoupValide(pionPos, {pionPos.x+1, pionPos.y-derriere}, tour)) return true;

	// Dans tous les autres cas, false
	return false;
}

// Fais jouer le joueur actuel
void ObtenirTour(Point &pionPos, Point &attaquePos, bool tour) {
	// Récupération du caractère du joueur qui joue
	char joueurQuiJoue = tour ? 'X' : 'O';

	// Réinitialisation du pion attaquant et de la case attaquée à 0;0
	pionPos = {0, 0};
	attaquePos = {0, 0};

	// Première boucle: on demande le pion à jouer
	bool sens = false;
	while(true) {
		// On itère à travers les cases jusqu'à trouver un pion nous appartenant et qui est jouable
		while(tableau[pionPos.y][pionPos.x] != joueurQuiJoue || !PionJouable(pionPos, tour)) {
			pionPos.x += (sens) ? -1 : 1;
			AjusterSel(pionPos.x, pionPos.y);
		}

		// On affiche le pion trouvé, notez que le pion affiché en vert a ses coordonnées à -1 pour l'empêcher de s'afficher vu qu'on veut surligner le pion actuel.
		Afficher({-1, -1}, pionPos);

		// On attends qu'aucune touche ne soit appuyée, puis qu'on en appuie une (pour éviter qu'on maintienne la touche)
		while(isKeyHeld(VK_LEFT) || isKeyHeld(VK_RIGHT) || isKeyHeld(VK_SPACE));
		while(!isKeyHeld(VK_LEFT) && !isKeyHeld(VK_RIGHT) && !isKeyHeld(VK_SPACE));
		
		// Si droite est appuyé, on avance de un et on revient au début de la boucle pour réitérer en avant jusqu'à trouver un pion jouable
		if(isKeyHeld(VK_RIGHT)) {
			pionPos.x++;
			sens = false;
			AjusterSel(pionPos.x, pionPos.y);
		}
		// Si gauche est appuyé, on recule de un et on revient au début de la boucle pour réitérer en arrière jusqu'à trouver un pion jouable
		if(isKeyHeld(VK_LEFT)) {
			pionPos.x--;
			sens = true;
			AjusterSel(pionPos.x, pionPos.y);
		}
		// Si espace est appuyé, on casse la boucle: on considère que le pion actuel est celui sélectionné
		if(isKeyHeld(VK_SPACE)) {
			break;
		}
	}

	// Deuxième boucle: on demande où le pion sélectionné jouera
	sens = false;
	while(true) {
		// On itère à travers les cases jusqu'à trouver une case jouable par le pion sélectionné
		while(!CoupValide(pionPos, attaquePos, tour)) {
			attaquePos.x += (sens) ? -1 : 1;
			AjusterSel(attaquePos.x, attaquePos.y);
		}

		// On affiche la case trouvée
		Afficher(pionPos, attaquePos);

		// On attends qu'aucune touche ne soit appuyée, puis qu'on en appuie une (pour éviter qu'on maintienne la touche)
		while(isKeyHeld(VK_LEFT) || isKeyHeld(VK_RIGHT) || isKeyHeld(VK_SPACE));
		while(!isKeyHeld(VK_LEFT) && !isKeyHeld(VK_RIGHT) && !isKeyHeld(VK_SPACE));
		
		// Si droite est appuyé, on avance de un et on revient au début de la boucle pour réitérer en avant jusqu'à trouver une autre case jouable
		if(isKeyHeld(VK_RIGHT)) {
			attaquePos.x++;
			sens = false;
			AjusterSel(attaquePos.x, attaquePos.y);
		}
		// Si gauche est appuyé, on recule de un et on revient au début de la boucle pour réitérer en arrière jusqu'à trouver une autre case jouable
		if(isKeyHeld(VK_LEFT)) {
			attaquePos.x--;
			sens = true;
			AjusterSel(attaquePos.x, attaquePos.y);
		}
		// Si espace est appuyé, on casse la boucle: on considère que la case actuelle est celle sélectionnée
		if(isKeyHeld(VK_SPACE)) {
			break;
		}
	}
}

// Faire jouer un pion à une case donnée
void Joue(Point pionPos, Point attaquePos) {
	tableau[attaquePos.y][attaquePos.x] = tableau[pionPos.y][pionPos.x];
	tableau[pionPos.y][pionPos.x] = ' ';
}

// Checker s'il y a eu une victoire. Si oui, on renvoie le char du gagnant. On renvoie un tiret dans le cas d'une égalité
char Victoire() {
	// Si il y a des X sur la première ligne, alors ils gagnent
	for(int x = 0; x < longueur; x++) {
		if(tableau[0][x] == 'X') return 'X';
	}

	// Si il y a des O sur la dernière ligne, alors ils gagnent
	for(int x = 0; x < longueur; x++) {
		if(tableau[hauteur-1][x] == 'O') return 'O';
	}

	// S'il n'y a plus aucun O ou X, alors l'aversaire gagne
	bool hasX = false;
	bool hasO = false;
	for(int y = 0; y < hauteur; y++) {
		for(int x = 0; x < longueur; x++) {
			if(!hasX && tableau[y][x] == 'X') hasX = true;
			if(!hasO && tableau[y][x] == 'O') hasO = true;

			if(hasX && hasO) break;
		}
	}
	if(!hasX) return 'O';
	if(!hasO) return 'X';

	// Vérification des jouabilités
	bool jouableX = false;
	bool jouableO = false;
	for(int y = 0; y < hauteur; y++) {
		for(int x = 0; x < longueur; x++) {
			if(tableau[y][x] != ' ') {
				bool tr = tableau[y][x] == 'X';
				if(PionJouable({x, y}, tr)) {
					if(tr)
						jouableX = true;
					else
						jouableO = true;

					break;
				}
			}
		}
	}

	// Si aucun pion n'est jouable, alors égalité
	if(!jouableX && !jouableO) return '-';

	// Si un joueur peut jouer mais pas l'autre, il gagne
	if(jouableO && !jouableX) return 'O';
	if(!jouableO && jouableX) return 'X';

	// Sinon, on continue
	return ' ';
}


/***************************/
/* Jeu - IA (Comportement) */
/***************************/

Pion* pionsO;
Pion* pionsX;

// Alloue les instances des pions de chaque joueur
void InitPionsIA() {
	pionsO = new Pion[longueur];
	pionsX = new Pion[longueur];
}

// Réinitialise les tableaux de pions à leurs coordonnées d'origine, et leur score si demandé
void ResetPionsIA(bool resetScore) {
	for(int x = 0; x < longueur; x++) {
		pionsO[x].coords.x = x;
		pionsO[x].coords.y = 0;
		pionsO[x].mort = false;

		pionsX[x].coords.x = x;
		pionsX[x].coords.y = hauteur-1;
		pionsX[x].mort = false;

		if(resetScore) {
			pionsO[x].score = 1;
			pionsX[x].score = 1;
		}
	}
}

// Retourne le pointeur d'un pion à des coordonnées
Pion* TrouverPion(int x, int y, bool tour) {
	Pion* mesPions = (tour) ? pionsX : pionsO;
	for(int i = 0; i < longueur; i++) {
		if(mesPions[i].coords.x == x && mesPions[i].coords.y == y) {
			return &mesPions[i];
		}
	}

	return NULL;
}

// Obtiens le coup que l'IA veux jouer
void ObtenirTourIA(Point &pionPos, Point &attaquePos, bool tour, bool contreJoueur) {
	if(contreJoueur)
		Afficher({-1, -1}, {-1, -1});



	char monPion = (tour) ? 'X' : 'O';


	// Obtenir la liste des pions jouables par notre joueur
	std::vector<Pion*> pions;
	int scoreTotal = 0;
	for(int y = 0; y < hauteur; y++) {
		for(int x = 0; x < longueur; x++) {
			if(tableau[y][x] == monPion) {
				Pion *pionActuel = TrouverPion(x, y, tour);
				if(!pionActuel->mort && PionJouable(pionActuel->coords, tour)) {
					scoreTotal += pionActuel->score;
					pions.push_back(pionActuel);
				}
			}
		}
	}


	// Générer un nombre aléatoire pour décider du pion à jouer selon son score
	double f = (double)rand() * 100.0f / RAND_MAX;

	Pion *aJouer;

	double chanceAvant = 0.0;
	int nombrePions = pions.size();
	for(int i = 0; i < nombrePions; i++) {
		aJouer = pions[i];

		double chance = (aJouer->score * 100.0 / (double)scoreTotal);
		if(f >= chanceAvant && f < (chanceAvant + chance)) {
			break;
		}

		chanceAvant += chance;
	}

	pionPos = aJouer->coords;


	// Obtenir les attaques possibles du pion choisi, en sachant que s'il peut manger on ignore les coups où il avance tout droit
	std::vector<Point> atq;
	bool peutManger = false;
	for(int y = 0; y < hauteur; y++) {
		for(int x = 0; x < longueur; x++) {
			if(CoupValide(aJouer->coords, {x, y}, tour)) {
				if(!peutManger) {
					if(aJouer->coords.x != x) {
						atq.clear();
						peutManger = true;
					}
					atq.push_back({x, y});
				}
				else {
					if(aJouer->coords.x != x) {
						atq.push_back({x, y});
					}
				}
			}
		}
	}


	// Générer un nombre aléatoire pour décider de l'attaque à sélectionner
	int nbAttaques = atq.size();
	if(nbAttaques == 1) {
		attaquePos = atq[0];
	}
	else {
		int numAtq = rand() % nbAttaques;
		attaquePos = atq[numAtq];
	}


	// Incrémentation de la valeur d'un pion s'il en a mangé un autre
	if(tableau[attaquePos.y][attaquePos.x] != ' ') {
		aJouer->score++;
		Pion *pionMange = TrouverPion(attaquePos.x, attaquePos.y, !tour);
		if(pionMange) {
			pionMange->mort = true;
			pionMange->coords = {-1, -1};
		}
	}
	aJouer->coords = attaquePos;


	// Animation de jeu de l'IA
	if(contreJoueur) {
		std::cout << "L'IA joue..." << std::endl;

		Sleep(500);
		Afficher({-1, -1}, pionPos);
		Sleep(500);
		Afficher(pionPos, attaquePos);
		Sleep(500);
		clearConsole(tableauCursor);
	}
}


/***********************/
/* Fonction principale */
/***********************/

int main() {
	SetConsoleOutputCP(65001);
	system(" "); // Les couleurs ne s'affichent pas sinon, Cf. https://stackoverflow.com/questions/58165576/why-is-ansi-printing-the-code-the-first-time-the-function-is-called-and-printing

	srand(time(NULL));

	std::cout << "Quelle longueur de tableau ?: ";
	std::cin >> longueur;

	std::cout << "Quelle hauteur de tableau ?: ";
	std::cin >> hauteur;

	Init();

	InitPionsIA();
	ResetPionsIA(true);
	int nbPartiesIA = longueur * hauteur * 1000;

	bool tour = false; // false = C'est au tour du joueur 1; true = C'est au tour du joueur 2;
	int numeroPartie = 0;

	tableauCursor = getConsolePos();
	std::cout << "Veuillez patienter, simulation de " << nbPartiesIA << " parties..." << std::endl;
	while(true) { // Boucle des parties
		Reset();
		ResetPionsIA(false);
		tour = false;
		bool contreJoueur = numeroPartie >= nbPartiesIA;

		if(contreJoueur) {
			clearConsole(tableauCursor);
		}


		char vic = ' ';
		while(true) { // Boucle des tours
			Point pionPos = {0, 0};
			Point attaquePos = {0, 0};

			if(!contreJoueur || tour) {
				ObtenirTourIA(pionPos, attaquePos, tour, contreJoueur);
			}
			else {
				ObtenirTour(pionPos, attaquePos, tour);
			}
			Joue(pionPos, attaquePos);

			vic = Victoire();
			if(vic != ' ') break;

			tour = !tour;
		}
		if(contreJoueur) {
			Afficher({-1, -1}, {-1, -1}); // On passe -1;-1 pour les deux coordonnées pour n'afficher rien en vert ni en surligné

			if(vic != '-')
				std::cout << "Les " << vic << " gagnent !";
			else 
				std::cout << "Égalité !";

			while(isKeyHeld(VK_SPACE));
			while(!isKeyHeld(VK_SPACE));
		}

		numeroPartie++;
	}


	return 0;
}