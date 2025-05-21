#include "Box.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CBox::RenderBoundingBox()
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

void CBox::Render()
{
	if (this->length <= 0 || this->height <= 0) return;
	float xx = x;
	float yy = y;
	CSprites* s = CSprites::GetInstance();

	int sprite_id_top_left, sprite_id_top_middle, sprite_id_top_right;
	int sprite_id_middle_left, sprite_id_middle, sprite_id_middle_right;
	int sprite_id_bottom_left, sprite_id_bottom_middle, sprite_id_bottom_right;
	int sprite_id_shadow_top_right = SPRITE_ID_BOX_SHADOW_TOP_RIGHT;
	int sprite_id_shadow_middle = SPRITE_ID_BOX_SHADOW_MIDDLE;
	int sprite_id_shadow_bottom_left = SPRITE_ID_BOX_SHADOW_BOTTOM_LEFT;
	int sprite_id_shadow_bottom_right = SPRITE_ID_BOX_SHADOW_BOTTOM_RIGHT;

	switch (this->color) {
	case 0:	//PINK
		sprite_id_top_left = SPRITE_ID_BOX_PINK_TOP_LEFT;
		sprite_id_top_middle = SPRITE_ID_BOX_PINK_TOP_MIDDLE;
		sprite_id_top_right = SPRITE_ID_BOX_PINK_TOP_RIGHT;
		sprite_id_middle_left = SPRITE_ID_BOX_PINK_MIDDLE_LEFT;
		sprite_id_middle = SPRITE_ID_BOX_PINK_MIDDLE;
		sprite_id_middle_right = SPRITE_ID_BOX_PINK_MIDDLE_RIGHT;
		sprite_id_bottom_left = SPRITE_ID_BOX_PINK_BOTTOM_LEFT;
		sprite_id_bottom_middle = SPRITE_ID_BOX_PINK_BOTTOM_MIDDLE;
		sprite_id_bottom_right = SPRITE_ID_BOX_PINK_BOTTOM_RIGHT;
		break;
	case 1:	//BLUE
		sprite_id_top_left = SPRITE_ID_BOX_BLUE_TOP_LEFT;
		sprite_id_top_middle = SPRITE_ID_BOX_BLUE_TOP_MIDDLE;
		sprite_id_top_right = SPRITE_ID_BOX_BLUE_TOP_RIGHT;
		sprite_id_middle_left = SPRITE_ID_BOX_BLUE_MIDDLE_LEFT;
		sprite_id_middle = SPRITE_ID_BOX_BLUE_MIDDLE;
		sprite_id_middle_right = SPRITE_ID_BOX_BLUE_MIDDLE_RIGHT;
		sprite_id_bottom_left = SPRITE_ID_BOX_BLUE_BOTTOM_LEFT;
		sprite_id_bottom_middle = SPRITE_ID_BOX_BLUE_BOTTOM_MIDDLE;
		sprite_id_bottom_right = SPRITE_ID_BOX_BLUE_BOTTOM_RIGHT;
		break;
	case 2:	//GREEN
		sprite_id_top_left = SPRITE_ID_BOX_GREEN_TOP_LEFT;
		sprite_id_top_middle = SPRITE_ID_BOX_GREEN_TOP_MIDDLE;
		sprite_id_top_right = SPRITE_ID_BOX_GREEN_TOP_RIGHT;
		sprite_id_middle_left = SPRITE_ID_BOX_GREEN_MIDDLE_LEFT;
		sprite_id_middle = SPRITE_ID_BOX_GREEN_MIDDLE;
		sprite_id_middle_right = SPRITE_ID_BOX_GREEN_MIDDLE_RIGHT;
		sprite_id_bottom_left = SPRITE_ID_BOX_GREEN_BOTTOM_LEFT;
		sprite_id_bottom_middle = SPRITE_ID_BOX_GREEN_BOTTOM_MIDDLE;
		sprite_id_bottom_right = SPRITE_ID_BOX_GREEN_BOTTOM_RIGHT;
		break;
	case 3:	//WHITE
		sprite_id_top_left = SPRITE_ID_BOX_WHITE_TOP_LEFT;
		sprite_id_top_middle = SPRITE_ID_BOX_WHITE_TOP_MIDDLE;
		sprite_id_top_right = SPRITE_ID_BOX_WHITE_TOP_RIGHT;
		sprite_id_middle_left = SPRITE_ID_BOX_WHITE_MIDDLE_LEFT;
		sprite_id_middle = SPRITE_ID_BOX_WHITE_MIDDLE;
		sprite_id_middle_right = SPRITE_ID_BOX_WHITE_MIDDLE_RIGHT;
		sprite_id_bottom_left = SPRITE_ID_BOX_WHITE_BOTTOM_LEFT;
		sprite_id_bottom_middle = SPRITE_ID_BOX_WHITE_BOTTOM_MIDDLE;
		sprite_id_bottom_right = SPRITE_ID_BOX_WHITE_BOTTOM_RIGHT;
		break;
	}

	// Drawing the shadow below
	xx += BOX_SHADOW_WIDTH / 2;
	yy += (this->cell_height + BOX_SHADOW_HEIGHT) / 2;
	s->Get(sprite_id_shadow_bottom_left)->Draw(xx, yy);
	xx += BOX_SHADOW_WIDTH;
	s->Get(sprite_id_shadow_middle)->Draw(xx, yy);
	xx += BOX_SHADOW_WIDTH;

	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(sprite_id_shadow_middle)->Draw(xx, yy);
		xx += BOX_SHADOW_WIDTH;
		s->Get(sprite_id_shadow_middle)->Draw(xx, yy);
		xx += BOX_SHADOW_WIDTH;
	}
	if (this->length > 1) {
		s->Get(sprite_id_shadow_middle)->Draw(xx, yy);
		xx += BOX_SHADOW_WIDTH;
		s->Get(sprite_id_shadow_bottom_right)->Draw(xx, yy);
	}

	yy -= (this->cell_height + BOX_SHADOW_HEIGHT) / 2;

	// Resetting the x-axis
	xx = x;

	// Drawing the first row
	s->Get(sprite_id_bottom_left)->Draw(xx, yy);
	xx += this->cell_width;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(sprite_id_bottom_middle)->Draw(xx, yy);
		xx += this->cell_width;
	}
	if (this->length > 1) 
		s->Get(sprite_id_bottom_right)->Draw(xx, yy);
	yy += BOX_SHADOW_HEIGHT / 2;
	xx += (this->cell_width + BOX_SHADOW_WIDTH) / 2;
	s->Get(sprite_id_shadow_middle)->Draw(xx, yy);
	yy -= BOX_SHADOW_HEIGHT;
	s->Get(sprite_id_shadow_middle)->Draw(xx, yy);
	yy += BOX_SHADOW_HEIGHT / 2;

	// Resetting the x-axis
	xx = x;

	// Drawing the middle row(s)
	yy -= this->cell_height;
	for (int i = 1; i < this->height - 1; i++) {
		s->Get(sprite_id_middle_left)->Draw(xx, yy);
		xx += this->cell_width;
		for (int i = 1; i < this->length - 1; i++)
		{
			s->Get(sprite_id_middle)->Draw(xx, yy);
			xx += this->cell_width;
		}
		if (this->length > 1)
			s->Get(sprite_id_middle_right)->Draw(xx, yy);

		if (i != this->height - 2) {
			yy += BOX_SHADOW_HEIGHT / 2;
			xx += (this->cell_width + BOX_SHADOW_WIDTH) / 2;
			s->Get(sprite_id_shadow_middle)->Draw(xx, yy);
			yy -= BOX_SHADOW_HEIGHT;
			s->Get(sprite_id_shadow_middle)->Draw(xx, yy);
			yy += BOX_SHADOW_HEIGHT / 2;
		}
		else {
			yy += BOX_SHADOW_HEIGHT / 2;
			xx += (this->cell_width + BOX_SHADOW_WIDTH) / 2;
			s->Get(sprite_id_shadow_middle)->Draw(xx, yy);
			yy -= BOX_SHADOW_HEIGHT;
			s->Get(sprite_id_shadow_middle)->Draw(xx, yy);
			yy += BOX_SHADOW_HEIGHT / 2;
		}
		

		// Resetting the x-axis
		xx = x;

		yy -= this->cell_height;
	}

	// Drawing the last row
	if (height > 1) {
		s->Get(sprite_id_top_left)->Draw(xx, yy);
		xx += this->cell_width;
		for (int i = 1; i < this->length - 1; i++)
		{
			s->Get(sprite_id_top_middle)->Draw(xx, yy);
			xx += this->cell_width;
		}
		if (this->length > 1)
			s->Get(sprite_id_top_right)->Draw(xx, yy);

		yy += BOX_SHADOW_HEIGHT / 2;
		xx += (this->cell_width + BOX_SHADOW_WIDTH) / 2;
		s->Get(sprite_id_shadow_top_right)->Draw(xx, yy);
	}
	
	//RenderBoundingBox();
}

void CBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cell_width / 2;
	l = x - cellWidth_div_2;
	b = y + this->cell_height / 2;
	t = b - this->cell_height * this->height;
	r = l + this->cell_width * this->length;

}

int CBox::IsDirectionCollidable(float nx, float ny)
{
	if (nx == 0 && ny == -1) return 1;
	return 0;
}