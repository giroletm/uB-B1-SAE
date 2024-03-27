#pragma once

#include "common.h"

byte* loadFileAsByteArray(const char* filename, uint *fileSize);
void saveFileToByteArray(const char* filename, byte *data, uint fileSize);
bool fileExists(const char *filename);