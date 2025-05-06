#include "Koopa.h"

#include <typeinfo>


void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - KOOPAS_BBOX_WIDTH / 2;
	right = x + KOOPAS_BBOX_WIDTH / 2;

	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_REVIVING || state == KOOPAS_STATE_SPINNING)
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

	HandleTimer(dt);
	if (isHeld)	HandleBeingHeld();

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}


// --- Render()
void CKoopa::Render()
{
	int aniId = -1;
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		aniId = (nx < 0)
			? ID_ANI_KOOPAS_WALKING_LEFT
			: ID_ANI_KOOPAS_WALKING_RIGHT;
		break;
	case KOOPAS_STATE_SHELL:
		aniId = ID_ANI_KOOPAS_SHELL;
		break;
	case KOOPAS_STATE_REVIVING:
		aniId = ID_ANI_KOOPAS_REVIVING;
		break;
	case KOOPAS_STATE_SPINNING:
		aniId = (nx < 0)
			? ID_ANI_KOOPAS_SPINNING_LEFT
			: ID_ANI_KOOPAS_SPINNING_RIGHT;
		break;
	case KOOPAS_STATE_DIE:
		aniId = ID_ANI_KOOPAS_DIE;
		break;
	case KOOPAS_STATE_DIE_WITH_BOUNCE:
		// for now
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

void CKoopa::HandleBeingHeld()
{
	CMario* mario = (CMario*)((CPlayScene*)CGame::GetInstance()
		->GetCurrentScene())->GetPlayer();

	if (mario->IsHolding())
	{
		// Keep shell attached to Mario’s hand
		nx = mario->GetDirection();
		float mX, mY;
		mario->GetPosition(mX, mY);
		y = mY + KOOPAS_HOLDING_Y_OFFSET;
		x = mX + nx * (mario->GetLevel() == MARIO_LEVEL_SMALL
			? MARIO_SMALL_BBOX_WIDTH
			: MARIO_BIG_BBOX_WIDTH);
		if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			y -= KOOPAS_HOLDING_SMALL_MARIO_Y_ADJUST;
	}
	else
	{
		// Mario just released the shell -> kick it
		mario->Kick();
		SetState(KOOPAS_STATE_SPINNING);
	}
}


void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	// Mario will handle logic of colliding with Koopa, skip for no duplication (since being collided will now call events too)
	if (dynamic_cast<CMario*>(e->obj)) return;
	//check all npc / which this npc can go through/interact ---> solve Mario triggering collision event multiple times because of IsBlocking setting
	if (dynamic_cast<CKoopa*>(e->obj)) {
		OnCollisionWithKoopa(e);
		return;
	}
	if (dynamic_cast<CGoomba*>(e->obj)) {
		OnCollisionWithGoomba(e);
		return;
	}

	//IsBlocking will be used only to check map objects ---> NPCs are not blocking, Mario can go through them.
	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0)
	{
		vy = 0;
	}

	//static overlap arguments, when Koopa is passively collided, or it is overlapping
	//use for when Mario kicking it directly into a block
	if (e->nx == 0 && e->ny == 0 && e->dx == 0 && e->dy == 0) {
		// Shell is held, basically invincible
		if (isHeld) return;
		// overlapping platforms are allowed, except CSideCollidablePlatform
		if (dynamic_cast<CPlatform*>(e->obj) != NULL && dynamic_cast<CSideCollidablePlatform*>(e->obj) == NULL) return;
		// center of shell goes inside bounding box, kill, else, dont, still count as hitting against surface
		float l, t, r, b;
		e->obj->GetBoundingBox(l, t, r, b);
		if (x < l) return;
		if (y < t) return;
		if (x > r) return;
		if (y > b) return;

		/// Debug code
		// Convert mangled name (char*) to wstring for DebugOut
		//std::string className = typeid(*e->obj).name();
		//std::wstring wideClassName(className.begin(), className.end()); // crude but readable
		//DebugOut(L"[INFO] Object: %s\n", wideClassName.c_str());
		///

		SetState(KOOPAS_STATE_DIE);
		SetSpeed(nx * KOOPAS_WALKING_SPEED, -KOOPAS_KILL_Y_BOUNCE);
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->AddScore(x, y - (KOOPAS_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, FLYING_SCORE_TYPE_100, true);
		return;
	}

	if (dynamic_cast<CQuestionBlock*>(e->obj)) {
		OnCollisionWithQuestionBlock(e);
		return;
	}

	if (dynamic_cast<CPlatform*>(e->obj)) {
		OnCollisionWithPlatform(e);
		return;
	}
	if (dynamic_cast<CBrick*>(e->obj)) {
		OnCollisionWithBrick(e);
		return;
	}
	// process all blocks, handle the turning seperately depending on the type and state, if not falling into any of the cases, turn automatically.
	if (e->nx != 0) {
		nx = -nx;
		vx = -vx;
	}
}
void CKoopa::OnCollisionWithPlatform(LPCOLLISIONEVENT e) {
	if (state != KOOPAS_STATE_SPINNING && state != KOOPAS_STATE_DIE) {
		if (!IsGroundAhead(e)) {
			nx = -nx;
			vx = -vx;
		}
	}
	if (e->nx != 0 && dynamic_cast<CSideCollidablePlatform*>(e->obj)) {
		nx = -nx;
		vx = -vx;
	}
}
void CKoopa::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e)
{
	if (state == KOOPAS_STATE_SPINNING) {
		CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);
		if (e->nx != 0) {
			if (questionBlock->GetState() != QUESTION_BLOCK_STATE_DISABLED)
			{
				questionBlock->SetState(QUESTION_BLOCK_STATE_DISABLED);
			}
		}
	}
	if (e->nx != 0) {
		nx = -nx;
		vx = -vx;
	}
}
void CKoopa::OnCollisionWithBrick(LPCOLLISIONEVENT e) {
	if (state == KOOPAS_STATE_SPINNING) {
		if (e->nx != 0) {
			e->obj->Delete();
			//will create break state later
			return;
		}
	}
	if (e->nx != 0) {
		nx = -nx;
		vx = -vx;
	}
}
void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	int otherState = koopa->GetState();

	bool thisSpinning = (state == KOOPAS_STATE_SPINNING);
	bool otherSpinning = (otherState == KOOPAS_STATE_SPINNING);
	bool otherWalking = (otherState == KOOPAS_STATE_WALKING);
	bool otherShell = (otherState == KOOPAS_STATE_SHELL);

	if ((thisSpinning || isHeld) && (otherSpinning || otherShell))
	{
		//kill both
		koopa->SetDirection(nx);
		koopa->SetState(KOOPAS_STATE_DIE_WITH_BOUNCE);
		nx = -nx;
		this->SetState(KOOPAS_STATE_DIE_WITH_BOUNCE);

		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->AddScore(x, y - (KOOPAS_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, FLYING_SCORE_TYPE_200, true);
		return;
	}
	if ((thisSpinning || isHeld) && otherWalking)
	{
		koopa->SetDirection(nx);
		koopa->SetState(KOOPAS_STATE_DIE_WITH_BOUNCE);

		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->AddScore(x, y - (KOOPAS_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, FLYING_SCORE_TYPE_100, true);
		return;
	}
	if (e->nx != 0) {
		nx = -nx;
		vx = -vx;
	}
}
void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (state == KOOPAS_STATE_SPINNING || isHeld) {
		// kill the goomba
		goomba->SetDirection(nx);
		goomba->SetState(GOOMBA_STATE_DIE_WITH_BOUNCE);

		float x, y;
		goomba->GetPosition(x, y);
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->AddScore(x, y - (GOOMBA_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, 100, true);
		return;
	}
	if (e->nx != 0) {
		nx = -nx;
		vx = -vx;
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
void CKoopa::HandleTimer(DWORD dt) {
	if (shellTimer.IsRunning()) {
		shellTimer.Tick(dt);
	}
	if (revivingTimer.IsRunning()) {
		revivingTimer.Tick(dt);
	}
	if (dyingTimer.IsRunning()) {
		dyingTimer.Tick(dt);
	}
	//

	// shell revival/shaking / only work when in shell
	if (state == KOOPAS_STATE_SHELL && shellTimer.HasPassed(KOOPAS_SHELL_TIMEOUT)) {
		shellTimer.Reset();
		// shaking out of shell
		SetState(KOOPAS_STATE_REVIVING);
	}
	// shell emerge / only work when in shell
	if (state == KOOPAS_STATE_REVIVING && revivingTimer.HasPassed(KOOPAS_REVIVING_TIMEOUT)) {
		revivingTimer.Reset();
		// popping out of shell
		y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) + 1;
		SetState(KOOPAS_STATE_WALKING);

	}
	// die
	if ((state == KOOPAS_STATE_DIE) && (dyingTimer.HasPassed(KOOPAS_DYING_TIMEOUT)))
	{
		dyingTimer.Reset();
		isDeleted = true;
		return;
	}
}
void CKoopa::SetState(int newState)
{
	CGameObject::SetState(newState);
	switch (newState)
	{
	case KOOPAS_STATE_WALKING:
		ay = KOOPAS_GRAVITY;
		vx = nx * KOOPAS_WALKING_SPEED;
		isHeld = false;
		break;

	case KOOPAS_STATE_SHELL:
		vx = 0.0f;
		ay = 0.0f;
		isHeld = false;
		shellTimer.Start();
		break;

	case KOOPAS_STATE_REVIVING:
		vx = 0.0f;
		ay = KOOPAS_GRAVITY;
		revivingTimer.Start();
		break;

	case KOOPAS_STATE_SPINNING:
		ay = KOOPAS_GRAVITY;
		vx = nx * KOOPAS_SPINNING_SPEED;
		isHeld = false;
		break;

	case KOOPAS_STATE_DIE:
		vx = 0.0f;
		vy = 0.0f;
		ay = 0.0f;
		isHeld = false;
		dyingTimer.Start();
		break;
	case KOOPAS_STATE_DIE_WITH_BOUNCE:
		vx = nx * KOOPAS_WALKING_SPEED;
		vy = -KOOPAS_KILL_Y_BOUNCE;
		ay = KOOPAS_GRAVITY;
		isHeld = false;
		dyingTimer.Start();
		break;
	}
}

//void CKoopa::OnCollisionWithPlant(LPCOLLISIONEVENT e)
//{

	//// plants die if you spin into them or hold shell into them
	//if (state == KOOPAS_STATE_SPINNING_LEFT
	//	|| state == KOOPAS_STATE_SPINNING_RIGHT
	//	|| isHeld)
	//{
	//CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	// 
	//	mario->AddScore(x, y, 100, true);
	//	plant->SetState(PIRANHAPLANT_STATE_DEATH);
	//	plant->isDestroyed = true;
	//}
//}

//void CKoopa::OnCollisionWithMusicalBrick(LPCOLLISIONEVENT e)
//{
	//if (state == KOOPAS_STATE_SPINNING_LEFT || state == KOOPAS_STATE_SPINNING_RIGHT) {
		 //bounce back
		//nx = -nx;  vx = -vx;
		//mb->SetState(MUSIC_BRICK_STATE_HIT_FROM_TOP);
	//}
//}