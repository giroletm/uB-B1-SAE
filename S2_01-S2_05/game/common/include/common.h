#pragma once

#include <vector>

#include <SDL_rect.h>

#include <core/io.h>


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

struct Vec2 {
	float x;
	float y;
};

struct Vec2i {
	int x;
	int y;
};

inline float distance(int x1, int y1, int x2, int y2) {
	float dX = (float)(x2 - x1);
	float dY = (float)(y2 - y1);
	return sqrtf((dX * dX) + (dY * dY));
}

template <typename T>
inline int indexOf(std::vector<T> vector, T item) {
	u32 vecSize = (u32)vector.size();
	for (u32 i = 0; i < vecSize; ++i)
		if (vector[i] == item)
			return i;

	return -1;
}

#define IsPtInRect(x1, y1, x2, y2, x, y) ((x) > (x1) && (x) < (x2) && (y) > (y1) && (y) < (y2))
#define IsRectInRect(ax1, ay1, ax2, ay2, bx1, by1, bx2, by2) ((ax1) < (bx2) && (ax2) > (bx1) && (ay1) < (by2) && (ay2) > (by1)) 

#define DIRECTION_NONE -1
#define DIRECTION_RIGHT 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2
#define DIRECTION_UP 3

// Uncomment to enable android controls
// #define SIMULATE_ANDROID

#ifndef INT_MIN
#define INT_MIN -2147483648
#endif

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

#ifndef NULL
#define NULL 0
#endif