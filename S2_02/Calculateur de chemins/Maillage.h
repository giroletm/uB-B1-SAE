#pragma once

#include "common.h"
#include "Map.h"

class Noeud : public Point2D {
public:
	Biome * biome;
};


class Arc {
public:
	Noeud * n1;
	Noeud * n2;
	float poids;
};

class Maillage {
public:
	Maillage(Map * map, float nbMailleUnite);
	~Maillage();

	float nbMailleUnite;

	int nbNoeuds;
	Noeud * noeuds;
	int nbArcs;
	Arc * arcs;

	int ObtenirIndexNoeudProche(Point2D pt);
	Noeud** CalculerCheminD(int numeroNoeudDepart, int numeroNoeudArrivee);
	Noeud** CalculerCheminA(int numeroNoeudDepart, int numeroNoeudArrivee);
	bool Invalide();
	
private:
	Noeud** CreerChemin(int numeroNoeudArrivee, int* pred);
	float CalculPoids(Noeud * n1, Noeud * n2);
	float CalculDistHeuristique(Noeud* n1, Noeud* n2);
	float CalculDistEuclidienne(Noeud* n1, Noeud* n2);
};
