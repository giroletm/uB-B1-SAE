#include "A_Camera.h"
#include "A_Joueur.h"
#include "A_Map.h"

A_Camera* A_Camera::instance = 0;

Actor* A_Camera::make() {
	instance = new A_Camera();
	return instance;
}

int A_Camera::onCreate() {
	this->boundPosition = &A_Joueur::instance->pos;

	this->onExecute();

	return 1;
}

int A_Camera::onDelete() {
	instance = 0;

	return 1;
}

int A_Camera::onExecute() {
	if (this->boundPosition) {
		this->pos = *this->boundPosition;

		SDL_Rect mapBounds = A_Map::instance->bounds;
		SDL_FRect winBounds = { 0, 0, (float)Game::instance->ww, (float)Game::instance->wh };
		ConvertScreenRectToWorldRectF(&winBounds, &winBounds);

		int trxM = (int)(mapBounds.x + mapBounds.w);
		int bryM = (int)(mapBounds.y + mapBounds.h);

		int trxW = (int)(winBounds.x + winBounds.w);
		int bryW = (int)(winBounds.y + winBounds.h);

		int overRight = trxW - trxM;
		int overLeft = (int)(mapBounds.x - winBounds.x);
		int overBottom = bryW - bryM;
		int overTop = (int)(mapBounds.y - winBounds.y);


		if (((overRight >= 0) && (overLeft >= 0)) || ((overLeft + overRight) > 0)) {
			this->pos.x = (float)(mapBounds.x + (mapBounds.w / 2));
		}
		else if (overRight >= 0) {
			this->pos.x -= overRight;
		}
		else if (overLeft >= 0) {
			this->pos.x += overLeft;
		}


		if (((overBottom >= 0) && (overTop >= 0)) || ((overTop + overBottom) > 0)) {
			this->pos.y = (float)(mapBounds.y + (mapBounds.h / 2));
		}
		else if (overBottom >= 0) {
			this->pos.y -= overBottom;
		}
		else if (overTop >= 0) {
			this->pos.y += overTop;
		}
	}

	return 1;
}

Vec2* A_Camera::GetPosition() {
	return &this->pos;
}


/* ConvertPointToFPoint */

void ConvertPointToFPoint(const SDL_Point* srcPoint, SDL_FPoint* dstPoint) {
	if (srcPoint == NULL || dstPoint == NULL)
		return;

	*dstPoint = { (float)srcPoint->x, (float)srcPoint->y };
}


void ConvertRectToFRect(const SDL_Rect* srcRect, SDL_FRect* dstRect) {
	if (srcRect == NULL || dstRect == NULL)
		return;

	*dstRect = { (float)srcRect->x, (float)srcRect->y, (float)srcRect->w, (float)srcRect->h };
}


/* ConvertWorldPointToScreenPosition */

void A_Camera::ConvertWorldPointToScreenPositionF(const SDL_FPoint* srcPoint, SDL_FPoint* dstPoint) {
	if (srcPoint == NULL || dstPoint == NULL)
		return;

	dstPoint->x = (float)(((srcPoint->x - this->GetPosition()->x) * zoom) + (Game::instance->ww / 2));
	dstPoint->y = (float)(((srcPoint->y - this->GetPosition()->y) * zoom) + (Game::instance->wh / 2));
}

void A_Camera::ConvertWorldPointToScreenPosition(const SDL_Point* srcPoint, SDL_Point* dstPoint) {
	if (srcPoint == NULL || dstPoint == NULL)
		return;

	dstPoint->x = (int)(((srcPoint->x - this->GetPosition()->x) * zoom) + (Game::instance->ww / 2));
	dstPoint->y = (int)(((srcPoint->y - this->GetPosition()->y) * zoom) + (Game::instance->wh / 2));
}


/* ConvertScreenPositionToWorldPoint */

void A_Camera::ConvertScreenPositionToWorldPointF(const SDL_FPoint* srcPoint, SDL_FPoint* dstPoint) {
	if (srcPoint == NULL || dstPoint == NULL)
		return;

	dstPoint->x = (float)(((srcPoint->x - (Game::instance->ww / 2)) / zoom) + this->GetPosition()->x);
	dstPoint->y = (float)(((srcPoint->y - (Game::instance->wh / 2)) / zoom) + this->GetPosition()->y);
}

void A_Camera::ConvertScreenPositionToWorldPoint(const SDL_Point* srcPoint, SDL_Point* dstPoint) {
	if (srcPoint == NULL || dstPoint == NULL)
		return;

	dstPoint->x = (int)(((srcPoint->x - (Game::instance->ww / 2)) / zoom) + this->GetPosition()->x);
	dstPoint->y = (int)(((srcPoint->y - (Game::instance->wh / 2)) / zoom) + this->GetPosition()->y);
}


/* ConvertWorldRectToScreenRect */

