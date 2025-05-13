#include "Leaf.h"

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - LEAF_BBOX_WIDTH / 2;
	top = y - LEAF_BBOX_HEIGHT / 2;
	right = left + LEAF_BBOX_WIDTH;
	bottom = top + LEAF_BBOX_HEIGHT;
}


void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	x += vx * dt;
	y += vy * dt;

	if (state == LEAF_STATE_SPAWNING && (y <= y0 + QUESTION_BLOCK_ITEM_Y_OFFSET - LEAF_START_FALLING_HEIGHT / 2)) {
		SetState(LEAF_STATE_FALLING);
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

void CLeaf::Render()
{
	//	if not spawned yet, no rendering
	if (!isSpawned)	return;
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	//	still animation, no relation to state
	//if (type == MUSHROOM_TYPE_RED) {
	if (state == LEAF_STATE_SPAWNING)
		aniId = ID_ANI_LEAF_SPAWNING;
	else if (state == LEAF_STATE_FALLING)
		aniId = ID_ANI_LEAF_FALLING;
	//}
	//else if (type == MUSHROOM_TYPE_GREEN) {
		//aniId = ID_ANI_MUSHROOM_GREEN;
	//}

	animations->Get(aniId)->Render(x, y);
}

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj)) return;
}

void CLeaf::HandleTimer(DWORD dt)
{
	if (spawningTimer.IsRunning()) {
		spawningTimer.Tick(dt);
	}
	if (glidingTimer.IsRunning()) {
		glidingTimer.Tick(dt);
	}

	if (state == LEAF_STATE_SPAWNING && spawningTimer.HasPassed(QUESTION_BLOCK_BOUNCE_TIME)) {
		vy = -LEAF_SPAWNING_SPEED;
		ay = LEAF_GRAVITY;
		isSpawned = true;
		spawningTimer.Reset();
	}
	if (glidingTimer.HasPassed(LEAF_GLIDING_TURNING_TIME)) {
		ReverseDirection();
		glidingTimer.Start();
	}
}


void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario;
	switch (state)
	{
	case LEAF_STATE_IDLE:
		break;
	case LEAF_STATE_SPAWNING:
		y0 = y;
		spawningTimer.Start();
		break;
	case LEAF_STATE_FALLING:
		vx = LEAF_GLIDING_SPEED;
		vy = 0.0f;
		ay = LEAF_GLIDING_GRAVITY;
		glidingTimer.Start();
		break;
	}
}