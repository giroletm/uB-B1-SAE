#pragma once

#include "common.h"

class Polygone;
class Point2D;
class Biome;
class Maillage;

class Map {
public:
	Map(const char* fichier);
	~Map();

	int biomeParDefaut;

	uint nbPolygones;
	Polygone** polygones;
	uint nbBiomes;
	Biome** biomes;

	enum Algorithme {
		DIJKSTRA = 0,
		ASTAR = 1
	};

	bool Invalide();
	void AfficherTout();
	Point2D* CalculerChemin(int* dstNbPoints, Point2D pt1, Point2D pt2, float nbMailleUnite, Algorithme algo);
	Biome* BiomeDePoint(float x, float y);

private:
	Maillage* maillage;
};

class Polygone {
public:
	int idxBiome;
	uint nbPoints;
	Point2D** points;
};

class Point2D {
public:
	float x;
	float y;
};

class Biome {
public:
	char* nom;
	uint couleur;
	float poidsEntrant;
	float poidsInterieur;
};