void A_Camera::ConvertWorldRectToScreenRectF(const SDL_FRect* srcRect, SDL_FRect* dstRect) {
	if (srcRect == NULL || dstRect == NULL)
		return;
	
	SDL_FRect dst;

	SDL_FPoint tmp = { srcRect->x, srcRect->y };
	ConvertWorldPointToScreenPositionF(&tmp, &tmp);
	dst.x = tmp.x;
	dst.y = tmp.y;

	dst.w = srcRect->x + srcRect->w;
	dst.h = srcRect->y + srcRect->h;
	tmp = { dst.w, dst.h };
	ConvertWorldPointToScreenPositionF(&tmp, &tmp);

	dstRect->x = dst.x;
	dstRect->y = dst.y;
	dstRect->w = tmp.x - dst.x;
	dstRect->h = tmp.y - dst.y;
}

void A_Camera::ConvertWorldRectToScreenRect(const SDL_Rect* srcRect, SDL_Rect* dstRect) {
	if (srcRect == NULL || dstRect == NULL)
		return;

	SDL_Rect dst;

	SDL_Point tmp = { srcRect->x, srcRect->y };
	ConvertWorldPointToScreenPosition(&tmp, &tmp);
	dst.x = tmp.x;
	dst.y = tmp.y;

	dst.w = srcRect->x + srcRect->w;
	dst.h = srcRect->y + srcRect->h;
	tmp = { dst.w, dst.h };
	ConvertWorldPointToScreenPosition(&tmp, &tmp);

	dstRect->x = dst.x;
	dstRect->y = dst.y;
	dstRect->w = tmp.x - dst.x;
	dstRect->h = tmp.y - dst.y;
}


/* ConvertScreenRectToWorldRect */

void A_Camera::ConvertScreenRectToWorldRectF(const SDL_FRect* srcRect, SDL_FRect* dstRect) {
	if (srcRect == NULL || dstRect == NULL)
		return;

	SDL_FRect dst;

	SDL_FPoint tmp = { srcRect->x, srcRect->y };
	ConvertScreenPositionToWorldPointF(&tmp, &tmp);
	dst.x = tmp.x;
	dst.y = tmp.y;

	dst.w = srcRect->x + srcRect->w;
	dst.h = srcRect->y + srcRect->h;
	tmp = { dst.w, dst.h };
	ConvertScreenPositionToWorldPointF(&tmp, &tmp);

	dstRect->x = dst.x;
	dstRect->y = dst.y;
	dstRect->w = tmp.x - dst.x;
	dstRect->h = tmp.y - dst.y;
}

void A_Camera::ConvertScreenRectToWorldRect(const SDL_Rect* srcRect, SDL_Rect* dstRect) {
	if (srcRect == NULL || dstRect == NULL)
		return;

	SDL_Rect dst;

	SDL_Point tmp = { srcRect->x, srcRect->y };
	ConvertScreenPositionToWorldPoint(&tmp, &tmp);
	dst.x = tmp.x;
	dst.y = tmp.y;

	dst.w = srcRect->x + srcRect->w;
	dst.h = srcRect->y + srcRect->h;
	tmp = { dst.w, dst.h };
	ConvertScreenPositionToWorldPoint(&tmp, &tmp);

	dstRect->x = dst.x;
	dstRect->y = dst.y;
	dstRect->w = tmp.x - dst.x;
	dstRect->h = tmp.y - dst.y;
}


/* RenderCopy */

int A_Camera::RenderCopyF(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_FRect* dstRect) {
	SDL_FRect* realDstPtr = NULL;

	SDL_FRect realDst;
	if (dstRect != NULL) {
		ConvertWorldRectToScreenRectF(dstRect, &realDst);
		realDstPtr = &realDst;

#ifdef __SWITCH
		if (!IsRectInRect(realDst.x, realDst.y, realDst.x + realDst.w, realDst.y + realDst.h, 0, 0, 1080, 720))
			return 0;
#endif
	}

	return SDL_RenderCopyF(Game::instance->renderer, texture, srcRect, realDstPtr);
}

int A_Camera::RenderCopy(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect) {
	SDL_Rect* realDstPtr = NULL;

	SDL_Rect realDst;
	if (dstRect != NULL) {
		ConvertWorldRectToScreenRect(dstRect, &realDst);
		realDstPtr = &realDst;

		#ifdef __SWITCH
			if (!IsRectInRect(realDst.x, realDst.y, realDst.x + realDst.w, realDst.y + realDst.h, 0, 0, 1080, 720))
			return 0;
		#endif
	}

	return SDL_RenderCopy(Game::instance->renderer, texture, srcRect, realDstPtr);
}


/* RenderCopyEx */

