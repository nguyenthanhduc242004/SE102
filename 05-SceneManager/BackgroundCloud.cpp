#include "BackgroundCloud.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CBackgroundCloud::Render()
{
	if (this->length <= 0) return;
	float xx = x;
	CSprites* s = CSprites::GetInstance();

	s->Get(SPRITE_ID_BACKGROUND_CLOUD_FIRST_QUARTER)->Draw(xx, y);
	xx += this->cellWidth;
	s->Get(SPRITE_ID_BACKGROUND_CLOUD_SECOND_QUARTER)->Draw(xx, y);
	xx += this->cellWidth;

	for (int i = 1; i < this->length; i++)
	{
		s->Get(SPRITE_ID_BACKGROUND_CLOUD_THIRD_QUARTER)->Draw(xx, y);
		xx += this->cellWidth;
		s->Get(SPRITE_ID_BACKGROUND_CLOUD_SECOND_QUARTER)->Draw(xx, y);
		xx += this->cellWidth;
	}

	s->Get(SPRITE_ID_BACKGROUND_CLOUD_THIRD_QUARTER)->Draw(xx, y);
	xx += this->cellWidth;
	s->Get(SPRITE_ID_BACKGROUND_CLOUD_FOURTH_QUARTER)->Draw(xx, y);

	//RenderBoundingBox();	
}

void CBackgroundCloud::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * 4 * (1 + 1 / 2 * (length - 1));
	b = t + this->cellHeight;
}