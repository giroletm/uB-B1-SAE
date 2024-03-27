#pragma once

#include <string>
#include <vector>

#include <common.h>

class TextKVP {
public:
	const char* key;
	const char* value;
};

class TextMgr {
public:
	TextMgr();
	~TextMgr();

	static TextMgr* instance;

	void Load();

	const char* GetValue(const char* key);

private:
	std::vector<TextKVP> content;
};