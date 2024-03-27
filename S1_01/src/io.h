#pragma once

#include <windows.h>
#include <fstream>

#include "common.h"

void clearConsole();
void clearConsole(COORD screenPos);
void takeConsoleToPos(short chr, short line);
void takeConsoleToPos(COORD coords);
bool getConsolePos(COORD *screenPos);
bool askOrdi(COORD consS);

bool isKeyHeld(int key);
bool isKeyUp(int key);

u8* loadFileAsByteArray(const char* filename, size_t *fileSize);
void saveFileToByteArray(const char* filename, u8 *data, size_t fileSize);
bool fileExists(const char *filename);