#pragma once

#include "GameObject.h"
#include "AssetIDs.h"
#include "Platform.h"

#define BOX_WIDTH 16
#define BOX_HEIGHT 16

#define BOX_SHADOW_WIDTH 8
#define BOX_SHADOW_HEIGHT 8


class CBox : public CPlatform
{
protected:
	int cell_width = BOX_WIDTH, cell_height = BOX_HEIGHT;
	int color;	// 0: pink; 1: blue; 2: green; 3: white;

public:
	CBox(float x, float y, int length_width, int length_height, int color) : CPlatform(x, y)
	{
		this->length = length_width;
		this->height = length_height;
		this->color = color;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionCollidable(float nx, float ny);
};

typedef CBox* LPBOX;