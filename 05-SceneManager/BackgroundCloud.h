#pragma once

#include "Background.h"

#define BACKGROUND_CLOUDE_WIDTH 32
#define BACKGROUND_CLOUDE_HEIGHT 24

#define SPRITE_ID_BACKGROUND_CLOUD_FIRST_QUARTER 90001
#define SPRITE_ID_BACKGROUND_CLOUD_SECOND_QUARTER 90002
#define SPRITE_ID_BACKGROUND_CLOUD_THIRD_QUARTER 90003
#define SPRITE_ID_BACKGROUND_CLOUD_FOURTH_QUARTER 90004

class CBackgroundCloud : public CBackground
{
public:
	CBackgroundCloud(float x, float y, int length)
		:CBackground(x, y, BACKGROUND_CLOUDE_WIDTH / 4, BACKGROUND_CLOUDE_HEIGHT, length, NULL, NULL, NULL) {}

	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

typedef CBackground* LPBACKGROUND;
