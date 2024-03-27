#include "common.h"
#include <vector>
#include <ctime>


/**********************/
/* Variables globales */
/**********************/

// Liste des sockets et des pseudonymes, avec le nombre d'utilisateurs
std::vector<SOCKET> sockets;
std::vector<std::string> Nom_Id;
unsigned int nbUsers = 0;

// Message en cours d'écriture par le serveur, et booléen d'interruption si son écriture doit être pausée
std::string message;
bool interrupt = false;

// Fichier pour la sauvegarde des logs
FILE *logFP = NULL;


/*************************/
/* Fonctions utilitaires */
/*************************/

// Fonction permettant d'afficher l'heure avant un printf, et de sauvegarder dans un fichier tout ce qui est loggé
int log(const char *fmt, ...) {
	printf("\r");
    char msg[1024];

	// Affichage & sauvegarde du temps
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
	sprintf(msg, "[%02d:%02d:%02d]: ", now->tm_hour, now->tm_min, now->tm_sec);
	printf("%s", msg);
	if(logFP)
		fprintf(logFP, "%s", msg);

	// Affichage & sauvegarde du contenu
    va_list args;
    va_start(args, fmt);
    int rc = vsprintf(msg, fmt, args);
    va_end(args);
    printf("%s", msg);
	if(logFP)
    	fprintf(logFP, "%s", msg);

    // Suppression du reste de la ligne si présent et interruption de toute saisie en cours
	ClearEndOfLine();
	interrupt = true;

	// Valeur de retour du sprintf de base
    return rc;
}


// Fonction envoyant un message à tous les clients sauf celui dont le socket est passé en paramètre (mettre à 0 pour envoyer à tout le monde)
void broadcastAll(const char* str, SOCKET except) {
	int nbSockets = sockets.size();

	for(int i = 0; i < nbSockets; i++) {
		if(sockets[i] != except) {
			int ecrits = send(sockets[i], str, strlen(str)+1, 0);
			gererErreurs(ecrits, sockets[i], "envoi", true, false);
		}
	}

	log("%s\n", str);
}


// Fait quitter l'utilisateur dont le socket est passé en paramètre
void leave(SOCKET socket) {
	for(unsigned int i = 0; i < nbUsers; i++) {
		if(sockets[i] == socket) {
			sockets.erase(sockets.begin() + i);
			Nom_Id.erase(Nom_Id.begin() + i);
			nbUsers--;
			gererErreurs(closesocket(socket), socket, "fermeture", true, false);
			break;
		}
	}
}


/******************/
/* Thread d'envoi */
/******************/

// Gère la saisie et l'envoi de messages par le serveur à tout le monde
void* ThreadEnvoi(void *ptr) {
	while (true) {
		message = "";

		// Demande de texte de manière à permettre l'interruption et la reprise
		do {
			printf("[SERVEUR]: ");
			interrupt = false;
			fakeCin(message, &interrupt, false);
		} while(interrupt);

		// Ajout du préfixe à l'intérieur du fichier
		char messageGlobal[1024];
		sprintf(messageGlobal, "[SERVEUR]: %s", message.data());
		
		// Envoi du message saisi à tous les clients
		broadcastAll(messageGlobal, 0);
	}
	
	return NULL;
}


/***************************/
/* Thread de l'utilisateur */
/***************************/

