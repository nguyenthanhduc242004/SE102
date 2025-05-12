#include "Goomba.h"


CGoomba::CGoomba(float x, float y, int color, boolean isParagoomba) :CGameObject(x, y)
{
	this->color = color;
	this->isParagoomba = isParagoomba;
	this->ay = GOOMBA_GRAVITY;
	nx = -1;
	this->paragoombaStateTimer.Start();
	if (isParagoomba) {
		SetState(PARAGOOMBA_STATE_WALKING);
	}
	else {
		SetState(GOOMBA_STATE_WALKING);
	}
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y - GOOMBA_BBOX_HEIGHT / 2 - 1;
	bottom = top + GOOMBA_BBOX_HEIGHT;
	left = x - GOOMBA_BBOX_WIDTH / 2;
	right = left + GOOMBA_BBOX_WIDTH;
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{

	if (dynamic_cast<CMario*>(e->obj)) return;
	
	if (dynamic_cast<CGoomba*>(e->obj)) {
		OnCollisionWithGoomba(e);
	}
	if (dynamic_cast<CKoopa*>(e->obj)) {
		OnCollisionWithKoopa(e);
	}
	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0)
	{
		vy = 0;
		if (e->ny == -1) {
			if (state == PARAGOOMBA_STATE_FLYING)
				SetState(PARAGOOMBA_STATE_WALKING);
			if (state == PARAGOOMBA_STATE_PRE_FLY)
				vy = -PARAGOOMBA_PRE_FLY_VY;
		}
	}

	if (e->nx != 0)
	{
		ReverseDirection();
	}
}

void CGoomba::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (e->nx != 0) {
		ReverseDirection();
		goomba->ReverseDirection();
	}
}

void CGoomba::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (e->nx != 0) {
		ReverseDirection();
		if (koopa->GetState() != KOOPAS_STATE_WALKING) return;
		koopa->ReverseDirection();
	}
}

