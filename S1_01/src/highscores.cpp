#include "highscores.h"

extern const char* minigames[];
extern u8 minigameCount;

Highscores::Highscores() { // Appelé quand Highscores est alloué. Charge le fichier des scores
	this->loadFile();
}

Highscores::~Highscores() { // Appelé lors de la suppression de Highscores. Sauvegarde le fichier des scores et supprime le tableau retenant ses données
	this->saveFile();
	delete[] data;
}

void Highscores::loadFile() { // Charge un fichier ou le créée s'il n'existe pas
	u8 dtSize = minigameCount*sizeof(GameScore);
	if(fileExists("scores.bin")) {
		this->data = (GameScore*)loadFileAsByteArray("scores.bin", &this->dataSize); // Chargement du fichier

		for(int i = 0; i < minigameCount; i++) {
			this->data[i].name += (u32)this->data; // Correction des offsets vers les textes

			int txtLen = strlen(this->data[i].name);
			char *newText = new char[txtLen + 1];
			memcpy(newText, this->data[i].name, txtLen);
			newText[txtLen] = 0;

			this->data[i].name = newText; // Réallocation de chaque texte pour éviter des soucis lors de leur potentielle modification
		}
	}
	else {
		this->data = new GameScore[minigameCount]; // Nouveau tableau des scores

		for(int i = 0; i < dtSize; i++) // Mise à 0 de tous les octets du tableau tout juste alloué
			((u8*)this->data)[i] = 0;

		for(int i = 0; i < minigameCount; i++) { // Création de nouveaux textes (vides) pour faciliter le reste
			this->data[i].name = new char[1];
			this->data[i].name[0] = 0;
		}

		this->saveFile();
	}
}

void Highscores::saveFile() { // Sauvegarde du fichier des scores
	u32 savedSize = minigameCount*sizeof(GameScore); // On a d'abord besoin de la taille totale du fichier
	for(int i = 0; i < minigameCount; i++)
		savedSize += strlen(this->data[i].name) + 1;

	u8 *rawdata = new u8[savedSize]; // Allocation d'un tableau d'octets temporaire pour stocker le fichier à sortir

	savedSize = minigameCount*sizeof(GameScore);
	for(int i = 0; i < minigameCount; i++) { // Mise des éléments du tableau des scrores dans le tableau tout juste créé
		memcpy(&rawdata[savedSize], this->data[i].name, strlen(this->data[i].name) + 1);

		*(u32*)&rawdata[i * 8] = this->data[i].score;
		*(u32*)&rawdata[(i * 8) + 4] = (u32)savedSize;

		savedSize += strlen(this->data[i].name) + 1;
	}

	saveFileToByteArray("scores.bin", rawdata, savedSize); // Écriture

	delete[] rawdata; // Supression du tableau temporaire
}

void Highscores::showHighscores() {
	clearConsole();

	while(isKeyHeld(VK_RETURN));

	std::cout << "Meilleurs scores:" << std::endl;
	bool noScore = true;
	for(int i = 0; i < minigameCount; i++) {
		int score = (int)this->getHSScoreForMinigame(i);
		if(score > 0) {
			std::cout << i << ". " << minigames[i] << ": " << score << " par " << this->getHSNameForMinigame(i) << std::endl;
			noScore = false;
		}
	}

	if(noScore) {
		std::cout << "Aucun score enregistré.";
	}

	std::cout << std::endl << "Appuyez sur Entrée pour continuer...";

	while(!isKeyHeld(VK_RETURN));
}

u32 Highscores::getHSScoreForMinigame(u32 id) {
	if(id < minigameCount)
		return data[id].score;
	else return -1;
}

char* Highscores::getHSNameForMinigame(u32 id) {
	if(id < minigameCount)
		return data[id].name;
	else return 0;
}

void Highscores::setHSForMinigame(u32 id, u32 score) {
	if(id < minigameCount) {
		clearConsole();

		std::cout << "Nouveau meilleur score pour " << minigames[id] << " !" << std::endl << "Votre nom: ";
		std::string name;
		std::cin >> name;
		int nameLen = name.length();

		if(data[id].name) delete data[id].name;

		data[id].name = new char[nameLen + 1];
		for(int i = 0; i <= nameLen; i++) {
			if(i < nameLen)
				data[id].name[i] = name[i];
			else
				data[id].name[i] = 0;
		}

		clearConsole();

		data[id].score = score;

		this->saveFile();
	}
}