// Prends en paramètre le pointeur vers l'index de l'utilisateur dans le tableau de sockets
void* ThreadUser(void* ptr) {
	// Récupération de l'index
	unsigned int* userIDPtr = (unsigned int*)ptr;
	unsigned int uID = *userIDPtr;
	delete userIDPtr;

	// Récupéation du socket et du pseudo localement pour ne plus dépendre de l'index qui risque de changer
	SOCKET socketDialogue = sockets[uID];
	std::string pseudoStr = Nom_Id[uID];
	const char* pseudo = pseudoStr.data();

	// Boucle principale - Récupère les messages de l'utilisateur et les envoie aux autres
	while(true) {
		char messageGlobal[1026];
		bool br = false;

		char messageRecu[1024];
		int lus = recv(socketDialogue, messageRecu, sizeof(messageRecu), 0);
		if (lus > 0) {
			sprintf(messageGlobal, "%s: %s", pseudo, messageRecu);
		}
		else if (lus == 0) { // Le socket est fermé par le client
			sprintf(messageGlobal, "<- %s est partit.", pseudo);
			br = true;
		}
		else { /* une erreur ! */
			sprintf(messageGlobal, "<- %s a été déconnecté.", pseudo);
			br = true;
		}

		broadcastAll(messageGlobal, socketDialogue);
		if(br)
			break;
		
		char rep = 0;
		int ecrits = send(socketDialogue, &rep, 1, 0);
		gererErreurs(ecrits, socketDialogue, "envoi", true, false);
	}

	// Si la boucle a été cassée, fermeture du socket de l'utilisateur
	leave(socketDialogue);

	return NULL;
}


/********************/
/* Thread principal */
/********************/

