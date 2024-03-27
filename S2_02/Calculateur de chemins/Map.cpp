#include "Map.h"
#include "Maillage.h"
#include "io.h"

#include <iostream>
#include <limits>
#include <math.h>


/*********************************/
/* Fonctions de lecture d'octets */
/*********************************/

inline long GetInt64(void* ptr, int index) {
	return *(long*)((byte*)ptr + index);
}

inline ulong GetUInt64(void* ptr, int index) {
	return *(ulong*)((byte*)ptr + index);
}

inline int GetInt32(void* ptr, int index) {
	return *(int*)((byte*)ptr + index);	
}

inline uint GetUInt32(void* ptr, int index) {
	return *(uint*)((byte*)ptr + index);
}

inline float GetFloat(void* ptr, int index) {
	return *(float*)((byte*)ptr + index);
}


/****************/
/***** Map ******/
/* Constructeur */
/****************/

/// Construit un objet Map à partir d'un fichier
Map::Map(const char* fichier) {
	// Initialisation des valeurs par défaut
	this->biomeParDefaut = -1;
	this->nbPolygones = 0;
	this->polygones = NULL;
	this->nbBiomes = 0;
	this->biomes = NULL;

	this->maillage = NULL;

	// Vérification de l'existence du fichier
	if(!fileExists(fichier))
		return;

	// Chargement du fichier et vérification du succès
	byte* data = loadFileAsByteArray(fichier, NULL);
	if(!data)
		return;

	// Vérification des quatres premiers octets
	if(*((uint*)data) != 0x4250414D) {
		delete[] data;
		return;
	}

	// Le fichier semble valide, lecture de celui-ci

	this->biomeParDefaut = GetInt32(data, 4);

	byte* polyList = data + GetUInt64(data, 8);
	this->nbPolygones = GetUInt32(polyList, 0);

	byte* biomeList = data + GetUInt64(data, 16);
	this->nbBiomes = GetUInt32(biomeList, 0);

	this->polygones = new Polygone*[this->nbPolygones];
	this->biomes = new Biome*[this->nbBiomes];

	for(uint i = 0; i < this->nbPolygones; i++) {
		byte* poly = data + GetUInt64(polyList, 4 + (i * 8));

		uint nbPts = GetUInt32(poly, 4);

		this->polygones[i] = new Polygone;
		this->polygones[i]->idxBiome = GetUInt32(poly, 0);
		this->polygones[i]->nbPoints = nbPts;
		this->polygones[i]->points = new Point2D*[nbPts];

		for(uint j = 0; j < nbPts; j++) {
			byte* pt = data + GetUInt64(poly, 8 + (j * 8));

			this->polygones[i]->points[j] = new Point2D;
			this->polygones[i]->points[j]->x = GetFloat(pt, 0);
			this->polygones[i]->points[j]->y = GetFloat(pt, 4);
		}
	}

	for(uint i = 0; i < this->nbBiomes; i++) {
		byte* biome = data + GetUInt64(biomeList, 4 + (i * 8));

		this->biomes[i] = new Biome;

		char* biomeName = (char*)(data + GetUInt64(biome, 0));
		uint lenName = 0;
		while(biomeName[lenName] != 0)
			lenName++;
		this->biomes[i]->nom = new char[lenName+1];
		for(uint j = 0; j < lenName; j++) {
			this->biomes[i]->nom[j] = biomeName[j];
		}
		this->biomes[i]->nom[lenName] = 0;
		this->biomes[i]->couleur = (biome[8] << 24) | (biome[9] << 16) | (biome[10] << 8) | (biome[11]);
		this->biomes[i]->poidsEntrant = GetFloat(biome, 12);
		this->biomes[i]->poidsInterieur = GetFloat(biome, 16);
	}

	delete[] data;
}


/***************/
/***** Map *****/
/* Destructeur */
/***************/

/// Détruit un objet Map
Map::~Map() {
	for(uint i = 0; i < this->nbPolygones; i++) {
		for(uint j = 0; j < this->polygones[i]->nbPoints; j++) {
			delete this->polygones[i]->points[j];
		}

		delete[] this->polygones[i]->points;
		delete this->polygones[i];
	}

	delete[] this->polygones;

	for(uint i = 0; i < this->nbBiomes; i++) {
		delete[] this->biomes[i]->nom;
		delete this->biomes[i];
	}

	delete[] this->biomes;
}


/************/
/*** Map ****/
/* Invalide */
/************/

/// Renvoie true si la map chargée est invalide
bool Map::Invalide() {
	return this->nbPolygones == 0 || this->nbBiomes == 0 || this->polygones == NULL || this->biomes == NULL;
}


/****************/
/***** Map ******/
/* AfficherTout */
/****************/

/// Affiche dans la console l'intégralité de ce que contient la map
void Map::AfficherTout() {
	if(this->Invalide()) {
		printf("Map invalide\n");
		return;
	}

	printf("Map:\n");
	printf("- Biome par défaut: %d\n", this->biomeParDefaut);

	printf("- Polygones:\n");
	printf("  - Nombre de polygones: %d\n", this->nbPolygones);
	for(uint i = 0; i < this->nbPolygones; i++) {
		printf("  - Polygone %d:\n", i);
		printf("    - Biome: %d\n", this->polygones[i]->idxBiome);
		printf("    - Points:\n");
		printf("      - Nombre de points: %d\n", this->polygones[i]->nbPoints);
		for(uint j = 0; j < this->polygones[i]->nbPoints; j++) {
			printf("      - Point %d:\n", j);
			printf("        - X: %f\n", this->polygones[i]->points[j]->x);
			printf("        - Y: %f\n", this->polygones[i]->points[j]->y);
		}
	}

	printf("- Biomes:\n");
	printf("  - Nombre de biomes: %d\n", this->nbBiomes);
	for(uint i = 0; i < this->nbBiomes; i++) {
		printf("  - Biome %d:\n", i);
		printf("    - Nom: %s\n", this->biomes[i]->nom);
		printf("    - Couleur: #%08X\n", this->biomes[i]->couleur);
		printf("    - Poids entrant: %f\n", this->biomes[i]->poidsEntrant);
		printf("    - Poids intérieur: %f\n", this->biomes[i]->poidsInterieur);
	}
}


