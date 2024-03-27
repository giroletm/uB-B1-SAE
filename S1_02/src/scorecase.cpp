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
void InitValeursIA(); // Alloue les tableaux de valeurs pour chaque pion
void ResetValeursIA(); // Réinitialise le tableau à son état d'origine
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

	// Si aucun pion n'est jouable, alors égalité
	bool jouable = false;
	for(int y = 0; y < hauteur; y++) {
		for(int x = 0; x < longueur; x++) {
			if(tableau[y][x] != ' ') {
				if(PionJouable({x, y}, tableau[y][x] == 'X')) {
					jouable = true;
					break;
				}
			}
		}
	}

	return jouable ? ' ' : '-';
}


/***************************/
/* Jeu - IA (Comportement) */
/***************************/

int** valeursO;
int** valeursX;

// Alloue les tableaux de valeurs pour chaque pion
void InitValeursIA() {
	valeursO = new int*[hauteur];
	valeursX = new int*[hauteur];
	for(int y = 0; y < hauteur; y++) {
		valeursO[y] = new int[longueur];
		valeursX[y] = new int[longueur];
	}
}

// Réinitialise le tableau à son état d'origine
void ResetValeursIA() {
	for(int y = 0; y < hauteur; y++) {
		for(int x = 0; x < longueur; x++) {
			valeursO[y][x] = 1;
			valeursX[y][x] = 1;
		}
	}
}

// Obtiens le coup que l'IA veux jouer
void ObtenirTourIA(Point &pionPos, Point &attaquePos, bool tour, bool contreJoueur) {
	if(contreJoueur)
		Afficher({-1, -1}, {-1, -1});


	// Obtention de chaque pion
	char monPion = (tour) ? 'X' : 'O';
	int** mesValeurs = (tour) ? valeursX : valeursO;

	std::vector<Point> pions;

	for(int y = 0; y < hauteur; y++) {
		for(int x = 0; x < longueur; x++) {
			if(tableau[y][x] == monPion) {
				pions.push_back({x, y});
			}
		}
	}

	// Obtention de chaque attaque
	int nombrePions = pions.size();
	std::vector<std::vector<Point>> attaques;

	int valeurTotale = 0;
	int nombreTotalAttaques = 0;
	int valTmp = -1;
	for(int i = 0; i < nombrePions; i++) {
		Point pionActuel = pions[i];
		std::vector<Point> atq;

		for(int y = 0; y < hauteur; y++) {
			for(int x = 0; x < longueur; x++) {
				if(CoupValide(pionActuel, {x, y}, tour)) {
					atq.push_back({x, y});

					valeurTotale += mesValeurs[y][x];
					nombreTotalAttaques++;
				}
			}
		}

		attaques.push_back(atq);
	}


	// Réécriture sous formes de possibilités et calcul de leur probabilité
	std::vector<Possibilite> possiblts;

	for(int i = 0; i < nombrePions; i++) {

		int nombreAttaques = attaques[i].size();
		for(int j = 0; j < nombreAttaques; j++) {
			Point attaqueActuelle = attaques[i][j];
			int valeur = mesValeurs[attaqueActuelle.y][attaqueActuelle.x];

			double chance = 0.0;
			if(valeurTotale == 0)
				chance = 100.0 / (double)nombreTotalAttaques;
			else
				chance = valeur * 100.0 / valeurTotale;

			possiblts.push_back({pions[i], attaqueActuelle, chance});
		}
	}


	// Calcul de la probabilité choisie à partir d'un nombre aléatoire
	double f = (double)rand() * 100.0f / RAND_MAX;

	Possibilite possActuelle;

	double chanceAvant = 0.0;
	int nbPossibilites = possiblts.size();
	for(int i = 0; i < nbPossibilites; i++) {
		possActuelle = possiblts[i];

		if(f >= chanceAvant && f < (chanceAvant + possActuelle.chance)) {
			break;
		}

		chanceAvant += possActuelle.chance;
	}

	// Sélection et incrémentation de la valeur de la case si nécéssaire
	pionPos = possActuelle.pion;
	attaquePos = possActuelle.attaque;

	if(tableau[attaquePos.y][attaquePos.x] != ' ') {
		mesValeurs[attaquePos.y][attaquePos.x]++;
	}


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

	InitValeursIA();
	ResetValeursIA();
	int nbPartiesIA = longueur * hauteur * 1000;

	bool tour = false; // false = C'est au tour du joueur 1; true = C'est au tour du joueur 2;
	int numeroPartie = 0;

	tableauCursor = getConsolePos();
	std::cout << "Veuillez patienter, simulation de " << nbPartiesIA << " parties..." << std::endl;
	while(true) { // Boucle des parties
		Reset();
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