int A_Camera::RenderCopyExF(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_FRect* dstRect, double angle, const SDL_FPoint* center, SDL_RendererFlip flip) {
	SDL_FRect* realDstPtr = NULL;

	SDL_FRect realDst;
	if (dstRect != NULL) {
		ConvertWorldRectToScreenRectF(dstRect, &realDst);
		realDstPtr = &realDst;

		#ifdef __SWITCH
			if (!IsRectInRect(realDst.x, realDst.y, realDst.x + realDst.w, realDst.y + realDst.h, 0, 0, 1080, 720))
			return 0;
		#endif
	}

	SDL_FPoint* realCtrPtr = NULL;

	SDL_FPoint realCtr;
	if (center != NULL) {
		ConvertWorldPointToScreenPositionF(center, &realCtr);
		realCtrPtr = &realCtr;
	}

	return SDL_RenderCopyExF(Game::instance->renderer, texture, srcRect, realDstPtr, angle, realCtrPtr, flip);
}

int A_Camera::RenderCopyEx(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect, double angle, const SDL_Point* center, SDL_RendererFlip flip) {
	SDL_Rect* realDstPtr = NULL;

	SDL_Rect realDst;
	if (dstRect != NULL) {
		ConvertWorldRectToScreenRect(dstRect, &realDst);
		realDstPtr = &realDst;

		#ifdef __SWITCH
			if (!IsRectInRect(realDst.x, realDst.y, realDst.x + realDst.w, realDst.y + realDst.h, 0, 0, 1080, 720))
			return 0;
		#endif
	}

	SDL_Point* realCtrPtr = NULL;

	SDL_Point realCtr;
	if (center != NULL) {
		ConvertWorldPointToScreenPosition(center, &realCtr);
		realCtrPtr = &realCtr;
	}

	return SDL_RenderCopyEx(Game::instance->renderer, texture, srcRect, realDstPtr, angle, realCtrPtr, flip);
}

/* RenderFillRect */

int A_Camera::RenderFillRectF(const SDL_FRect* rect) {
	SDL_FRect* realDstPtr = NULL;

	SDL_FRect realDst;
	if (rect != NULL) {
		ConvertWorldRectToScreenRectF(rect, &realDst);
		realDstPtr = &realDst;
	}

	return SDL_RenderFillRectF(Game::instance->renderer, realDstPtr);
}

int A_Camera::RenderFillRect(const SDL_Rect* rect) {
	SDL_Rect* realDstPtr = NULL;

	SDL_Rect realDst;
	if (rect != NULL) {
		ConvertWorldRectToScreenRect(rect, &realDst);
		realDstPtr = &realDst;
	}

	return SDL_RenderFillRect(Game::instance->renderer, realDstPtr);
}

/* Zoom */

void A_Camera::Zoom(int value) {
	float mzoom = (value / 2.0f);
	zoom += mzoom;

	if (zoom < 1.5f)
		zoom = 1.5f;

	if (zoom > 3.0f)
		zoom = 3.0f;
}

/* isPointInBounds */

bool A_Camera::isPointInBoundsF(const SDL_FPoint* point) {
	if (point == NULL)
		return false;

	SDL_Point actualPoint = { (int)point->x, (int)point->y };
	return isPointInBounds(&actualPoint);
}

bool A_Camera::isPointInBounds(const SDL_Point* point) {
	if (point == NULL)
		return false;

	SDL_FRect winBounds = { 0, 0, (float)Game::instance->ww, (float)Game::instance->wh };
	ConvertScreenRectToWorldRectF(&winBounds, &winBounds);

	return point->x >= winBounds.x
		&& point->y >= winBounds.y
		&& point->x <= (winBounds.x + winBounds.w)
		&& point->y <= (winBounds.y + winBounds.h);
}

/* isRectInBounds */

bool A_Camera::isRectInBoundsF(const SDL_FRect* rect) {
	if (rect == NULL)
		return false;

	SDL_Rect actualRect = { (int)rect->x, (int)rect->y, (int)rect->w, (int)rect->h };
	return isRectInBounds(&actualRect);
}

bool A_Camera::isRectInBounds(const SDL_Rect* rect) {
	if (rect == NULL)
		return false;

	SDL_FRect winBounds = { 0, 0, (float)Game::instance->ww, (float)Game::instance->wh };
	ConvertScreenRectToWorldRectF(&winBounds, &winBounds);

	float rec1x2 = winBounds.x + winBounds.w;
	float rec1y2 = winBounds.y + winBounds.h;
	float rec2x2 = (float)(rect->x + rect->w);
	float rec2y2 = (float)(rect->y + rect->h);

	bool widthIsPositive = SDL_min(rec1x2, rec2x2) > SDL_max(winBounds.x, rect->x);
	bool heightIsPositive = SDL_min(rec1y2, rec2y2) > SDL_max(winBounds.y, rect->y);
	return (widthIsPositive && heightIsPositive);
}