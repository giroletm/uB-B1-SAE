#include <core/InputHandler.h>

InputHandler::InputHandler() {
	keyHandles.push_back(new KeyHandle);
	keyHandles.push_back(new KeyHandle);
	keyHandles[0]->keycode = (SDL_Keycode)0; // All 0s (used for nonexistent keys)
	keyHandles[1]->keycode = (SDL_Keycode)0; // Any key state

	for (int i = 0; i < 3; i++) {
		mouseHandle.buttons[i] = new KeyHandle();
		mouseHandle.buttons[i]->keycode = i;
	}
}

InputHandler::~InputHandler() {
	u32 khsSize = (u32)keyHandles.size();
	for (u32 i = 0; i < khsSize; i++) {
		delete keyHandles[i];
	}
}

void InputHandler::updateMouse() {
	#ifndef __SWITCH__
	int xM, yM;
	SDL_GetMouseState(&xM, &yM);

	mouseHandle.x = xM;
	mouseHandle.y = yM;
	#endif
}

SDL_Keycode InputHandler::GetKeyAlias(SDL_Keycode keycode) {
	if (keycode == SDLK_RSHIFT) keycode = SDLK_LSHIFT;
	if (keycode == SDLK_KP_ENTER) keycode = SDLK_RETURN;
	if (keycode == SDLK_APPLICATION) keycode = SDL_BUTTON_RIGHT;
	if (keycode == SDLK_KP_0) keycode = SDLK_0;
	if (keycode >= SDLK_KP_1 && keycode <= SDLK_KP_9) keycode -= (SDLK_KP_1 - SDLK_1);

	return keycode;
}

void InputHandler::keyStateChanged(SDL_Keycode keycode, bool idUp) {
	// Blacklist
	if (keycode >= SDLK_F1 && keycode <= SDLK_F12) return;
	if (keycode == SDLK_CAPSLOCK || keycode == SDLK_LCTRL || keycode == SDLK_RCTRL ||
		keycode == SDLK_LGUI || keycode == SDLK_RGUI || keycode == SDLK_TAB ||
		keycode == SDLK_LALT || keycode == SDLK_RALT ||
		keycode == SDLK_NUMLOCKCLEAR || keycode == SDLK_VOLUMEDOWN || keycode == SDLK_VOLUMEUP || keycode == SDLK_CALCULATOR ||
		keycode == SDLK_PRINTSCREEN || keycode == SDLK_PAUSE || keycode == SDLK_PASTE || keycode == SDLK_BACKSPACE)
		return;

	// Aliases
	keycode = GetKeyAlias(keycode);

	KeyHandle* khandle = getKeyHandle(keycode, false);

	if (!idUp) {
		khandle->pressed = true;
		keyHandles[1]->pressed = true;
		khandle->held = true;
		khandle->released = false;
	}
	else {
		if (khandle->held)
			khandle->released = true;
		khandle->held = false;
	}
}

KeyHandle* InputHandler::getKeyHandle(SDL_Keycode keycode, bool return0) {
	if (keycode >= SDL_BUTTON_LEFT && keycode <= SDL_BUTTON_RIGHT) {
		return mouseHandle.buttons[keycode - 1];
	}

	keycode = GetKeyAlias(keycode);

	u32 khsSize = (u32)keyHandles.size();
	for (u32 i = 2; i < khsSize; i++) {
		if (keyHandles[i]->keycode == keycode) return keyHandles[i];
	}

	if (return0) return 0;
	else {
		KeyHandle* khandle = new KeyHandle;
		khandle->keycode = keycode;
		keyHandles.push_back(khandle);

		return khandle;
	}
}

KeyHandle* InputHandler::getAnyKeyHandle() {
	keyHandles[1]->held = false;
	keyHandles[1]->released = false;

	u32 khsSize = (u32)keyHandles.size();
	for (u32 i = 2; i < khsSize; i++) {
		if (keyHandles[i]->held) {
			keyHandles[1]->held = true;
			break;
		}
	}

	if (!keyHandles[1]->held) {
		for (u32 i = 2; i < khsSize; i++) {
			if (keyHandles[i]->released) {
				keyHandles[1]->released = true;
				break;
			}
		}
	}

	return keyHandles[1];
}

void InputHandler::updateKeys() {
	for (int i = 0; i < 3; i++) {
		mouseHandle.buttons[i]->pressed = false;
		mouseHandle.buttons[i]->released = false;
	}

	u32 khsSize = (u32)keyHandles.size();
	for (u32 i = 0; i < khsSize; i++) {
		keyHandles[i]->pressed = false;
		keyHandles[i]->released = false;
	}
}

