#pragma once

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

#include "common.h" // Typedefs, defines, structures et variables communes
#include "io.h" // Inclus <windows.h>
#include "highscores.h"


class psquatre {
public:
	int main();

	void init(char tbl[6][7], int size,int size2);
	void affichertab();
	bool Victoire(char tbl[][7], int ligneSel, int colonneSel, char joueur);
};