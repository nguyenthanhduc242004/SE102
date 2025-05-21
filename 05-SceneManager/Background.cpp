#include "Background.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CBackground::Render()
{
	if (this->length <= 0 || this->height <= 0) return;
	CSprites* s = CSprites::GetInstance();
	float yy = y;
	RenderRow(yy);
	yy += this->cellHeight;
	for (int i = 1; i < this->height - 1; i++)
	{
		RenderRow(yy);
		yy += this->cellHeight;
	}
	if (height > 1)
		RenderRow(yy);

	//RenderBoundingBox();	
}

void CBackground::RenderRow(float y) {
	CSprites* s = CSprites::GetInstance();
	float xx = x;
	s->Get(this->spriteIdBegin)->Draw(xx, y);
	xx += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(xx, y);
		xx += this->cellWidth;
	}
	if (length > 1)
		s->Get(this->spriteIdEnd)->Draw(xx, y);
}

int CBackground::IsDirectionCollidable(float nx, float ny)
{
	return 0;
}