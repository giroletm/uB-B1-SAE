#include "common.h"
#include <string>
#include <iostream>


/**********************/
/* Variables globales */
/**********************/

// Socket du serveur et pseudo de l'utilisateur
SOCKET socketSrv;
std::string pseudoStr;

// Message reçu, sa taille et s'il est en attente d'être traité
char messageRecu[1024];
int lus = 0;
bool queued = false;

// Message en cours d'écriture par l'utilisateur, et booléen d'interruption si son écriture doit être pausée
std::string message;
bool interrupt = false;


/******************/
/* Thread d'envoi */
/******************/

// Gère la saisie et l'envoi de messages par l'utilisateur
void* ThreadEnvoi(void *ptr) {
	while (true) {
		message = "";

		// Demande de texte de manière à permettre l'interruption et la reprise
		do {
			printf("%s: ", pseudoStr.data());
			interrupt = false;
			fakeCin(message, &interrupt);
		} while(interrupt);
		
		// Envoi du message saisi
		const char* messageEnvoi = message.data();
		int ecrits = send(socketSrv, messageEnvoi, (int)strlen(messageEnvoi)+1, 0);
		gererErreurs(ecrits, socketSrv, "envoi", true, true);

		// Attente de la réception d'un message mis en attente
		while(!queued) {}
		
		// Si le message en attente reçu contient des octets, alors traitement en tant que code d'erreur: 0 si ok, 1 si erreur
		if (lus > 0) {
			if(messageRecu[0] != 0)
				printf("Erreur d'envoi\n");
		}
		else if (lus == 0) // le socket est fermé par le serveur
			printf("le socket a été fermé par le serveur !\n");
		else {
			printf("\r");
			ClearEndOfLine();
			gererErreurs(lus, socketSrv, "reception", true, true);
		}
		
		// Le message est traité
		queued = false;
	}
	
	return NULL;
}


/********************/
/* Thread principal */    
/********************/

// Initialise le client et gère la réception de messages
int main()
{
	initApp();
	int iResult;


	/*******************************************************************/
	/* Initialisation d'un socket et connection de celui-ci au serveur */
	/*******************************************************************/
	// On créée le socket socketSrv et on le connecte au serveur

	// Création d'un socket de communication
	socketSrv = socket(AF_INET, SOCK_STREAM, 0); /* 0 indique que l’on utilisera le protocole par défaut associé à SOCK_STREAM soit TCP */
	gererErreurs(socketSrv, socketSrv, "creation", false, true);

	// On prépare l’adresse d’attachement  distante
	struct sockaddr_in pointDeRencontreDistant; // ou SOCKADDR_IN pointDeRencontreDistant;

	// Demande de l'adresse IP
	std::string ipStr;
	std::cout << "IP du serveur: ";
	std::cin >> ipStr;

	if(ipStr.length() == 0 || ipStr == "localhost")
		ipStr = "127.0.0.1";

	// Renseigne la structure sockaddr_in avec les informations du serveur distant
	pointDeRencontreDistant.sin_family = AF_INET;
	pointDeRencontreDistant.sin_addr.s_addr = inet_addr(ipStr.data()); // IP du serveur
	pointDeRencontreDistant.sin_port = htons(5000); // Numéro de port d’écoute du serveur

	// Début de la connexion vers le processus serveur distant
	iResult = connect(socketSrv, (SOCKADDR *)&pointDeRencontreDistant, sizeof(pointDeRencontreDistant));
	gererErreurs(iResult, socketSrv, "connexion", true, true);

	printf("Connexion au serveur réussie avec succès !\n");


	/******************/
	/* Identification */
	/******************/
	
	while (true) {
		// Demande du pseudo
		printf("Votre pseudo: ");
		pseudoStr = "";
		fakeCin(pseudoStr, NULL);
		
		// Envoi du pseudo écrit
		const char* pseudoAtt = pseudoStr.data();
		int ecrits = send(socketSrv, pseudoAtt, (int)strlen(pseudoAtt)+1, 0);
		gererErreurs(ecrits, socketSrv, "envoi", true, true);
		
		// Vérification de la réponse du serveur
		char errCode;
		int lus = recv(socketSrv, &errCode, 1, 0);
		if (lus > 0) { // octet reçu (normalement un seul), valeur à: 0 si ok, 1 si invalide, 2 si déjà pris
			if(errCode == 0)
				break;
			else if (errCode == 1)
				printf("Pseudo invalide\n");
			else if (errCode == 2)
				printf("Pseudo déjà pris\n");
			else
				printf("Erreur non supportée\n");
		}
		else if (lus == 0) { // Le socket est fermé par le serveur
			printf("le socket a été fermé par le serveur !\n");
			break;
		}
		else
			gererErreurs(lus, socketSrv, "reception", true, true);
	}
	

	/*******************************/
	/* Réception et Thread d'envoi */
	/*******************************/
	
	// Creation du thread d'envoi
	pthread_t tEnvoi;
	pthread_create(&tEnvoi, NULL, &ThreadEnvoi, NULL);
	
	// Boucle de réception des messages du serveur
	while(true) {
		lus = recv(socketSrv, messageRecu, sizeof(messageRecu), 0);
		if (lus > 1) { // Si plus d'un octet reçu, alors nettoyage de la ligne, affichage du message et interruption de toute écriture
			printf("\r%s", messageRecu);
			ClearEndOfLine();
			printf("\n");
			interrupt = true;
		}
		else if (lus == 1) // Si un seul octet reçu, alors mise en attente car c'est un code d'erreur à destination du thread d'envoi (un message fait au moins deux octets: un caractère et le caractère nul de terminaison)
			queued = true;
		else if (lus == 0) { // Le socket est fermé par le serveur
			printf("le socket a été fermé par le serveur !\n");
			break;
		}
		else {
			printf("\r");
			ClearEndOfLine();
			gererErreurs(lus, socketSrv, "reception", true, true);
		}
	}
	
	pthread_cancel(tEnvoi);


	/*************/
	/* Fermeture */
	/*************/

	// On ferme le socket lié au serveur avant de quitter
	iResult = closesocket(socketSrv);
	gererErreurs(iResult, socketSrv, "fermeture", false, true);

	quitApp(0);

	return 0;
}