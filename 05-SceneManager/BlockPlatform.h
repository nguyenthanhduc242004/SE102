#pragma once

#include "Platform.h"

class CBlockPlatform : public CPlatform
{
protected:
	int cell_width, cell_height;
	int length;				// Unit: cell 
	int spride_id_begin, sprite_id_middle, sprite_id_end;

public:
	CBlockPlatform(float x, float y,
		int cell_width, int cell_height,
		int length, int sprite_id_begin,
		int sprite_id_middle, int sprite_id_end)
		: CPlatform(x, y, cell_width, cell_height, length, sprite_id_begin, sprite_id_middle, sprite_id_end) {}

	int IsDirectionColliable(float nx, float ny);
};

typedef CBlockPlatform* LPBLOCKPLATFORM;