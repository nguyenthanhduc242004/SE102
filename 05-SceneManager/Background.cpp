#include "Background.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CBackground::Render()
{
	if (this->length <= 0) return;
	float xx = x;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBegin)->Draw(xx, y);
	xx += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(xx, y);
		xx += this->cellWidth;
	}
	if (length > 1)
		s->Get(this->spriteIdEnd)->Draw(xx, y);

	//RenderBoundingBox();	
}

int CBackground::IsDirectionColliable(float nx, float ny)
{
	return 0;
}