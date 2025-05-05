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
	if (state == MUSHROOM_STATE_SPAWNING && GetTickCount64() - spawning_start > QUESTION_BLOCK_BOUNCE_TIME) {
		vy = -MUSHROOM_SPAWNING_SPEED;
	}
	if (state == MUSHROOM_STATE_SPAWNING && (y <= y0 + QUESTION_BLOCK_ITEM_Y_OFFSET - (QUESTION_BLOCK_HEIGHT + MUSHROOM_HEIGHT) / 2)) {
		SetState(MUSHROOM_STATE_WALKING);
	}

	//no logic for camera spawning yet
	//Camera* cam = Camera::GetInstance();
	//if (!cam->isAreaCamera(x, y))

	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}

void CMushroom::Render()
{
	//	if not spawned yet, no rendering
	if (state == MUSHROOM_STATE_IDLE)	return;
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	//	still animation, no relation to state
	if (type == MUSHROOM_TYPE_RED) {
		aniId = ID_ANI_MUSHROOM_RED;
	}
	else if (type == MUSHROOM_TYPE_GREEN) {
		aniId = ID_ANI_MUSHROOM_GREEN;
	}

	// Render only after the QuestionBlock has finished bouncing
	if (GetTickCount64() - spawning_start > QUESTION_BLOCK_BOUNCE_TIME) {
		animations->Get(aniId)->Render(x, y);
	}

	//RenderBoundingBox();
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
	if (state != MUSHROOM_STATE_WALKING) return;
		//if (state == MUSHROOM_STATE_IDLE)
			//return;
		//if (e->ny < 0 && state == MUSHROOM_STATE_SPAWNING)
			//SetState(MUSHROOM_STATE_WALKING);
	//}

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
	left = x - MUSHROOM_WIDTH / 2;
	top = y - MUSHROOM_HEIGHT / 2;
	right = left + MUSHROOM_WIDTH;
	bottom = top + MUSHROOM_HEIGHT;
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario;
	switch (state)
	{
	case MUSHROOM_STATE_IDLE:
		break;
	case MUSHROOM_STATE_SPAWNING:
		spawning_start = GetTickCount64();
		y0 = y;
		break;
	case MUSHROOM_STATE_WALKING:
		ay = MUSHROOM_GRAVITY;
		mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		vx = mario->GetDirection() * MUSHROOM_SPEED;
		break;
	}
}