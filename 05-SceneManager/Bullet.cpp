#include "Bullet.h"
#include "Game.h"

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BULLET_WIDTH / 2;
	top = y - BULLET_HEIGHT / 2;
	right = left + BULLET_WIDTH;
	bottom = top + BULLET_HEIGHT;
}


void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Bullet stops at nothing, so its position shouldnt be affected by the collision system, basically a slap-on bullet
	x += vx * dt;
	y += vy * dt;

	CGame* game = CGame::GetInstance();

	if (!(game->IsInCamera(x - BULLET_WIDTH, y + BULLET_HEIGHT) || game->IsInCamera(x - BULLET_WIDTH, y - BULLET_HEIGHT)
		&& game->IsInCamera(x + BULLET_WIDTH, y + BULLET_HEIGHT) || game->IsInCamera(x + BULLET_WIDTH, y - BULLET_HEIGHT))) 
	{
		isDeleted = true;
	}

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