/******************/
/****** Map *******/
/* CalculerChemin */
/******************/

/// Renvoie un tableau de points reliant le point pt1 au point pt2 calculé sur un maillage de nbMailleUnite mailles par unité grâce à l'algorithme algo. Le nombre de points du tableau sera stocké dans dstNbPoints
Point2D* Map::CalculerChemin(int* dstNbPoints, Point2D pt1, Point2D pt2, float nbMailleUnite, Algorithme algo) {
	// Si aucun pointeur pour stocker le nombre de points n'a été passé, on ne renvoie rien
	if(!dstNbPoints)
		return NULL;

	// Si un maillage d'un nombre de mailles par unité différent existe, destruction de celui-ci
	if(this->maillage && this->maillage->nbMailleUnite != nbMailleUnite) {
		delete this->maillage;
		this->maillage = NULL;
	}

	// Si aucun maillage ou aucun du même nombre de mailles par unité, création d'un nouveau
	if(!this->maillage)
		this->maillage = new Maillage(this, nbMailleUnite);

	// Obtention des numéros des noeuds les plus proches des coordonnées passées
	int depID = this->maillage->ObtenirIndexNoeudProche(pt1);
	int arrID = this->maillage->ObtenirIndexNoeudProche(pt2);

	// Calcul du chemin selon l'algorithme passé
	Noeud** chemin = NULL;
	switch(algo) {
		default:
		case DIJKSTRA:
		{
			chemin = maillage->CalculerCheminD(depID, arrID);
			break;
		}
		case ASTAR:
		{
			chemin = maillage->CalculerCheminA(depID, arrID);
			break;
		}
	}

	// Si aucun chemin retourné, on ne renvoie rien
	if(!chemin)
		return NULL;

	// Booléens permettant d'identifier si le point de départ et d'arrivée spécifiés sont des points du maillage (afin d'éviter les doublons)
	bool doDep = this->maillage->noeuds[depID].x != pt1.x || this->maillage->noeuds[depID].y != pt1.y;
	bool doArr = this->maillage->noeuds[arrID].x != pt2.x || this->maillage->noeuds[arrID].y != pt2.y;
	
	// Calcul de la taille totale du tableau de points à retourner
	int taille = 0;
	while(chemin[taille] != NULL) {
		taille++;
	}

	// Allocation du tableau de points à retourner et définition des éléments de départ et d'arrivé si possible
	int totaille = taille+doDep+doArr;
	Point2D* pts = new Point2D[totaille];
	if(doDep)
		pts[0] = pt1;
	if(doArr)
		pts[taille+1] = pt2;

	// Copie des coordonnées de chaque noeud dans le tableau de points
	for(int i = 0; i < taille; i++) {
		pts[i+doDep].x = chemin[i]->x;
		pts[i+doDep].y = chemin[i]->y;
	}

	// Assignation du nombre de points du tableau au pointeur donné en paramètres
	*dstNbPoints = totaille;

	// Destruction du chemin calculé alloué par l'algorithme utilisé
	delete[] chemin;

	// Retour du tableau de points
	return pts;
}


/****************/
/***** Map ******/
/* BiomeDePoint */
/****************/
// Implémentation originale de http://www.jeffreythompson.org/collision-detection/poly-point.php

/// Renvoie le biome dans lequel se trouve le point de coordonnées x;y
Biome* Map::BiomeDePoint(float x, float y) {
	Biome* biomePoint = NULL;

	// Itération à travers chaque polygone
	for(int nbPoly = 0; nbPoly < this->nbPolygones; nbPoly++) {
		Polygone* polyActuel = this->polygones[nbPoly];

		bool collision = false;

		// Itération à travers chaque sommet du polygone
		int next = 0;
		for (int current = 0; current < polyActuel->nbPoints; current++)
		{
			// Obtention du sommet suivant, si on est sur le dernier on prend le premier sommet
			next = current + 1;
			if (next == polyActuel->nbPoints) next = 0;

			// Sauvegarde du point actuel et du suivant afin de simplier notre if
			Point2D * vc = polyActuel->points[current];    // c pour "current" ("actuel")
			Point2D * vn = polyActuel->points[next];       // n pour "next" ("suivant")

			// Comparaison des positions: si le point a passé une ligne, on inverse le booléen de collision
			if (((vc->y >= y && vn->y < y) || (vc->y < y && vn->y >= y)) &&
				(x < (vn->x - vc->x) * (y - vc->y) / (vn->y - vc->y) + vc->x))
			{
				collision = !collision;
			}
		}

		// S'il y a eu collision avec le polygone en cours de traitement (= le point est dedans), le biome à retourner sera celui qui lui correspond
		if(collision)
			biomePoint = this->biomes[polyActuel->idxBiome];

		// On continue d'itérer au cas où des biomes se superposent: le biome le plus récent (= index le plus gros) a la priorité
	}

	// Si aucun biome n'a été trouver, alors on retournera le biome par défaut
	if(biomePoint == NULL)
		biomePoint = this->biomes[this->biomeParDefaut];

	// Retour du biome sélectionné
	return biomePoint;
}