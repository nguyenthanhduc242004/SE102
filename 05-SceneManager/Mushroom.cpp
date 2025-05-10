#include "Mushroom.h"


void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == MUSHROOM_STATE_SPAWNING && (y <= y0 + QUESTION_BLOCK_ITEM_Y_OFFSET - (QUESTION_BLOCK_HEIGHT + MUSHROOM_HEIGHT) / 2)) {
		SetState(MUSHROOM_STATE_WALKING);
	}

	HandleTimer(dt);
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
	if (!isSpawned)	return;
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	//	still animation, no relation to state
	if (type == MUSHROOM_TYPE_RED) {
		aniId = ID_ANI_MUSHROOM_RED;
	}
	else if (type == MUSHROOM_TYPE_GREEN) {
		aniId = ID_ANI_MUSHROOM_GREEN;
	}

	animations->Get(aniId)->Render(x, y);
}

//RenderBoundingBox();

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	// Mario is blocking by default, so we handle so Mushroom if landing on Mario's head wont just stop but phase through for Mario to consume, just for safety
	if (dynamic_cast<CMario*>(e->obj)) return;
	if (e->ny != 0)
	{
		vy = 0;
	}

	if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CMushroom::HandleTimer(DWORD dt)
{
	if (spawningTimer.IsRunning()) {
		spawningTimer.Tick(dt);
	}

	if (state == MUSHROOM_STATE_SPAWNING && spawningTimer.HasPassed(QUESTION_BLOCK_BOUNCE_TIME)) {
		vy = -MUSHROOM_SPAWNING_SPEED;
		isSpawned = true;
		spawningTimer.Reset();
	}
}

void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MUSHROOM_WIDTH / 2;
	top = y - MUSHROOM_HEIGHT / 2 - 1;
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
		spawningTimer.Start();
		y0 = y;
		break;
	case MUSHROOM_STATE_WALKING:
		ay = MUSHROOM_GRAVITY;
		mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		vx = mario->GetDirection() * MUSHROOM_SPEED;
		break;
	}
}