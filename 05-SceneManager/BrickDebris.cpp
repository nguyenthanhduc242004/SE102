#include "BrickDebris.h"

void CBrickDebris::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BRICK_DEBRIS_WIDTH / 2;
	top = y - BRICK_DEBRIS_HEIGHT / 2;
	right = left + BRICK_DEBRIS_WIDTH;
	bottom = top + BRICK_DEBRIS_HEIGHT;
}

void CBrickDebris::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CBrickDebris::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CBrickDebris::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_BRICK_DEBRIS)->Render(x, y);
	//RenderBoundingBox();
}