// Initialise le serveur et gère les demandes de connexion
int main(int argc, char** argv)
{
	initApp();
	int iResult;

	bool noLogs = (argc == 2) && (strcmp(argv[1], "--no-logs") == 0);
	if(!noLogs) {
		char logFName[24];
	    std::time_t t = std::time(0);
	    std::tm* now = std::localtime(&t);
		sprintf(logFName, "%04d-%02d-%02d %02d-%02d-%02d.log", 1900+now->tm_year, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
		logFP = fopen(logFName, "w");
	}


	/****************************************************/
	/* Initialisation du socket d'écoute et attachement */
	/****************************************************/
	// On créée le socket socketEcoute et on le relie à notre réseau

	// Création d'un socket de communication
	SOCKET socketEcoute = socket(AF_INET, SOCK_STREAM, 0); // 0 indique que l’on utilisera le protocole par défaut associé à SOCK_STREAM soit TCP
	gererErreurs(socketEcoute, socketEcoute, "creation", false, true);

	// On prépare l’adresse d’attachement locale
	struct sockaddr_in pointDeRencontreLocal;

	// Renseigne la structure sockaddr_in avec les informations locales du serveur
	pointDeRencontreLocal.sin_family = AF_INET;
	pointDeRencontreLocal.sin_addr.s_addr = htonl(INADDR_ANY); // interfaces locales acceptées, toutes les interfaces locales disponibles
	pointDeRencontreLocal.sin_port = htons(5000); // Numéro de port d’écoute du serveur

	// Attachement du socket créé et du point de rencontre local
	iResult = bind(socketEcoute, (SOCKADDR *)&pointDeRencontreLocal, sizeof(pointDeRencontreLocal));
	gererErreurs(iResult, socketEcoute, "bind", true, true);

	// Le socket est créé et attaché
	log("Socket attaché avec succès !\n");


	/******************************/
	/* Estimation de l'adresse IP */
	/******************************/
	// On tente d'estimer l'IP du serveur à partager
	// Celle-ci peut être inexacte si plusieurs périphériques réseau sont branchés (d'où "potentielle", "estimée")

	unsigned char ip[4];
	if(getMyIP(ip))
		log("Adresse IP potentielle du serveur: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
	else
		log("L'adresse IP n'a pas pu être estimée.\n");


	/************************************/
	/* Passage en mode d'écoute passive */
	/************************************/
	// On passe en mode d'écoute passive sur socketEcoute pour attendre des demandes de connexion

	// On fixe la taille de la file d’attente (pour les demandes de connexion non encore traitées)
	iResult = listen(socketEcoute, SOMAXCONN);
	gererErreurs(iResult, socketEcoute, "listen", true, true);

	log("Socket placé en écoute passive...\n");


	/*********************************/
	/* Création du thread de d'envoi */
	/*********************************/

	pthread_t tEnvoi;
	pthread_create(&tEnvoi, NULL, &ThreadEnvoi, NULL);


	/****************************************/
	/* Traitement des demandes de connexion */
	/****************************************/

	// Boucle d’attente de demandes de connexion: en théorie, celle-ci dure indéfiniement
	while(true)
	{
		/************************/
		/* Attente de connexion */
		/************************/

		struct sockaddr_in pointDeRencontreDistant;
		socklen_t longueurAdresse = sizeof(pointDeRencontreDistant);

		SOCKET socketDialogue = accept(socketEcoute, (SOCKADDR*)&pointDeRencontreDistant, &longueurAdresse); // c’est un appel bloquant
		gererErreurs(socketDialogue, socketDialogue, "accept", true, true);

		char *dIP = inet_ntoa(pointDeRencontreDistant.sin_addr);
		log("Tentative de connexion de %d.%d.%d.%d (socket %X)\n", dIP[0], dIP[1], dIP[2], dIP[3], (int)socketDialogue);

		/************************************/
		/* Demande reçue, attente du pseudo */
		/************************************/

		char idRecu[1024];
		// Boucle de demande de pseudonyme, itère jusqu'à ce qu'un pseudonyme valide soit reçu
		while(true) {
			// Réception du pseudo
			int lu = recv(socketDialogue, idRecu, sizeof(idRecu), 0);

			if(lu > 0) { // Réception d'octets
				int verif = 0;

				// On parcourt le tableau d'identifiants déjà connectés afin de voir si l'identifiant reçu est déjà utilisé
				int nb_id = Nom_Id.size();
				for(int b = 0; b < nb_id; b++) {
					if(strcmp(Nom_Id[b].data(), idRecu) == 0) {
						verif = 2;
						break;
					}
				}

				// On vérifie la validité du pseudo
				if(verif == 0) {
					// Seulement des caractères alphanumériques
					for(int a = 0; a < lu-1; a++) {
						if (!((idRecu[a] >= 'a' && idRecu[a] <= 'z') || (idRecu[a] >= 'A' && idRecu[a] <= 'Z') || (idRecu[a] >= '0' && idRecu[a] <= '9'))) {
							verif = 1;
							break;
						}
					}

					// Caractère nul de terminaison
					if(idRecu[lu-1] != 0)
						verif = 1;
				}

				char valeurEnvoi = verif;
				int ecrits = send(socketDialogue, &valeurEnvoi, 1, 0); // envoi du code d'erreur: 0 si ok, 1 si invalide, 3 si déjà pris
				if(!gererErreurs(ecrits, socketDialogue, "envoi", true, true)) {					
					if(verif == 0) { // Si pseudo valide
						Nom_Id.push_back(idRecu); // Stock de l'identifiant du client
						sockets.push_back(socketDialogue); // Stock du socket de communication avec le client

						// Allocation d'un emplacement pour passer l'index du pseudo
						unsigned int* uID = new unsigned int;
						*uID = nbUsers;
						nbUsers++;

						// Affichage du message de connection auprès de tous les autres clients
						char messageGlobal[1044];
						sprintf(messageGlobal, "-> %s s'est connecté.", idRecu);
						broadcastAll(messageGlobal, socketDialogue);

						// Création d'un thread pour le nouvel utilisateur
						pthread_t tUser; // Sera out of scope après la fin de la fonction, mais non dérangeant puisque celui-ci n'est pas censé se terminer autrement que par lui-même
						pthread_create(&tUser, NULL, &ThreadUser, uID);

						// Tout est bon, on peut casser la boucle
						break;
					}
					else { // Si pseudo incorrect
						log("%d.%d.%d.%d (socket %X) a tenté le pseudo \"%s\" -> erreur %d\n", dIP[0], dIP[1], dIP[2], dIP[3], (int)socketDialogue, idRecu, verif);
					}
				}
			}
			else if (lu == 0)
				printf("le socket a été fermé par le client !\n");
			else
				gererErreurs(lu, socketDialogue, "lecture", true, true);
		}
	}


	/*************/
	/* Fermeture */
	/*************/

	// On ferme le thread d'envoi
	pthread_cancel(tEnvoi);

	// On ferme le socket d'écoute avant de quitter
	iResult = closesocket(socketEcoute);
	gererErreurs(iResult, socketEcoute, "fermeture", false, true);

	quitApp(0);

	return 0;
}