void InputHandler::consumeKey(SDL_Keycode keycode) {
	if (KeyHandle* kh = this->getKeyHandle(keycode, true)) {
		kh->pressed = false;
		kh->held = false;
		kh->released = true;
	}
}

int InputHandler::getHeldDirection() {
	// Right
	KeyHandle* rightArr = this->getKeyHandle(SDLK_RIGHT);
	if (rightArr->held)
		return DIRECTION_RIGHT;

	KeyHandle* dKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_D));
	if (dKey->held)
		return DIRECTION_RIGHT;

	// Down
	KeyHandle* downArr = this->getKeyHandle(SDLK_DOWN);
	if (downArr->held)
		return DIRECTION_DOWN;

	KeyHandle* sKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_S));
	if (sKey->held)
		return DIRECTION_DOWN;

	// Left
	KeyHandle* leftArr = this->getKeyHandle(SDLK_LEFT);
	if (leftArr->held)
		return DIRECTION_LEFT;

	KeyHandle* aKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_A));
	if (aKey->held)
		return DIRECTION_LEFT;

	// Up
	KeyHandle* upArr = this->getKeyHandle(SDLK_UP);
	if (upArr->held)
		return DIRECTION_UP;

	KeyHandle* wKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_W));
	if (wKey->held)
		return DIRECTION_UP;

	return DIRECTION_NONE;
}

int InputHandler::getPressedDirection() {
	// Right
	KeyHandle* rightArr = this->getKeyHandle(SDLK_RIGHT);
	if (rightArr->pressed)
		return DIRECTION_RIGHT;

	KeyHandle* dKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_D));
	if (dKey->pressed)
		return DIRECTION_RIGHT;

	// Down
	KeyHandle* downArr = this->getKeyHandle(SDLK_DOWN);
	if (downArr->pressed)
		return DIRECTION_DOWN;

	KeyHandle* sKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_S));
	if (sKey->pressed)
		return DIRECTION_DOWN;

	// Left
	KeyHandle* leftArr = this->getKeyHandle(SDLK_LEFT);
	if (leftArr->pressed)
		return DIRECTION_LEFT;

	KeyHandle* aKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_A));
	if (aKey->pressed)
		return DIRECTION_LEFT;

	// Up
	KeyHandle* upArr = this->getKeyHandle(SDLK_UP);
	if (upArr->pressed)
		return DIRECTION_UP;

	KeyHandle* wKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_W));
	if (wKey->pressed)
		return DIRECTION_UP;

	return DIRECTION_NONE;
}

int InputHandler::getHeldInteraction() {
	KeyHandle* zKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_Z));
	if (zKey->held)
		return 1;

	KeyHandle* xKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_X));
	if (xKey->held)
		return 1;

	KeyHandle* cKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_C));
	if (cKey->held)
		return 1;

	KeyHandle* enterKey = this->getKeyHandle(SDLK_RETURN);
	if (enterKey->held)
		return 1;

	KeyHandle* spaceKey = this->getKeyHandle(SDLK_SPACE);
	if (spaceKey->held)
		return 1;

	return 0;
}

int InputHandler::getPressedInteraction() {
	KeyHandle* zKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_Z));
	if (zKey->pressed)
		return 1;

	KeyHandle* xKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_X));
	if (xKey->pressed)
		return 1;

	KeyHandle* cKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_C));
	if (cKey->pressed)
		return 1;

	KeyHandle* enterKey = this->getKeyHandle(SDLK_RETURN);
	if (enterKey->pressed)
		return 1;

	KeyHandle* spaceKey = this->getKeyHandle(SDLK_SPACE);
	if (spaceKey->pressed)
		return 1;

	return 0;
}

void InputHandler::consumeInteraction() {
	KeyHandle* zKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_Z));
	if (zKey->held)
		this->consumeKey(SDL_GetKeyFromScancode(SDL_SCANCODE_Z));

	KeyHandle* xKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_X));
	if (xKey->held)
		this->consumeKey(SDL_GetKeyFromScancode(SDL_SCANCODE_X));

	KeyHandle* cKey = this->getKeyHandle(SDL_GetKeyFromScancode(SDL_SCANCODE_C));
	if (cKey->held)
		this->consumeKey(SDL_GetKeyFromScancode(SDL_SCANCODE_C));

	KeyHandle* enterKey = this->getKeyHandle(SDLK_RETURN);
	if (enterKey->held)
		this->consumeKey(SDLK_RETURN);

	KeyHandle* spaceKey = this->getKeyHandle(SDLK_SPACE);
	if (spaceKey->held)
		this->consumeKey(SDLK_SPACE);
}