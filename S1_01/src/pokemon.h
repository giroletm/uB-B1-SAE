#pragma once

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

#include "common.h" // Typedefs, defines, structures et variables communes
#include "io.h" // Inclus <windows.h>
#include "highscores.h"


class pokemon {
public:
	int main();

	void choix_pokemon (int & a,int & b);
	void choix_ordi(int & a, int & b);
	void faire_choix (int a);
	void affichage_choix (int a);

	void choix_atk_plante(int & a);
	void choix_atk_feu(int & a);
	void choix_atk_eau(int & a);

	void coup_critique_et_esquive ( int & b, int & c);
	void avantage_type (int pok, int pokad, int & atk);

	void aff_dmg (int a, int b);

	void vive_attaque (int & a,int & b, int pok);
	void deflagration (int & a,int & b, int & c, int pok, int pokad);
	void flammeche (int & a,int & b, int pok, int pokad);
	void trancherb (int & a,int & b, int pok, int pokad);
	void vampigraine (int & a,int & b,int & c, int pok, int pokad);
	void vibraqua (int & a,int & b, int pok, int pokad);
	void hydroqueue (int & a,int & b, int pok);
};