#pragma once

#if defined(_WIN32)

#include <fstream>
#include <iostream>
#include <sstream>

#elif defined(__ANDROID__)

#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>
#include <SDL_system.h>
#include <SDL_filesystem.h>

#elif defined(__SWITCH__)

#include <SDL_system.h>
#include <SDL_filesystem.h>

#endif


bool FileExists(const char* name);
int LoadFileAndAllocate(const char* name, char** dstPtr);

bool InternalFileExists(const char* name);
int LoadInternalFileAndAllocate(const char* name, char** dstPtr);
void SaveInternalFileAndAllocate(const char* name, char* srcPtr, int fileLen);