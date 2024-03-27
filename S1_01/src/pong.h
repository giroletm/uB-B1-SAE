#pragma once

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <math.h>

#include "common.h" // Typedefs, defines, structures et variables communes
#include "io.h" // Inclus <windows.h>
#include "highscores.h"


class pong {
public:
	int main();
	void resetVars();
	void keyPresses();
	void updatePosition(bool updateBall);
	void renderAll();

};