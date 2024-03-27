#pragma once

#include "sockets.h"
#include "thread.h"
#include "console.h"
#include <stdio.h>
#include <cstring>

void initApp();
void quitApp(int errCode);
bool gererErreurs(int error, SOCKET descripteurSocket, const char* nomErreur, bool fermerSocket, bool quitterApp);
bool getMyIP(unsigned char* ip);

struct UserInfo {
	SOCKET socket;
	std::string pseudo;
};