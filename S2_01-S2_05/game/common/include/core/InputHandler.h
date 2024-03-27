#pragma once

#include <vector>

#include <SDL_keycode.h>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include <common.h>


class KeyHandle {
public:
	SDL_Keycode keycode;
	bool pressed = 0;
	bool held = 0;
	bool released = 0;
};

class MouseHandle {
public:
	int x = 0;
	int y = 0;
	KeyHandle* buttons[3] = {NULL, NULL, NULL};
};

class InputHandler {
public:
	InputHandler();
	~InputHandler();

	MouseHandle mouseHandle;
	std::vector<KeyHandle*> keyHandles;

	void updateMouse();

	void keyStateChanged(SDL_Keycode keycode, bool idUp);
	KeyHandle* getKeyHandle(SDL_Keycode keycode, bool return0 = false);
	KeyHandle* getAnyKeyHandle();
	void updateKeys();
	void consumeKey(SDL_Keycode keycode);

	int getHeldDirection();
	int getPressedDirection();
	int getHeldInteraction();
	int getPressedInteraction();
	void consumeInteraction();

private:
	SDL_Keycode GetKeyAlias(SDL_Keycode keycode);
};