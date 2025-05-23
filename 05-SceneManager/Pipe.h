#pragma once

#include "GameObject.h"

#define PIPE_WIDTH 16
#define PIPE_HEIGHT 16

#define SPRITE_ID_PIPE_GREEN_TOP_LEFT		180001
#define SPRITE_ID_PIPE_GREEN_TOP_RIGHT		180002
#define SPRITE_ID_PIPE_GREEN_BOTTOM_LEFT	180003
#define SPRITE_ID_PIPE_GREEN_BOTTOM_RIGHT	180004

#define SPRITE_ID_PIPE_TRANSPARENT_TOP_LEFT			181001
#define SPRITE_ID_PIPE_TRANSPARENT_TOP_RIGHT		181002
#define SPRITE_ID_PIPE_TRANSPARENT_BOTTOM_LEFT		181003
#define SPRITE_ID_PIPE_TRANSPARENT_BOTTOM_RIGHT		181004

class CPipe : public CGameObject
{
protected:
	int height;				// Unit: cell 
	float cell_width = PIPE_WIDTH;
	float cell_height = PIPE_HEIGHT;
	int color;
	bool isUpsideDown;
	bool isAscendingAfter;
	float toX, toY;

	int spriteIdTopLeft, spriteIdTopRight, spriteIdBottomLeft, spriteIdBottomRight;

public:
	CPipe(float x, float y) : CGameObject(x, y) {}
	CPipe(float x, float y, int height, int color, bool isUpsideDown, float toX, float toY, bool isAscendingAfter) :CGameObject(x, y)
	{
		// 0: transparent; 1: green
		this->color = color;
		this->height = height;
		this->isUpsideDown = isUpsideDown;
		this->isAscendingAfter = isAscendingAfter;
		this->toX = toX;
		this->toY = toY;

		switch (color) {
		// Transparent
		case 0:
			spriteIdTopLeft = SPRITE_ID_PIPE_TRANSPARENT_TOP_LEFT;
			spriteIdTopRight = SPRITE_ID_PIPE_TRANSPARENT_TOP_RIGHT;
			spriteIdBottomLeft = SPRITE_ID_PIPE_TRANSPARENT_BOTTOM_LEFT;
			spriteIdBottomRight = SPRITE_ID_PIPE_TRANSPARENT_BOTTOM_RIGHT;
			break;
		// Green
		case 1:
			spriteIdTopLeft = SPRITE_ID_PIPE_GREEN_TOP_LEFT;
			spriteIdTopRight = SPRITE_ID_PIPE_GREEN_TOP_RIGHT;
			spriteIdBottomLeft = SPRITE_ID_PIPE_GREEN_BOTTOM_LEFT;
			spriteIdBottomRight = SPRITE_ID_PIPE_GREEN_BOTTOM_RIGHT;
			break;
		}
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int IsCollidable() { return 1; };
	int IsDirectionCollidable(float nx, float ny);

	bool GetUpsideDown() { return this->isUpsideDown; };
	bool IsAscendingAfter() { return this->isAscendingAfter; }
	float GetToX() { return this->toX; };
	float GetToY() { return this->toY; };

};

typedef CPipe* LPPIPE;