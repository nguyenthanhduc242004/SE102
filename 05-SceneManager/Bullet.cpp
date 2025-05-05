#include "Bullet.h"

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BULLET_WIDTH / 2;
	top = y - BULLET_HEIGHT / 2;
	right = left + BULLET_WIDTH;
	bottom = top + BULLET_HEIGHT;
}

void CBullet::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CBullet::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_BULLET_LEFT)->Render(x, y);
	//RenderBoundingBox();
}