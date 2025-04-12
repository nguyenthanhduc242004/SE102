#pragma once

#include "Background.h"

#define BACKGROUND_CLOUDE_WIDTH 32
#define BACKGROUND_CLOUDE_HEIGHT 24

class CBackgroundCloud : public CBackground
{
protected:
	int sprite_id_first_quarter;
	int sprite_id_second_quarter;
	int sprite_id_third_quarter;
	int sprite_id_fourth_quarter;

public:
	CBackgroundCloud(float x, float y,
		int length,
		int sprite_id_first_quarter, int sprite_id_second_quarter, int sprite_id_third_quarter, int sprite_id_fourth_quarter)
		:CBackground(x, y, BACKGROUND_CLOUDE_WIDTH / 4, BACKGROUND_CLOUDE_HEIGHT, length, NULL, NULL, NULL)
	{
		this->sprite_id_first_quarter = sprite_id_first_quarter;
		this->sprite_id_second_quarter = sprite_id_second_quarter;
		this->sprite_id_third_quarter = sprite_id_third_quarter;
		this->sprite_id_fourth_quarter = sprite_id_fourth_quarter;
	}

	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

typedef CBackground* LPBACKGROUND;
