#pragma once

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

#include "common.h" // Typedefs, defines, structures et variables communes
#include "io.h" // Inclus <windows.h>
#include "highscores.h"


class snake {
public:
	int main();
	void resetVars();
	void keyPresses();
	void placeApple();
	bool updatePosition();
	void renderAll();

};