void CGoomba::HandleTimer(DWORD dt)
{
	if (dyingTimer.IsRunning()) {
		dyingTimer.Tick(dt);
	}
	if ((state == GOOMBA_STATE_DIE) && (dyingTimer.HasPassed(GOOMBA_DYING_TIMEOUT)))
	{
		dyingTimer.Reset();
		isDeleted = true;
		return;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!CGame::GetInstance()->IsInCamera(x - RESPAWN_OFFSET_CAM_X, DEFAULT_CAM_Y) &&
		!CGame::GetInstance()->IsInCamera(x + RESPAWN_OFFSET_CAM_X, DEFAULT_CAM_Y)) {
		isDeleted = true;
		return;
	}

	vy += ay * dt;

	if (isParagoomba) {
		if (state == PARAGOOMBA_STATE_WALKING) {
			paragoombaStateTimer.Tick(dt);
			float distanceTraveled = paragoombaStateTimer.getAccumulated() * abs(vx);
			if (fliesLeft > 0) {
				if (distanceTraveled >= PARAGOOMBA_WALKING_X_OFFSET * fliesLeft / PARAGOOMBA_TOTAL_FLIES) {
					CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
					float marioX, marioY;
					mario->GetPosition(marioX, marioY);
					if (marioX <= x) {
						nx = -1;
					}
					else {
						nx = 1;
					}
					vx = nx * GOOMBA_WALKING_SPEED;
				}
			}
			if (distanceTraveled >= PARAGOOMBA_WALKING_X_OFFSET) {
				SetState(PARAGOOMBA_STATE_PRE_FLY);
			}
		}
		else if (state == PARAGOOMBA_STATE_PRE_FLY) {
			paragoombaStateTimer.Tick(dt);
			if (paragoombaStateTimer.HasPassed(PARAGOOMBA_PRE_FLY_TIME)) {
				SetState(PARAGOOMBA_STATE_FLYING);
			}
		}
		else if (state == PARAGOOMBA_STATE_FLYING) {
		}
	}

	HandleTimer(dt);

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	if (state == GOOMBA_STATE_HIDDEN || state == GOOMBA_STATE_RESPAWNABLE) return;

	int aniId;
	int spriteId;
	CAnimations* animations = CAnimations::GetInstance();
	CSprites* s = CSprites::GetInstance();

	// color: 0: brown; 1: red
	if (state == GOOMBA_STATE_DIE || state == GOOMBA_STATE_DIE_WITH_BOUNCE) {
		if (state == GOOMBA_STATE_DIE) {
			if (color == 0) {
				spriteId = SPRITE_ID_BROWN_GOOMBA_DIE;
			}
			else if (color == 1) {
				spriteId = SPRITE_ID_RED_GOOMBA_DIE;
			}
		}
		else if (state == GOOMBA_STATE_DIE_WITH_BOUNCE) {
			if (color == 0) {
				spriteId = SPRITE_ID_YELOW_GOOMBA_DIE_UPSIDE_DOWN;
			}
			else if (color == 1) {
				spriteId = SPRITE_ID_RED_GOOMBA_DIE_UPSIDE_DOWN;
			}
		}

		s->Get(spriteId)->Draw(x, y);
	}
	else {
		if (state == GOOMBA_STATE_WALKING) {
			if (color == 0) {
				aniId = ID_ANI_BROWN_GOOMBA_WALKING;
			}
			else if (color == 1) {
				aniId = ID_ANI_RED_GOOMBA_WALKING;
			}
		}
		else if (state == PARAGOOMBA_STATE_WALKING) {
			if (color == 0) {
				// No brown paragoomba yet -> Use red paragoomba animation to avoid bugs
				aniId = ID_ANI_RED_PARAGOOMBA_WALKING;
			}
			else if (color == 1) {
				aniId = ID_ANI_RED_PARAGOOMBA_WALKING;
			}
		}
		else if (state == PARAGOOMBA_STATE_PRE_FLY) {
			if (color == 0) {
				// No brown paragoomba yet -> Use red paragoomba animation to avoid bugs
				aniId = ID_ANI_RED_PARAGOOMBA_PRE_FLY;
			}
			else if (color == 1) {
				aniId = ID_ANI_RED_PARAGOOMBA_PRE_FLY;
			}
		}
		else if (state == PARAGOOMBA_STATE_FLYING) {
			if (color == 0) {
				// No brown paragoomba yet -> Use red paragoomba animation to avoid bugs
				aniId = ID_ANI_RED_PARAGOOMBA_FLYING;
			}
			else if (color == 1) {
				aniId = ID_ANI_RED_PARAGOOMBA_FLYING;
			}
		}

		if (isParagoomba)
			animations->Get(aniId)->Render(x, y - (PARAGOOMBA_HEIGHT - GOOMBA_BBOX_HEIGHT) / 2);
		else
			animations->Get(aniId)->Render(x, y);
	}

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);

	CMario* mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float marioX, marioY;
	mario->GetPosition(marioX, marioY);

	switch (state)
	{
	case GOOMBA_STATE_DIE:
		vx = 0.0f;
		vy = 0.0f;
		ay = 0.0f;
		dyingTimer.Start();
		break;
	case GOOMBA_STATE_DIE_WITH_BOUNCE:
		vx = nx * KOOPAS_WALKING_SPEED;
		vy = -KOOPAS_KILL_Y_BOUNCE;
		dyingTimer.Start();
		break;
	case GOOMBA_STATE_WALKING:
		if (marioX <= x) {
			nx = -1;
		}
		else {
			nx = 1;
		}
		vx = nx * GOOMBA_WALKING_SPEED;
		break;
	case PARAGOOMBA_STATE_WALKING:
		paragoombaStateTimer.Reset();
		if (marioX <= x) {
			nx = -1;
		}
		else {
			nx = 1;
		}
		vx = nx * GOOMBA_WALKING_SPEED;
		ay = GOOMBA_GRAVITY;
		break;
	case PARAGOOMBA_STATE_PRE_FLY:
		paragoombaStateTimer.Reset();
		break;
	case PARAGOOMBA_STATE_FLYING:
		vy = -PARAGOOMBA_FLY_VY;
		if (fliesLeft > 0) {
			fliesLeft--;
		}
		break;
	}
}

void CGoomba::TakeDamageFrom(LPGAMEOBJECT obj)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (CMario* mario = dynamic_cast<CMario*>(obj)) {
		if (state != GOOMBA_STATE_DIE)
		{
			float vx, vy;
			mario->GetSpeed(vx, vy);
			mario->SetSpeed(vx, -MARIO_JUMP_DEFLECT_SPEED);
			if (isParagoomba) {
				mario->AddScore(x, y - (KOOPAS_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, FLYING_SCORE_TYPE_200, true);
			}
			else {
				mario->AddScore(x, y - (KOOPAS_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, FLYING_SCORE_TYPE_100, true);
			}
			if (isParagoomba) {
				SetState(GOOMBA_STATE_WALKING);
				isParagoomba = false;
			}
			else {
				SetState(GOOMBA_STATE_DIE);
			}
		}
		return;
	}
	if (CKoopa* koopa = dynamic_cast<CKoopa*>(obj)) {
		mario->AddScore(x, y - (GOOMBA_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, 100, true);
		SetState(GOOMBA_STATE_DIE_WITH_BOUNCE);
		return;
	}
}
