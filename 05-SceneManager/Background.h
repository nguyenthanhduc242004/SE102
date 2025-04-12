#pragma once

#include "GameObject.h"

// 
// The most popular type of object in Mario! 
// 
class CBackground : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteId;

public:
	CBackground(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteId = sprite_id;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionColliable(float nx, float ny);
};

typedef CBackground* LPBACKGROUND;