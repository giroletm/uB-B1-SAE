#include <core/SaveMgr.h>

#include <fstream>

SaveMgr* SaveMgr::instance = NULL;

SaveMgr::SaveMgr() {
	instance = this;
	Load();
}

SaveMgr::~SaveMgr() {
	for (SaveKVP saveKVP : content)
		delete saveKVP.value;
}

void SaveMgr::Reset() {
	content.clear();
}

void SaveMgr::Load()
{
	Reset();

	if (InternalFileExists("save.bin")) {
		// Open the file
		u8* buffer;
		LoadInternalFileAndAllocate("save.bin", (char**)&buffer);

		// Histoire d'éviter d'envoyer un fichier cassé, on vérifie quatre octets à valeur fixe
		if (buffer[0] != 'S' || buffer[1] != 'A' || buffer[2] != 'V' || buffer[3] != 'E') {
			delete[] buffer;
			return;
		}

		u32 nbKVPs = *((u32*)(&buffer[4]));
		int idx = 8;

		for (u32 i = 0; i < nbKVPs; i++) {
			SaveKVP saveKVP;

			u32 nbKeyChr = *((u32*)(&buffer[idx]));
			idx += 4;

			for (u32 chr = 0; chr < nbKeyChr; chr++) {
				saveKVP.key.push_back(buffer[idx + chr]);
			}
			idx += nbKeyChr;

			u32 nbValueBytes = saveKVP.valueSize = *((u32*)(&buffer[idx]));
			idx += 4;

			saveKVP.value = new u8[nbValueBytes];
			for (u32 byte = 0; byte < nbValueBytes; byte++) {
				saveKVP.value[byte] = buffer[idx + byte];
			}
			idx += nbValueBytes;

			content.push_back(saveKVP);
		}

		fileLoaded = true;

		delete[] buffer;
	}
}

bool SaveMgr::Save()
{
	try {
		if (!fileLoaded && content.size() == 0)
			return true;

		int baseSize = 8;
		for (SaveKVP saveKVP : content) {
			baseSize += 4 + (int)saveKVP.key.size() + 4 + saveKVP.valueSize;
		}

		u8* buffer = new u8[baseSize];
		buffer[0] = 'S';
		buffer[1] = 'A';
		buffer[2] = 'V';
		buffer[3] = 'E';

		u32 nbKVPs = (u32)content.size();
		*((u32*)(&buffer[4])) = nbKVPs;

		int idx = 8;
		for (u32 i = 0; i < nbKVPs; i++) {
			SaveKVP saveKVP = content[i];

			u32 nbKeyChr = (u32)saveKVP.key.size();
			*((u32*)(&buffer[idx])) = nbKeyChr;
			idx += 4;

			for (u32 chr = 0; chr < nbKeyChr; chr++) {
				buffer[idx + chr] = saveKVP.key[chr];
			}
			idx += nbKeyChr;

			*((u32*)(&buffer[idx])) = saveKVP.valueSize;
			idx += 4;

			for (u32 byte = 0; byte < saveKVP.valueSize; byte++) {
				#pragma warning(push, 3)
				#pragma warning(disable : 6386)
				buffer[idx + byte] = saveKVP.value[byte];
				#pragma warning(pop) 
			}
			idx += saveKVP.valueSize;
		}

		SaveInternalFileAndAllocate("save.bin", (char*)buffer, baseSize);
	}
	catch (...) {
		return false;
	}

	return true;
}

bool SaveMgr::KeyExists(const char* key)
{
	return GetKey(key) != NULL;
}

SaveKVP* SaveMgr::GetKey(const char* key)
{
	for (auto it = content.begin(); it != content.end(); ++it)
		if ((*it).key == key)
			return &(*it);

	return NULL;
}

SaveKVP* SaveMgr::AddKey(const char* key)
{
	SaveKVP* saveKVP = GetKey(key);

	if (saveKVP == NULL) {
		SaveKVP nSaveKVP;
		nSaveKVP.key = key;
		nSaveKVP.valueSize = 0;
		nSaveKVP.value = NULL;

		content.push_back(nSaveKVP);
		return &(*(content.end() - 1));
	}

	return saveKVP;
}
