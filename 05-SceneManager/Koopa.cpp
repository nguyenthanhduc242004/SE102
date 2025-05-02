#include "Koopa.h"
#include "debug.h"
#include "Game.h"
#include "Platform.h"


void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - KOOPAS_BBOX_WIDTH / 2;
	right = x + KOOPAS_BBOX_WIDTH / 2;

	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_SPINNING_LEFT)
	{
		top = y - KOOPAS_BBOX_HEIGHT_SHELL / 2;
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL / 2;
	}
	else
	{
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		bottom = y + KOOPAS_BBOX_HEIGHT / 2;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	if (shellTimer.IsRunning()) {
		shellTimer.Tick(dt);
	}
	// shell revival
	if (state == KOOPAS_STATE_SHELL && shellTimer.HasPassed(KOOPAS_SHELL_TIMEOUT)) {
		shellTimer.Reset();

		// pop out of shell
		y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) + 1;
		(nx < 0) ? SetState(KOOPAS_STATE_WALKING_LEFT) : SetState(KOOPAS_STATE_WALKING_RIGHT);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}

void CKoopa::Render()
{
	int aniId = -1;

	switch (state)
	{
	case KOOPAS_STATE_WALKING_LEFT:
		aniId = ID_ANI_KOOPAS_WALKING_LEFT;
	case KOOPAS_STATE_WALKING_RIGHT:
		aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
		break;
	case KOOPAS_STATE_SHELL:
		aniId = ID_ANI_KOOPAS_SHELL;
		break;
	case KOOPAS_STATE_SPINNING_LEFT:
		aniId = ID_ANI_KOOPAS_SPINNING;
		break;
	case KOOPAS_STATE_SPINNING_RIGHT:
		aniId = ID_ANI_KOOPAS_SPINNING;
	case KOOPAS_STATE_DIE:
		aniId = ID_ANI_KOOPAS_DIE;
		break;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
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
	if (state != KOOPAS_STATE_SPINNING_LEFT && state != KOOPAS_STATE_SPINNING_RIGHT) {
		if (dynamic_cast<CPlatform*>(e->obj)) {
			if (!IsGroundAhead(e)) {
				nx = -nx;
				vx = -vx;
			}
		}
	}
}

bool CKoopa::IsGroundAhead(LPCOLLISIONEVENT e)
{
	float probeX = x + (vx > 0 ? KOOPAS_BBOX_WIDTH / 2 : -KOOPAS_BBOX_WIDTH / 2);
	float probeY = y + KOOPAS_BBOX_HEIGHT / 2 + 1; // 1 pixel below the bottom

	float l, t, r, b;
	e->obj->GetBoundingBox(l, t, r, b);

	// Check if probe point is inside the object's bounding box
	if (probeX >= l && probeX <= r && probeY >= t && probeY <= b)
	{
		return true; // Solid ground detected
	}

	return false; // No ground
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case KOOPAS_STATE_WALKING_LEFT:
		nx = -1;
		vx = -KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_WALKING_RIGHT:
		nx = 1;
		vx = KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_SHELL:
		vx = 0;
		shellTimer.Start();
		break;
	case KOOPAS_STATE_SPINNING_LEFT:
		//DebugOut(L"this is spinning left");
		nx = -1;
		vx = -KOOPAS_SPINNING_SPEED;
		break;
	case KOOPAS_STATE_SPINNING_RIGHT:
		//DebugOut(L"this is spinning left");
		nx = 1;
		vx = KOOPAS_SPINNING_SPEED;
		break;
	case KOOPAS_STATE_DIE:
		//DebugOut(L"this is dead");
		vx = 0;
		break;
	}
}
