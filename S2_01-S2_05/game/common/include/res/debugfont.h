#pragma once

#include <SDL_ttf.h>

#include <common.h>

#define DEFAULT_DBGFONT_SIZE 24
#define DEFAULT_DBGFONT_STYLE TTF_STYLE_NORMAL
#define DEFAULT_DBGFONT_WRAPPING TTF_WRAPPED_ALIGN_LEFT

TTF_Font* GetDebugFont();
TTF_Font* GetDebugFontOutline();
SDL_Surface* WriteDebugText(const char* text, bool invertColors=false, int ptSize=DEFAULT_DBGFONT_SIZE, int style=DEFAULT_DBGFONT_STYLE, int wrappedAlign=DEFAULT_DBGFONT_WRAPPING);