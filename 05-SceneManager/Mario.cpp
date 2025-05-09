#include "Mario.h"
#include <algorithm>
#include "debug.h"
#include "Game.h"
#include "PlayScene.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	HandleTimer(dt);

	//there should be a mechanism to ease from running to walking, currently it just cuts down immediately
	if (vx > 0.0f) {
		vx -= dragX * dt;
		if (vx < 0.0f)	vx = 0.0f;
	}
	else if (vx < 0.0f) {
		vx += dragX * dt;
		if (vx > 0.0f)	vx = 0.0f;
	}

	if (abs(vx) > abs(maxVx) && vx * maxVx > 0) vx = maxVx;
	if (vy > maxVy) vy = maxVy;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e) {

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0.0f;
		if (e->ny < 0) {
			isOnPlatform = true;
			dragX = MARIO_DRAG_X;
		}
	}
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0.0f;
	}

	if (dynamic_cast<CGoomba*>(e->obj)) {
		OnCollisionWithGoomba(e);
		return;
	}
	if (dynamic_cast<CKoopa*>(e->obj)) {
		OnCollisionWithKoopa(e);
		return;
	}
	if (dynamic_cast<CQuestionBlock*>(e->obj)) {
		OnCollisionWithQuestionBlock(e);
		return;
	}
	if (dynamic_cast<CCoin*>(e->obj)) {
		OnCollisionWithCoin(e);
		return;
	}
	if (dynamic_cast<CPortal*>(e->obj)) {
		OnCollisionWithPortal(e);
		return;
	}
	if (dynamic_cast<CMushroom*>(e->obj)) {
		OnCollisionWithMushroom(e);
		return;
	}
	if (dynamic_cast<CPiranhaPlant*>(e->obj)) {
		OnCollisionWithPiranhaPlant(e);
	}
	if (dynamic_cast<CBullet*>(e->obj)) {
		OnCollisionWithBullet(e);
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		goomba->TakeDamageFrom(this);
	}
	else // hit by Goomba
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_DIE_WITH_BOUNCE)
		{
			TakeDamageFrom(goomba);
		}
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e) {
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	// hit koopa in shell in any direction
	if (koopa->GetState() == KOOPAS_STATE_SHELL || koopa->GetState() == KOOPAS_STATE_REVIVING) {
		if (!isReadyToHold || e->ny != 0) {
			Kick();
			koopa->SetDirection(nx);
			koopa->SetState(KOOPAS_STATE_SPINNING);
		}
		else {
			isHolding = true;
			koopa->SetIsHeld(true);
		}
	}
	//hit koopa not in shell on top, can turn koopa ---> shell
	else if (e->ny < 0)
	{
		koopa->TakeDamageFrom(this);
	}
	//touch koopa not in shell, not hit on top, koopa can be any other state, just not dead
	else if (koopa->GetState() != KOOPAS_STATE_DIE && koopa->GetState() != KOOPAS_STATE_DIE_WITH_BOUNCE)
	{
		TakeDamageFrom(koopa);
	}
}

void CMario::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e)
{
	CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);

	// hit the bottom of QuestionBlock >> Change the QuestionBlock state
	if (e->ny > 0)
	{
		if (questionBlock->GetState() != QUESTION_BLOCK_STATE_DISABLED)
		{
			questionBlock->SetState(QUESTION_BLOCK_STATE_DISABLED);
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
	if (mushroom->GetState() != MUSHROOM_STATE_WALKING) return;
	int type = mushroom->GetType();
	if (type == MUSHROOM_TYPE_RED) {
		if (level < MARIO_LEVEL_BIG) {
			SetLevel(MARIO_LEVEL_BIG);
		}
		AddScore(x, y - MARIO_BIG_BBOX_HEIGHT / 2, FLYING_SCORE_TYPE_1000, true);
		//add score later
	}
	else if (type == MUSHROOM_TYPE_GREEN) {
		//add life later
		//add score later
	}
	mushroom->Delete();
}

void CMario::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e)
{
	CPiranhaPlant* piranhaPlant = dynamic_cast<CPiranhaPlant*>(e->obj);

	if (piranhaPlant->GetState() != PIRANHA_PLANT_STATE_HIDDEN)
	{
		TakeDamageFrom(piranhaPlant);
	}
}

void CMario::OnCollisionWithBullet(LPCOLLISIONEVENT e)
{
	TakeDamageFrom(e->obj);
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0.0f)
			{
				if (ax <= 0.0f)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (vx >= MARIO_RUNNING_SPEED)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;

			}
			else // vx < 0
			{
				if (ax >= 0.0f)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (vx <= -MARIO_RUNNING_SPEED)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0.0f)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0.0f)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (vx >= MARIO_RUNNING_SPEED)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0.0f)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (vx <= -MARIO_RUNNING_SPEED)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();

	//DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	//DebugOutTitle(L"State: %d", state);
	//DebugOut(L"maxVx: %f\n", maxVx);
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		//if airborn reaching halt, reset gravity, mario drops heavier
		if (!isOnPlatform && vy >= 0.0f)
		{
			ay = MARIO_GRAVITY;
		}
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		if (!isOnPlatform && vy >= 0.0f)
		{
			ay = MARIO_GRAVITY;
		}
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		if (!isOnPlatform && vy >= 0.0f)
		{
			ay = MARIO_GRAVITY;
		}
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		if (!isOnPlatform && vy >= 0.0f)
		{
			ay = MARIO_GRAVITY;
		}
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
			//airborn, gravity will be lifted, drag reduces
			ay = MARIO_LIFTED_GRAVITY;
			dragX = MARIO_AIR_DRAG_X;
			isOnPlatform = false;
		}
		break;
	case MARIO_STATE_RELEASE_JUMP:
		//release jump reset gravity back
		ay = MARIO_GRAVITY;
		break;

	case MARIO_STATE_SIT:
		if (ax != 0) break;
		if (isHolding) break;
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		if (!isOnPlatform && vy >= 0.0f)
		{
			ay = MARIO_GRAVITY;
		}
		break;

	case MARIO_STATE_DIE:
		vx = 0.0f;
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		ax = 0.0f;
		ay = MARIO_LIFTED_GRAVITY * 2;
		isHolding = false;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::AddScore(float x, float y, int value, bool showEffect)
{
	score += value;

	if (showEffect)
	{
		// create a score effect popup at (x, y)
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		scene->AddObject(new CFlyingScore(x, y, value));
	}
}

void CMario::HandleTimer(DWORD dt)
{
	if (invincibleTimer.IsRunning()) {
		invincibleTimer.Tick(dt);
	}
	if (kickTimer.IsRunning()) {
		kickTimer.Tick(dt);
	}
	// reset timer if time has passed
	if (invincibleTimer.HasPassed(MARIO_UNTOUCHABLE_TIME))
	{
		invincibleTimer.Reset();
		untouchable = 0;
	}
	if (kickTimer.HasPassed(MARIO_KICK_TIME)) {
		kickTimer.Reset();
		untouchable = 0;
	}
}

