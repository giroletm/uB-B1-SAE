#include <fstream>

#include "io.h"


/***********************/
/* loadFileAsByteArray */
/***********************/
// Le morceau de code de base vient pas de nous mais nous l'avons adapté comme cela nous arrangeait

/// Charge un fichier retourne son contenu sous la forme d'un tableau d'octets, et écrit la taille de celui-ci dans la variable pointée par fileSize
byte* loadFileAsByteArray(const char* filename, uint *fileSize) { 
	std::ifstream file;
	file.open(filename, std::ios_base::binary);
	if(!file.is_open())
	  return 0;

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);

	byte *data = new byte[size];

	file.read(reinterpret_cast<char*>(data), size);

	if(fileSize)
		*fileSize = size;

	file.close();

	return data;
}


/***********************/
/* saveFileToByteArray */
/***********************/
// Le morceau de code de base vient pas de nous mais nous l'avons adapté comme cela nous arrangeait

/// Sauvegarde un tableau d'octets dans un fichier de nom donné
void saveFileToByteArray(const char* filename, byte *data, uint fileSize) {
	std::ofstream file(filename, std::ios::binary);

	file.write(reinterpret_cast<char*>(data), fileSize);

	file.close();
}


/**************/
/* fileExists */
/**************/
// https://stackoverflow.com/a/19841704

/// Retourne true si le fichier de nom ou chemin spécifié existe
bool fileExists(const char *filename)
{
    std::ifstream infile(filename);
    return infile.good();
}