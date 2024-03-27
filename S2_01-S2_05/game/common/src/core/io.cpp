#include <core/io.h>

#ifndef NULL
#define NULL 0
#endif

#ifdef __ANDROID__
jobject global_asset_manager;
AAssetManager* amgr = NULL;

void InitAndroidAssetMgr() {
	if (!amgr) {
		JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();

		jobject activity = (jobject)SDL_AndroidGetActivity();

		jclass activity_class = env->GetObjectClass(activity);

		jmethodID activity_class_getAssets = env->GetMethodID(activity_class, "getAssets", "()Landroid/content/res/AssetManager;");
		jobject asset_manager = env->CallObjectMethod(activity, activity_class_getAssets); // activity.getAssets();
		global_asset_manager = env->NewGlobalRef(asset_manager);

		amgr = AAssetManager_fromJava(env, global_asset_manager);
	}
}
#endif


bool FileExists(const char* name) {
#if defined(_WIN32)

	struct stat buffer;
	return (stat(name, &buffer) == 0);

#elif defined(__ANDROID__)

	InitAndroidAssetMgr();

	AAsset* asset = AAssetManager_open(amgr, name, AASSET_MODE_STREAMING);
	if (asset == NULL)
		return false;

	AAsset_close(asset);
	return true;

#elif defined(__SWITCH__)

	SDL_RWops* fileptr = SDL_RWFromFile(name, "rb");  // Open the file in binary mode

	if (fileptr != NULL) {
		fileptr->close(fileptr); // Close the file
		return true;
	}

	return false;

#endif

	return false;
}

int LoadFileAndAllocate(const char* name, char** dstPtr) {
	if (!FileExists(name))
		return -1;

#if defined(_WIN32)

	FILE* fileptr = NULL;
	long filelen = 0;

	fopen_s(&fileptr, name, "rb");  // Open the file in binary mode
	if (fileptr != NULL) {
		fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
		filelen = ftell(fileptr);             // Get the current byte offset in the file
		rewind(fileptr);                      // Jump back to the beginning of the file

		if (dstPtr != NULL) {
			*dstPtr = new char[filelen];
			fread(*dstPtr, filelen, 1, fileptr); // Read in the entire file
		}
		fclose(fileptr); // Close the file
	}

	return filelen;

#elif defined(__ANDROID__)

	InitAndroidAssetMgr();

	AAsset* asset = AAssetManager_open(amgr, name, AASSET_MODE_STREAMING);
	int len = AAsset_getLength(asset);

	if (dstPtr != NULL) {
		*dstPtr = new char[len];
		int r = AAsset_read(asset, *dstPtr, len);
	}

	AAsset_close(asset);

	return len;
	
#elif defined(__SWITCH__)

	long filelen = 0;

	SDL_RWops* fileptr = SDL_RWFromFile(name, "rb");  // Open the file in binary mode
	if (fileptr != NULL) {
		filelen = (*fileptr->size)(fileptr);

		if (dstPtr != NULL) {
			*dstPtr = new char[filelen];
			fileptr->read(fileptr, *dstPtr, filelen, 1); // Read in the entire file
		}
		fileptr->close(fileptr); // Close the file
	}

	return filelen;

#endif

	return -1;
}

bool InternalFileExists(const char* name) {
#if defined(_WIN32)

	return FileExists(name);

#elif defined(__ANDROID__)

	char* prefPath = SDL_GetPrefPath("Friteries", "SAE201205");

	char* nPath;
	SDL_asprintf(&nPath, "%s%s", prefPath, name);

	SDL_RWops* file = SDL_RWFromFile(nPath, "rb");
	if (file == NULL)
		return false;

	SDL_RWclose(file);
	return true;

#endif
	
	return false;
}

int LoadInternalFileAndAllocate(const char* name, char** dstPtr) {
	if (!InternalFileExists(name))
		return -1;

#if defined(_WIN32)

	return LoadFileAndAllocate(name, dstPtr);

#elif defined(__ANDROID__)

	char* prefPath = SDL_GetPrefPath("Friteries", "SAE201205");

	char* nPath;
	SDL_asprintf(&nPath, "%s%s", prefPath, name);

	SDL_RWops* file = SDL_RWFromFile(nPath, "rb");
	int filelen = (int)((*file->size)(file));

	if (dstPtr) {
		*dstPtr = new char[filelen];
		SDL_RWread(file, *dstPtr, filelen, 1);
	}

	SDL_RWclose(file);

	SDL_free(nPath);
	SDL_free(prefPath);

	return filelen;

#endif

	return -1;
}

void SaveInternalFileAndAllocate(const char* name, char* srcPtr, int fileLen) {
	if (srcPtr == NULL)
		return;

#if defined(_WIN32)

	std::ofstream file(name, std::ios::binary);
	file.write(srcPtr, fileLen);

#elif defined(__ANDROID__)

	char* prefPath = SDL_GetPrefPath("Friteries", "SAE201205");

	char* nPath;
	SDL_asprintf(&nPath, "%s%s", prefPath, name);

	SDL_RWops* file = SDL_RWFromFile(nPath, "wb");
	SDL_RWwrite(file, srcPtr, fileLen, 1);

	SDL_RWclose(file);

	SDL_free(nPath);
	SDL_free(prefPath);

#endif
}