#include "BoBro.h"


CBoBro::CBoBro(float x, float y)
	: CGameObject(x, y)
{
	x0 = x;
	nx = -1;
	SetState(BOOMERANG_BROTHER_STATE_WALKING);
	reloadTimer.Start();
	ay = BB_GRAVITY;
}

void CBoBro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BOOMERANG_BROTHER_BBOX_WIDTH / 2;
	top = y - BOOMERANG_BROTHER_BBOX_HEIGHT / 2;
	right = left + BOOMERANG_BROTHER_BBOX_WIDTH;
	bottom = top + BOOMERANG_BROTHER_BBOX_HEIGHT;
}

void CBoBro::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CBoBro::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (CBoomerang* boom = dynamic_cast<CBoomerang*>(e->obj)) {
		if (boom->GetState() >= BOOM_STATE_5) {
			boom->Delete();
			boomerangsThrown--;
		}
	}
	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0) vy = 0;
	if (e->nx != 0)
		ReverseDirection();
}

void CBoBro::HoldBoomerang()
{
	boomerang = new CBoomerang(x, y);
	CGame::GetInstance()->GetCurrentScene()->AddObject(boomerang);
}

void CBoBro::ThrowBoomerang()
{
	if (boomerang && !boomerang->IsDeleted()) boomerang->SetState(BOOM_STATE_1);
}

void CBoBro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	// patrol
	if (x < x0) vx = BB_SPEED;
	if (x > x0 + BB_LIMIT_RANGE) vx = -BB_SPEED;

	// face Mario at all time, thus keep in update
	CMario* m = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float mX, mY;
	m->GetPosition(mX, mY);
	if (m) nx = (mX > x) ? 1 : -1;


	// throw logic all handled in timers
	// stop bro throwing when he dies, or he wont stop until he is deleted...
	HandleTimer(dt);

	if (boomerang && boomerang->GetState() == BOOM_STATE_IDLE) {
		boomerang->SetPosition(x, y);
		boomerang->SetDirection(nx);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBoBro::Render()
{
	int aniId = (nx > 0)
		? (aimTimer.IsRunning() ? ID_ANI_BOOMERANG_BROTHER_AIM_RIGHT : ID_ANI_BOOMERANG_BROTHER_WALK_RIGHT)
		: (aimTimer.IsRunning() ? ID_ANI_BOOMERANG_BROTHER_AIM_LEFT : ID_ANI_BOOMERANG_BROTHER_WALK_LEFT);

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
}
void CBoBro::HandleTimer(DWORD dt) {
	if (state == BOOMERANG_BROTHER_STATE_DIE) return;
	if (aimTimer.IsRunning()) aimTimer.Tick(dt);
	if (throwTimer.IsRunning()) throwTimer.Tick(dt);
	if (chargeTimer.IsRunning()) chargeTimer.Tick(dt);
	if (reloadTimer.IsRunning()) reloadTimer.Tick(dt);

	if (aimTimer.HasPassed(BB_AIM_TIME)) {
		aimTimer.Reset();
		if (boomerangsThrown < BB_MAX_BOOMS) {
			HoldBoomerang();
			throwTimer.Start();
		}
		else chargeTimer.Start();
	}
	if (throwTimer.HasPassed(BB_THROW_TIME)) {
		throwTimer.Reset();
		ThrowBoomerang();
		boomerangsThrown++;
		reloadTimer.Start();
	}
	if (chargeTimer.HasPassed(BB_CHARGE_TIME)) {
		chargeTimer.Reset();
		boomerangsThrown = 0;
		aimTimer.Start();
	}
	if (reloadTimer.HasPassed(BB_RELOAD_TIME)) {
		float mX, mY;
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition(mX, mY);
		if (abs(x - mX) <= BB_ACTIVE_RANGE) {
			reloadTimer.Reset();
			aimTimer.Start();
		}
	}
}

void CBoBro::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case BOOMERANG_BROTHER_STATE_WALKING:
		vx = nx * BB_SPEED;
		break;
	case BOOMERANG_BROTHER_STATE_DIE:
		vy = BB_SPEED * 0.003;
		break;
	}
}

void CBoBro::TakeDamageFrom(LPGAMEOBJECT obj) {
	SetState(BOOMERANG_BROTHER_STATE_DIE);
	CMario* mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario->AddScore(x, y - BOOMERANG_BROTHER_BBOX_HEIGHT / 2 - FLYING_SCORE_HEIGHT, FLYING_SCORE_TYPE_100, true);
}