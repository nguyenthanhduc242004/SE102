#include "Pipe.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CPipe::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->cell_width / 2 + rect.right / 2;
	float yy = y + this->cell_height / 2 - rect.bottom / 2;

	CGame::GetInstance()->Draw(xx - cx, yy - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CPipe::Render()
{
	if (this->height <= 0) return;
	float xx = x;
	float yy = y;
	CSprites* s = CSprites::GetInstance();

	// Drawing the first row
	s->Get(spriteIdBottomLeft)->Draw(xx, yy);
	xx += this->cell_width;
	s->Get(spriteIdBottomRight)->Draw(xx, yy);

	// Resetting the x-axis
	xx = x;

	// Drawing the middle row(s)
	if (!isUpsideDown) {
		yy -= this->cell_height;
		for (int i = 1; i < this->height - 1; i++) {
			s->Get(spriteIdBottomLeft)->Draw(xx, yy);
			xx += this->cell_width;
			s->Get(spriteIdBottomRight)->Draw(xx, yy);

			// Resetting the x-axis
			xx = x;

			yy -= this->cell_height;
		}
	}
	else {
		yy -= this->cell_height;
		for (int i = 1; i < this->height - 1; i++) {
			s->Get(spriteIdTopLeft)->Draw(xx, yy);
			xx += this->cell_width;
			s->Get(spriteIdTopRight)->Draw(xx, yy);

			// Resetting the x-axis
			xx = x;

			yy -= this->cell_height;
		}
	}

	// Drawing the last row
	if (this->height > 1) {
		s->Get(spriteIdTopLeft)->Draw(xx, yy);
		xx += this->cell_width;
		s->Get(spriteIdTopRight)->Draw(xx, yy);
	}
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cell_width / 2;
	l = x - cellWidth_div_2;
	b = y + this->cell_height / 2;
	t = b - this->cell_height * this->height;
	// Pipe length is always 2
	r = l + this->cell_width * 2;
}

int CPipe::IsDirectionCollidable(float nx, float ny)
{
	return 1;
}