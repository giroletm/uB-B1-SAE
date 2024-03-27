#include <iostream>
#include "Map.h"

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif


float AskForFloat(const char* str) {
	float f;
	std::cout << str;
	std::cin >> f;
	while(std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(256,'\n');
		std::cout << str;
		std::cin >> f;
	}

	return f;
}

float AskForInt(const char* str, int min, int max) {
	int i;
	std::cout << str;
	std::cin >> i;

	bool fail = std::cin.fail();
	while(fail || i > max || i < min) {
		if(fail) {
			std::cin.clear();
			std::cin.ignore(256,'\n');
		}
		else {
			std::cout << "Entrée invalide." << std::endl;
		}

		std::cout << str;
		std::cin >> i;
	}

	return i;
}

int main(int argc, char** argv) {
	#if defined(WIN32) || defined(_WIN32)
	SetConsoleOutputCP(CP_UTF8);
	#endif

	if(argc != 2) {
		printf("Usage:\n");
		printf("\t%s <fichier RSMG à lire>\n\n", argv[0]);
		
		return 0;
	}

	Map* map = new Map(argv[1]);
	if(map->Invalide()) {
		printf("Fichier inexistant ou invalide.\n");
		return 1;
	}

	map->AfficherTout();

	float nbMailleUnite = AskForFloat("Nombre de mailles par unité: ");
	int algo = AskForInt("Algorithme à utiliser (0=Dijkstra, 1=A*): ", 0, 1);

	float pt1X = AskForFloat("Coordonnée X du premier point: ");
	float pt1Y = AskForFloat("Coordonnée Y du premier point: ");
	float pt2X = AskForFloat("Coordonnée X du second point: ");
	float pt2Y = AskForFloat("Coordonnée Y du second point: ");

	printf("Calcul pour %.9g maille%s par unité avec l'algorithme \"%s\"...\n", nbMailleUnite, (nbMailleUnite != 1.0f) ? "s" : "", (algo == 0) ? "Dijkstra" : "A*");

	int nbPoints;
	Point2D* chemin = map->CalculerChemin(&nbPoints, {pt1X, pt1Y}, {pt2X, pt2Y}, nbMailleUnite, (Map::Algorithme)algo);

	printf("Chemin à suivre:\n");
	for(int i = 0; i < nbPoints; i++) {
		printf((i < nbPoints-1) ? "%.9g;%.9g -> " : "%.9g;%.9g\n", chemin[i].x, chemin[i].y);
	}
	
	delete[] chemin;
	delete map;

	return 0;
}