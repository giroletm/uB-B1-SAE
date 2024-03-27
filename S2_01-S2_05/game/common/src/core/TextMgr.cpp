#include <core/TextMgr.h>

#include <SDL_locale.h>

#include <fstream>

#include <json.hpp>
using json = nlohmann::json;

TextMgr* TextMgr::instance = NULL;

TextMgr::TextMgr() {
	instance = this;
	Load();
}

TextMgr::~TextMgr() {
	for (TextKVP textKVP : content) {
		delete[] textKVP.key;
		delete[] textKVP.value;
	}
}

void TextMgr::Load() {
	char* filename = NULL;

	SDL_Locale * locales = SDL_GetPreferredLocales();

	bool found = false;

	if (locales) {
		int i = 0;
		while (locales[i].language != NULL) {
			SDL_asprintf(&filename, "assets/strings-%s.json", locales[i].language);
			if (FileExists(filename)) {
				found = true;
				break;
			}

			SDL_free(filename);
			i++;
		}

		SDL_free(locales);
	}

	if (!found) {
		SDL_asprintf(&filename, "%s", "assets/strings-en.json");
	}

	if (FileExists(filename)) {
		// Open the file
		char* file;
		int fileLen = LoadFileAndAllocate(filename, &file);
		json stringsData = json::parse(file, file + fileLen);
		delete[] file;

		for (auto it = stringsData.begin(); it != stringsData.end(); ++it) {
			std::string key = it.key();
			std::string value = it.value().get<std::string>();

			u32 keySize = (u32)key.size() + 1;
			u32 valueSize = (u32)value.size() + 1;

			char* keyChr = new char[keySize];
			char* valueChr = new char[valueSize];

			SDL_strlcpy(keyChr, key.c_str(), keySize);
			SDL_strlcpy(valueChr, value.c_str(), valueSize);

			content.push_back({ keyChr, valueChr });
		}
	}

	SDL_free(filename);
}


const char* TextMgr::GetValue(const char* key)
{
	for (TextKVP textKVP : content)
		if (SDL_strcmp(textKVP.key, key) == 0)
			return textKVP.value;

	return NULL;
}