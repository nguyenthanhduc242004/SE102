#include "Koopa.h"
#include "debug.h"
#include "Game.h"
#include "Platform.h"


void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - KOOPAS_BBOX_WIDTH / 2;
	right = x + KOOPAS_BBOX_WIDTH / 2;

	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_REVIVING || state == KOOPAS_STATE_SPINNING_LEFT || state == KOOPAS_STATE_SPINNING_RIGHT)
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

	// timer handling funtion seperately
	HandleTimer(dt);

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
	case KOOPAS_STATE_REVIVING:
		aniId = ID_ANI_KOOPAS_REVIVING;
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
	//check all npc / which this npc can go through/interact ---> solve Mario triggering collision event multiple times
	if (dynamic_cast<CKoopa*>(e->obj)) {
		OnCollisionWithKoopa(e);
		return;
	}
	if (dynamic_cast<CGoomba*>(e->obj)) {
		OnCollisionWithGoomba(e);
		return;
	}

	//IsBlocking will be used only to check map objects ---> NPCs are not blocking, then Mario can finally go through it.
	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0)
	{
		vy = 0;
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
	if (state != KOOPAS_STATE_SPINNING_LEFT && state != KOOPAS_STATE_SPINNING_RIGHT) {
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
	if (state == KOOPAS_STATE_SPINNING_LEFT || state == KOOPAS_STATE_SPINNING_RIGHT) {
		CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);
		if (e->nx != 0) {
			if (questionBlock->GetState() != QUESTION_BLOCK_STATE_DISABLED)
			{
				questionBlock->SetState(QUESTION_BLOCK_STATE_DISABLED);
			}
		}
	}
	nx = -nx;
	vx = -vx;
}
void CKoopa::OnCollisionWithBrick(LPCOLLISIONEVENT e) {
	if (state == KOOPAS_STATE_SPINNING_LEFT || state == KOOPAS_STATE_SPINNING_RIGHT) {
		if (e->nx != 0) {
			e->obj->Delete();
			//will create break state later
			return;
		}
	}
	nx = -nx;
	vx = -vx;
}
void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	int otherState = koopa->GetState();

	bool thisSpinning = (state == KOOPAS_STATE_SPINNING_LEFT || state == KOOPAS_STATE_SPINNING_RIGHT);
	bool otherSpinning = (otherState == KOOPAS_STATE_SPINNING_LEFT || otherState == KOOPAS_STATE_SPINNING_RIGHT);
	bool otherWalking = (otherState == KOOPAS_STATE_WALKING_LEFT || otherState == KOOPAS_STATE_WALKING_RIGHT);
	bool otherShell = (otherState == KOOPAS_STATE_SHELL);

	if (thisSpinning && otherSpinning)
	{
		this->SetState(KOOPAS_STATE_SHELL);
		koopa->SetState(KOOPAS_STATE_SHELL);
	}
	else if (thisSpinning && otherWalking)
	{
		koopa->SetState(KOOPAS_STATE_DIE);
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->AddScore(x, y - (KOOPAS_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, FLYING_SCORE_TYPE_100, true);
		return;
	}
	else if (thisSpinning && otherShell)
	{
		//spinning the opposite direction
		koopa->SetState(nx < 0 ? KOOPAS_STATE_SPINNING_LEFT : KOOPAS_STATE_SPINNING_RIGHT);
	}
}
void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if ((state == KOOPAS_STATE_SPINNING_LEFT || state == KOOPAS_STATE_SPINNING_RIGHT) || isHeld) {
		// kill the goomba
		float x, y;
		goomba->GetPosition(x, y);
		mario->AddScore(x, y - (GOOMBA_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, 100, true);
		goomba->SetState(GOOMBA_STATE_DIE);
		return;
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

	// shell revival / only work when in shell
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
		(nx < 0) ? SetState(KOOPAS_STATE_WALKING_LEFT) : SetState(KOOPAS_STATE_WALKING_RIGHT);

	}
	// die
	if ((state == KOOPAS_STATE_DIE) && (dyingTimer.HasPassed(KOOPAS_DYING_TIMEOUT)))
	{
		dyingTimer.Reset();
		isDeleted = true;
		return;
	}
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case KOOPAS_STATE_WALKING_LEFT:
		nx = -1;
		vx = -KOOPAS_WALKING_SPEED;
		isHeld = false;
		break;
	case KOOPAS_STATE_WALKING_RIGHT:
		nx = 1;
		vx = KOOPAS_WALKING_SPEED;
		isHeld = false;
		break;
	case KOOPAS_STATE_SHELL:
		vx = 0;
		shellTimer.Start();
		break;
	case KOOPAS_STATE_REVIVING:
		revivingTimer.Start();
		break;
	case KOOPAS_STATE_SPINNING_LEFT:
		//DebugOut(L"this is spinning left");
		nx = -1;
		vx = -KOOPAS_SPINNING_SPEED;
		isHeld = false;
		break;
	case KOOPAS_STATE_SPINNING_RIGHT:
		//DebugOut(L"this is spinning left");
		nx = 1;
		vx = KOOPAS_SPINNING_SPEED;
		isHeld = false;
		break;
	case KOOPAS_STATE_DIE:
		dyingTimer.Start();
		ay = 0.0f;
		vx = 0.0f;
		vy = 0.0f;
		//DebugOut(L"this is dead");
		vx = 0;
		break;
	}
}
