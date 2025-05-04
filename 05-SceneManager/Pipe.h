#pragma once

#include "GameObject.h"

#define PIPE_WIDTH 16
#define PIPE_HEIGHT 16

#define SPRITE_ID_PIPE_TOP_LEFT		180001
#define SPRITE_ID_PIPE_TOP_RIGHT	180002
#define SPRITE_ID_PIPE_BOTTOM_LEFT	180003
#define SPRITE_ID_PIPE_BOTTOM_RIGHT	180004

class CPipe : public CGameObject
{
protected:
	int height;				// Unit: cell 
	float cell_width = PIPE_WIDTH;
	float cell_height = PIPE_HEIGHT;

public:
	CPipe(float x, float y) : CGameObject(x, y) {}
	CPipe(float x, float y,	int height) :CGameObject(x, y)
	{
		this->height = height;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionCollidable(float nx, float ny);
};

typedef CPipe* LPPIPE;