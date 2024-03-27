#pragma once

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

#include "common.h" // Typedefs, defines, structures et variables communes
#include "io.h" // Inclus <windows.h>


class morpion {
public:
	int main();
	void resetVars();
	void keyPresses();
	bool isVictory();
	void renderAll();
};