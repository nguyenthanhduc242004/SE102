#include "FlyingScore.h"

CFlyingScore::CFlyingScore(float x, float y, int type) :CGameObject(x, y)
{
	this->type = type;
	switch (type) {
	case FLYING_SCORE_TYPE_100:
		this->sprite_id = SPRITE_ID_FLYING_SCORE_100;
		break;
	case FLYING_SCORE_TYPE_200:
		this->sprite_id = SPRITE_ID_FLYING_SCORE_200;
		break;
	case FLYING_SCORE_TYPE_400:
		this->sprite_id = SPRITE_ID_FLYING_SCORE_400;
		break;
	case FLYING_SCORE_TYPE_800:
		this->sprite_id = SPRITE_ID_FLYING_SCORE_800;
		break;
	case FLYING_SCORE_TYPE_1000:
		this->sprite_id = SPRITE_ID_FLYING_SCORE_1000;
		break;
	case FLYING_SCORE_TYPE_2000:
		this->sprite_id = SPRITE_ID_FLYING_SCORE_2000;
		break;
	case FLYING_SCORE_TYPE_4000:
		this->sprite_id = SPRITE_ID_FLYING_SCORE_4000;
		break;
	case FLYING_SCORE_TYPE_8000:
		this->sprite_id = SPRITE_ID_FLYING_SCORE_8000;
		break;
	}
	this->ax = 0;
	this->ay = FLYING_SCORE_GRAVITY;
	this->vx = 0;
	this->vy = -FLYING_SCORE_INITIAL_SPEED;
	created_start = GetTickCount64();
}

void CFlyingScore::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FLYING_SCORE_WIDTH / 2;
	top = y - FLYING_SCORE_HEIGHT / 2;
	right = left + FLYING_SCORE_WIDTH;
	bottom = top + FLYING_SCORE_HEIGHT;
}

void CFlyingScore::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	x += vx * dt;
	y += vy * dt;

	if (GetTickCount64() - created_start > FLYING_SCORE_FLYING_TIME)
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CFlyingScore::Render()
{
	CSprites* s = CSprites::GetInstance();
	s->Get(this->sprite_id)->Draw(x, y);
	//RenderBoundingBox();
}
