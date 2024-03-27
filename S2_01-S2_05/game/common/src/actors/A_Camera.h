#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <common.h>

#include <core/Game.h>
#include <core/Actors.h>

class A_Camera : public Actor {
public:
	int onCreate();
	int onDelete();
	int onExecute();

	Vec2* boundPosition = NULL;

	float zoom =
	#if defined(_WIN32) || defined(__SWITCH__)
		2.0f;
	#elif defined(__ANDROID__)
		4.0f;
	#endif

	Vec2* GetPosition();

	void ConvertWorldPointToScreenPositionF(const SDL_FPoint* srcPoint, SDL_FPoint* dstPoint);
	void ConvertWorldPointToScreenPosition(const SDL_Point* srcPoint, SDL_Point* dstPoint);

	void ConvertScreenPositionToWorldPointF(const SDL_FPoint* srcPoint, SDL_FPoint* dstPoint);
	void ConvertScreenPositionToWorldPoint(const SDL_Point* srcPoint, SDL_Point* dstPoint);

	void ConvertWorldRectToScreenRectF(const SDL_FRect* srcRect, SDL_FRect* dstRect);
	void ConvertWorldRectToScreenRect(const SDL_Rect* srcRect, SDL_Rect* dstRect);

	void ConvertScreenRectToWorldRectF(const SDL_FRect* srcRect, SDL_FRect* dstRect);
	void ConvertScreenRectToWorldRect(const SDL_Rect* srcRect, SDL_Rect* dstRect);

	int RenderCopyF(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_FRect* dstRect);
	int RenderCopy(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect);

	int RenderCopyExF(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_FRect* dstRect, double angle, const SDL_FPoint* center, SDL_RendererFlip flip);
	int RenderCopyEx(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect, double angle, const SDL_Point* center, SDL_RendererFlip flip);

	int RenderFillRectF(const SDL_FRect* rect);
	int RenderFillRect(const SDL_Rect* rect);

	void Zoom(int value);

	bool isPointInBoundsF(const SDL_FPoint* point);
	bool isPointInBounds(const SDL_Point* point);
	bool isRectInBoundsF(const SDL_FRect* rect);
	bool isRectInBounds(const SDL_Rect* rect);

	static Actor* make();

	static A_Camera* instance;

};
