#include "io.h"

extern COORD mainScreenPos;

void clearConsole() {
	// return;
	clearConsole(mainScreenPos);
}

void clearConsole(COORD screenPos) { // Merci internet: https://learn.microsoft.com/en-us/windows/console/clearing-the-screen
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	// Get the number of character cells in the current buffer.
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}

	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// Fill the entire screen with blanks.
	if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
									(TCHAR)' ',      // Character to write to the buffer
									dwConSize,       // Number of cells to write
									screenPos,   // Coordinates of first cell
									&cCharsWritten)) // Receive number of characters written
	{
		return;
	}

	// Get the current text attribute.
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}

	// Set the buffer's attributes accordingly.
	if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
									csbi.wAttributes, // Character attributes to use
									dwConSize,        // Number of cells to set attribute
									screenPos,    // Coordinates of first cell
									&cCharsWritten))  // Receive number of characters written
	{
		return;
	}

	// Put the cursor at its home coordinates.
	SetConsoleCursorPosition(hConsole, screenPos);
}

void takeConsoleToPos(short chr, short line) {
	takeConsoleToPos((COORD){chr, line});
}

void takeConsoleToPos(COORD coords) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coords);
}

bool getConsolePos(COORD *screenPos) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
	{
		return false;
	}

	*screenPos = csbi.dwCursorPosition;
	return true;
}

bool askOrdi(COORD consS) {
	char j;
	std::cout << "Jouer contre l'ordi ? (O/N): ";
	std::cin >> j;
	while(std::cin.fail()) {
		std::cout << "Entrée invalide." << std::endl << "Jouer contre l'ordi ? (O/N): ";
		std::cin.clear();
		std::cin.ignore(256,'\n');
		std::cin >> j;
	}
	
	clearConsole(consS);
	return j == 'O' || j == 'o';
}


bool isKeyHeld(int key) {
	return GetKeyState(key) & 0x8000;
}


u8* loadFileAsByteArray(const char* filename, size_t *fileSize) { // Le morceau de code de base vient pas de moi mais je l'ai adapté comme ça m'arrangeait
	std::ifstream file;
	file.open(filename, std::ios_base::binary);
	if(!file.is_open())
	  return 0;

	file.seekg(0, std::ios::end);
	*fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	u8 *data = new u8[*fileSize];

	file.read(reinterpret_cast<char*>(data), *fileSize);

	file.close();

	return data;
}


void saveFileToByteArray(const char* filename, u8 *data, size_t fileSize) { // Le morceau de code de base vient pas de moi mais je l'ai adapté comme ça m'arrangeait
	std::ofstream file(filename, std::ios::binary);

	file.write(reinterpret_cast<char*>(data), fileSize);

	file.close();
}

bool fileExists(const char *filename) // https://stackoverflow.com/a/19841704
{
    std::ifstream infile(filename);
    return infile.good();
}