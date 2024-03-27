#pragma once

#include <string>
#include <vector>

#include <common.h>

class SaveKVP {
public:
	std::string key = "";
	u32 valueSize = 0;
	u8* value = NULL;
};

class SaveMgr {
public:
	SaveMgr();
	~SaveMgr();

	static SaveMgr* instance;

	void Reset();
	void Load();
	bool Save();

	bool WasFileLoaded() { return fileLoaded; }

	bool KeyExists(const char* key);

	template<typename T>
	inline void AddValue(const char* key, T* value, int size)
	{
		SaveKVP* saveKVP = AddKey(key);

		if (saveKVP->value) {
			delete saveKVP->value;
		}

		saveKVP->valueSize = size * sizeof(T);
		saveKVP->value = (u8*)(::operator new(saveKVP->valueSize));
		for (u32 i = 0; i < saveKVP->valueSize; i++)
			saveKVP->value[i] = ((u8*)value)[i];
	}

	template<typename T>
	inline void AddValue(const char* key, T value)
	{
		SaveKVP* saveKVP = AddKey(key);

		if (saveKVP->value && saveKVP->valueSize != sizeof(T)) {
			delete saveKVP->value;
		}

		saveKVP->valueSize = sizeof(T);
		saveKVP->value = (u8*)(new T);
		*((T*)saveKVP->value) = value;
	}

	inline void AddValueCC(const char* key, const char* value)
	{
		SaveKVP* saveKVP = AddKey(key);

		u32 nbChr = (int)SDL_strlen(value);

		if (saveKVP->value && saveKVP->valueSize != nbChr) {
			delete saveKVP->value;
		}

		saveKVP->valueSize = nbChr;
		saveKVP->value = new u8[nbChr];

		for (u32 chr = 0; chr < nbChr; chr++) {
			saveKVP->value[chr] = value[chr];
		}
	}

	inline void AddValueStr(const char* key, std::string value)
	{
		AddValue(key, value.c_str());
	}

	template<typename T>
	inline T* GetValue(const char* key)
	{
		SaveKVP* saveKVP = GetKey(key);
		if (saveKVP == NULL || saveKVP->value == NULL) {
			return NULL;
		}

		return (T*)saveKVP->value;
	}

	inline std::string GetValueStr(const char* key)
	{
		std::string str = "";

		SaveKVP* saveKVP = GetKey(key);
		if (saveKVP == NULL || saveKVP->value == NULL) {
			return str;
		}

		for (u32 chr = 0; chr < saveKVP->valueSize; chr++)
			str.push_back(saveKVP->value[chr]);

		return str;
	}

	inline int GetValueSize(const char* key)
	{
		SaveKVP* saveKVP = GetKey(key);
		if (saveKVP == NULL || saveKVP->value == NULL) {
			return 0;
		}

		return saveKVP->valueSize;
	}

private:
	std::vector<SaveKVP> content;
	bool fileLoaded = false;

	SaveKVP* GetKey(const char* key);
	SaveKVP* AddKey(const char* key);
};