#pragma once

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

#include "common.h" // Typedefs, defines, structures et variables communes
#include "io.h" // Inclus <windows.h>
#include "highscores.h"


class blackjack {
public:
	int main();

	void init (int a[], int size);
	void cartes (int a[]);
	void regles (int & choix);
	void fin (std::string prenom, int partie);
	void verification_as (int joueur[], int & somme, int tour);
	void distribution (int joueur[], int ordi[], int & choix, int & tour, int & somme, int & partie, std::string prenom, int & seuil);
	void demarrer (int joueur[],int ordi[], int & tour, int & choix, int & somme, int & partie, std::string prenom, int & seuil);
};