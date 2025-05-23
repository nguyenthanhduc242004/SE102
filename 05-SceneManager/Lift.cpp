#include "Lift.h"

void CLift::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!CGame::GetInstance()->IsInCamera(x - RESPAWN_OFFSET_CAM_X, DEFAULT_CAM_Y) &&
		!CGame::GetInstance()->IsInCamera(x + RESPAWN_OFFSET_CAM_X, DEFAULT_CAM_Y))
		return;

	x += vx * dt;
	y += vy * dt;

	vy += ay * dt;

	HandleTimer(dt);
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLift::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (CMario* mario = dynamic_cast<CMario*>(e->obj)) {
		if (!e->nx && !e->ny && !e->dx && !e->dy) {
			float ml, mt, mr, mb, mx, my, l, t, r, b;
			mario->GetBoundingBox(ml, mt, mr, mb);
			mario->GetPosition(mx, my);
			GetBoundingBox(l, t, r, b);

			// Calculate overlap on both axes
			float overlapLeft = mr - l;
			float overlapRight = r - ml;
			float overlapTop = mb - t;
			float overlapBottom = b - mt;

			// Find the minimal translation axis
			float minOverlapX = (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
			float minOverlapY = (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;

			if (std::abs(minOverlapX) < std::abs(minOverlapY)) {
				// Push mario along X
				mx += minOverlapX;
			}
			else {
				// Push mario along Y
				my += minOverlapY;
			}
			mario->SetPosition(mx, my);
		}
	}
}

void CLift::HandleTimer(DWORD dt) {
	if (startFallingTimer.IsRunning()) startFallingTimer.Tick(dt);
	if (respawnTimer.IsRunning()) respawnTimer.Tick(dt);

	if (startFallingTimer.HasPassed(LIFT_START_FALLING_TIME)) {
		startFallingTimer.Reset();
		ay = GRAVITY * 0.08f;
	}
	if (respawnTimer.HasPassed(LIFT_RESPAWN_TIME)) {
		if (!CGame::GetInstance()->IsInCamera(x0, y0)) {
			respawnTimer.Reset();
			x = x0;
			y = y0;
			SetState(LIFT_STATE_GLIDING);
		}
	}
}

void CLift::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case LIFT_STATE_GLIDING:
		vx = nx * LIFT_SPEED_X;
		break;
	case LIFT_STATE_FALLING:
		vx = 0;
		startFallingTimer.Start();
		respawnTimer.Start();
		break;
	}
}
