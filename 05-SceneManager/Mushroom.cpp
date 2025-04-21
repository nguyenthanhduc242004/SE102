#include "Mushroom.h"
#include "Utils.h"
#include "Mario.h"
//#include "Block.h"
#include "Brick.h"
#include "PlayScene.h"
//#include "IntroScene.h"
//#include "Camera.h"

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Camera* cam = Camera::GetInstance();
	//if (!cam->isAreaCamera(x, y))

	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}

void CMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	//	still animation, no relation to state
	//	if not spawned yet, no rendering
	if (!isSpawned)
		return;
	if (type == MUSHROOM_TYPE_RED) {
		aniId = ID_ANI_MUSHROOM_RED;
	}
	else if (type == MUSHROOM_TYPE_GREEN) {
		aniId = ID_ANI_MUSHROOM_GREEN;
	}

	//just has a default ani for practice
	if (aniId == -1) aniId = ID_ANI_MUSHROOM_RED;
	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CMushroom*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}


void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MUSHROOM_BBOX_WIDTH / 2;
	top = y - MUSHROOM_BBOX_HEIGHT / 2;
	right = left + MUSHROOM_BBOX_WIDTH;
	bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario;
	switch (state)
	{
	case MUSHROOM_STATE_IDLE:
		vx = vy = 0;
		break;
	case MUSHROOM_STATE_UP:
		vy = -0.5f;
		//start_y = y;
		break;
	case MUSHROOM_STATE_WALKING:
		mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		vx = mario->GetDirection() * MUSHROOM_SPEED;
		break;
	}
}