#include "common.h"

#if defined(WIN32) || defined(_WIN32) /* si vous êtes sous Windows */
#include <windows.h>
#endif

void initApp() {
	#if defined(WIN32) || defined(_WIN32) /* si vous êtes sous Windows */
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(850);

	system("cls");
	//HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleMode(hOutput, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

	WSADATA WSAData; // variable initialisée par WSAStartup
	WSAStartup(MAKEWORD(2,0), &WSAData); // indique la version utilisée, ici 2.0
	#endif
}

void quitApp(int errCode) {
	#if defined(WIN32) || defined(_WIN32) /* si vous êtes sous Windows */
	WSACleanup();
	#endif

	exit(errCode);
}

bool gererErreurs(int error, SOCKET descripteurSocket, const char* nomErreur, bool fermerSocket, bool quitterApp) {
	bool retVal = false;

	if ((SOCKET)error == INVALID_SOCKET || error == SOCKET_ERROR)
	{
		#if defined(WIN32) || defined(_WIN32) /* si vous êtes sous Windows */
		if(nomErreur == NULL)
			printf("Erreur socket : %d\n", WSAGetLastError());
		else 
			printf("Erreur %s socket : %d\n", nomErreur, WSAGetLastError());
		#elif defined (linux) /* si vous êtes sous Linux */
		if(nomErreur == NULL)
			perror("Erreur socket ");
		else {
			char outMsg[64];
			sprintf(outMsg, "Erreur %s socket ", nomErreur);
			perror(outMsg);
		}
		#endif

		if(fermerSocket) {
			gererErreurs(closesocket(descripteurSocket), descripteurSocket, "fermeture", false, false);
			retVal = true;
		}

		if(quitterApp) {
			quitApp(1);
			retVal = true;
		}
	}

	return retVal;
}

// https://stackoverflow.com/a/122225
bool getMyIP(unsigned char* ip)
{
	char szBuffer[1024];

	if(gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
	{
		return false;
	}

	struct hostent *host = gethostbyname(szBuffer);
	if(host == NULL)
	{
		return false;
	}

	ip[0] = ((unsigned char*)(host->h_addr))[0];
	ip[1] = ((unsigned char*)(host->h_addr))[1];
	ip[2] = ((unsigned char*)(host->h_addr))[2];
	ip[3] = ((unsigned char*)(host->h_addr))[3];

	return true;
}