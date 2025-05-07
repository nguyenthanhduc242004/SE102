#include "Goomba.h"

CGoomba::CGoomba(float x, float y) :CGameObject(x, y)
{
	this->ay = GOOMBA_GRAVITY;
	nx = -1;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		top = y - GOOMBA_BBOX_HEIGHT / 2;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
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
	}

	if (e->nx != 0)
	{
		nx = -nx;
		vx = -vx;
	}
}

void CGoomba::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (e->nx != 0) {
		nx = -nx;
		vx = -vx;
		goomba->nx = -goomba->nx;
		goomba->vx = -goomba->vx;
	}
}

void CGoomba::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (e->nx != 0) {
		nx = -nx;
		vx = -vx;
		if (koopa->GetState() != KOOPAS_STATE_WALKING) return;
		koopa->SetDirection(-koopa->GetDirection());
		koopa->SetState(KOOPAS_STATE_WALKING);
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
	vy += ay * dt;
	vx += ax * dt;

	HandleTimer(dt);

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0.0f;
		vy = 0.0f;
		ay = 0.0f;
		dyingTimer.Start();
		break;
	case GOOMBA_STATE_DIE_WITH_BOUNCE:
		vx = nx * KOOPAS_WALKING_SPEED;
		vy = -KOOPAS_KILL_Y_BOUNCE;
		break;
	case GOOMBA_STATE_WALKING:
		vx = nx * GOOMBA_WALKING_SPEED;
		break;
	}
}

void CGoomba::TakeDamageFrom(LPGAMEOBJECT obj)
{
	if (CMario* mario = dynamic_cast<CMario*>(obj)) {
		if (state != GOOMBA_STATE_DIE)
		{
			float vx, vy;
			mario->GetSpeed(vx, vy);
			mario->SetSpeed(vx, -MARIO_JUMP_DEFLECT_SPEED);
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			mario->AddScore(x, y - (KOOPAS_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, FLYING_SCORE_TYPE_100, true);
			SetState(GOOMBA_STATE_DIE);
		}
		return;
	}
	if (CKoopa* koopa = dynamic_cast<CKoopa*>(obj)) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->AddScore(x, y - (GOOMBA_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, 100, true);
		SetState(GOOMBA_STATE_DIE_WITH_BOUNCE);
		return;
	}
}
