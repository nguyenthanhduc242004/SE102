#pragma once

#include "GameObject.h"

// 
// The most popular type of object in Mario! 
// 
class CPlatform : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;
	boolean direction1, direction2, direction3, direction4;

public:
	// Thêm 4 thuộc tính là direction 1,2,3,4 để custom về hướng collision
	// direction1 -> nx = -1
	// direction2 -> nx = 1
	// direction3 -> ny = -1
	// direction4 -> ny = 1
	CPlatform(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end,
		boolean direction1, boolean direction2, boolean direction3, boolean direction4) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
		this->direction1 = direction1;
		this->direction2 = direction2;
		this->direction3 = direction3;
		this->direction4 = direction4;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionColliable(float nx, float ny);
};

typedef CPlatform* LPPLATFORM;