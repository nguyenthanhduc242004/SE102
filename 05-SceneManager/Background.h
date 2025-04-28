#pragma once

#include "Platform.h"

class CBackground : public CPlatform
{

public:
	CBackground(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end)
		:CPlatform(x, y, cell_width, cell_height, length, sprite_id_begin, sprite_id_middle, sprite_id_end) {}

	void Render();
	int IsDirectionCollidable(float nx, float ny);
};

typedef CBackground* LPBACKGROUND;