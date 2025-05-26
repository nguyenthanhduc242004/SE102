#include "Mario.h"


void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	tanookiTail->SetPosition(x, y);

	vy += ay * dt;
	vx += ax * dt;

	HandleTimer(dt);

	if (travellingThroughPipeTimer->IsRunning()) {
		travellingThroughPipeTimer->Tick(dt);
		vx = 0;
		ay = 0;
		if (isAscendingThroughPipe)
			vy = -MARIO_TRAVELLING_THROUGH_PIPE_SPEED;
		else if (isDescendingThroughPipe)
			vy = MARIO_TRAVELLING_THROUGH_PIPE_SPEED;
	}
	if (isDescendingThroughPipe && travellingThroughPipeTimer->HasPassed(MARIO_TRAVELLING_THROUGH_PIPE_TIME)) {
		if (toX > 0 && toY > 0) {
			SetPosition(toX, toY);
			toX = -1;
			toY = -1;
		}

		if (isAscendingAfterTravellingThroughPipe) {
			vy = -MARIO_TRAVELLING_THROUGH_PIPE_SPEED;
		}
		if (!isAscendingAfterTravellingThroughPipe || travellingThroughPipeTimer->HasPassed(MARIO_TRAVELLING_THROUGH_PIPE_TIME * 2)) {
			travellingThroughPipeTimer->Reset();
			isAscendingAfterTravellingThroughPipe = false;
			isDescendingThroughPipe = false;
			SetState(MARIO_STATE_IDLE);
			ay = MARIO_GRAVITY;
		}
	}
	if (isAscendingThroughPipe && travellingThroughPipeTimer->HasPassed(MARIO_TRAVELLING_THROUGH_PIPE_TIME)) {
		if (toX > 0 && toY > 0) {
			SetPosition(toX, toY);
			toX = -1;
			toY = -1;
		}
		if (!isAscendingAfterTravellingThroughPipe || travellingThroughPipeTimer->HasPassed(MARIO_TRAVELLING_THROUGH_PIPE_TIME * 2)) {
			travellingThroughPipeTimer->Reset();
			SetState(MARIO_STATE_IDLE);
			ay = MARIO_GRAVITY;
			isAscendingThroughPipe = false;
			isAscendingAfterTravellingThroughPipe = false;
		}
	}

	if (tailWagTimer.IsRunning()) {
		if (flyTimer.IsRunning())
			vy = -MARIO_FLY_SPEED;
		else
			vy = MARIO_SLOW_FALL_SPEED;
	}
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
	if (!isOnPlatformTimer.IsRunning())
		isOnPlatform = false;
	//isHittingWall = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0.0f;
		if (e->ny < 0) {
			isOnPlatform = true;
			isOnPlatformTimer.Start();
			dragX = MARIO_DRAG_X;
		}
	}
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0.0f;
	}

	if (e->ny < 0 && !dynamic_cast<CGoomba*>(e->obj) && !dynamic_cast<CKoopa*>(e->obj)) {
		stompingStreak = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj)) {
		OnCollisionWithGoomba(e);
		return;
	}
	if (dynamic_cast<CKoopa*>(e->obj)) {
		OnCollisionWithKoopa(e);
		return;
	}
	if (dynamic_cast<CBoBro*>(e->obj)) {
		OnCollisionWithBoBro(e);
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
		if (!tailWhipTimer->IsRunning())
			OnCollisionWithMushroom(e);
		return;
	}
	if (dynamic_cast<CLeaf*>(e->obj)) {
		if (!tailWhipTimer->IsRunning())
			OnCollisionWithLeaf(e);
		return;
	}
	if (dynamic_cast<CPiranhaPlant*>(e->obj)) {
		OnCollisionWithPiranhaPlant(e);
		return;
	}
	if (dynamic_cast<CBullet*>(e->obj)) {
		OnCollisionWithBullet(e);
		return;
	}
	// basically same projectile as bullet
	if (dynamic_cast<CBoomerang*>(e->obj)) {
		OnCollisionWithBullet(e);
		return;
	}
	if (dynamic_cast<CPipe*>(e->obj)) {
		OnCollisionWithPipe(e);
		return;
	}
	if (e->ny > 0 && dynamic_cast<CBrick*>(e->obj)) {
		OnCollisionWithBrick(e);
		return;
	}
	if (dynamic_cast<CLift*>(e->obj)) {
		OnCollisionWithLift(e);
		return;
	}
	if (dynamic_cast<CSwitch*>(e->obj)) {
		OnCollisionWithSwitch(e);
		return;
	}
}

CMario::CMario(float x, float y) : CGameObject(x, y)
{
	//DebugOut(L"X: %f", x);
	//DebugOut(L" Y: %f\n", y);
	isSitting = false;
	isOnPlatform = false;
	isHolding = false;
	isReadyToHold = false;
	maxVy = MARIO_FALL_SPEED_Y;
	ay = MARIO_GRAVITY;
	untouchable = 0;
	LoadProgress();
	dragX = MARIO_DRAG_X;
	tanookiTail = new CTanookiTail(x, y);
	CGame::GetInstance()->GetCurrentScene()->AddObject(tanookiTail);
}

void CMario::AddScoreBasedOnStreak(int streak, LPGAMEOBJECT obj) {
	int FLYING_SCORE_TYPE;
	switch (streak) {
	case 1:
		FLYING_SCORE_TYPE = FLYING_SCORE_TYPE_100;
		break;
	case 2:
		FLYING_SCORE_TYPE = FLYING_SCORE_TYPE_200;
		break;
	case 3:
		FLYING_SCORE_TYPE = FLYING_SCORE_TYPE_400;
		break;
	case 4:
		FLYING_SCORE_TYPE = FLYING_SCORE_TYPE_800;
		break;
	default:
		// Don't know the score after the streak of 4 yet so I set it to be 800
		FLYING_SCORE_TYPE = FLYING_SCORE_TYPE_800;
	}
	float objX, objY;
	obj->GetPosition(objX, objY);
	// Might change 16 to the exact object's height later
	AddScore(objX, objY - (16 + FLYING_SCORE_HEIGHT) / 2, FLYING_SCORE_TYPE, true);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		goomba->TakeDamageFrom(this);
		stompingStreak++;
		AddScoreBasedOnStreak(stompingStreak, e->obj);
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

	if (tailWhipTimer->IsRunning() && e->ny == 0) {
		//bounce off right away, hit by tail OF MARIO, thus put mario in is logical
		koopa->TakeDamageFrom(this);
	}
	// hit koopa in shell in any direction
	else if (koopa->GetState() == KOOPAS_STATE_SHELL || koopa->GetState() == KOOPAS_STATE_REVIVING || koopa->GetState() == KOOPAS_STATE_FIRST_BOUNCE || koopa->GetState() == KOOPAS_STATE_SECOND_BOUNCE) {
		// Mario is not trying to hold because he wants to kick = kick
		// Mario is not trying to hold because he's already holding != kick ---> since mario will just kick the shell he is holding/colliding
		// Vertical movement will hinder the ability to hold, hence kick
		if ((!isReadyToHold && !isHolding) || e->ny != 0) {
			Kick();
			int koopaNx;
			float koopaX, koopaY;
			koopa->GetPosition(koopaX, koopaY);
			if (x <= koopaX)
				koopaNx = 1;
			else
				koopaNx = -1;
			koopa->SetDirection(koopaNx);
			koopa->SetState(KOOPAS_STATE_SPINNING);
		}
		// else if now check for a simple if trying to hold, when holding for real, trying to hold is set to false.
		else if (isReadyToHold) {
			isHolding = true;
			isReadyToHold = false;
			koopa->SetIsHeld(true);
		}
		if (e->ny < 0) {
			stompingStreak++;
			AddScoreBasedOnStreak(stompingStreak, e->obj);
		}
	}
	//hit koopa not in shell on top, can turn koopa ---> shell
	else if (e->ny < 0)
	{
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		koopa->TakeDamageFrom(this);
		stompingStreak++;
		AddScoreBasedOnStreak(stompingStreak, e->obj);
	}
	//touch koopa not in shell, not hit on top, koopa can be any other state, just not dead
	else if (koopa->GetState() != KOOPAS_STATE_DIE_WITH_BOUNCE && koopa->GetState() != KOOPAS_STATE_FIRST_BOUNCE && koopa->GetState() != KOOPAS_STATE_SECOND_BOUNCE)
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
		questionBlock->TakeDamageFrom(this);
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	AddScore(0, 0, 50, 0);
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* portal = (CPortal*)e->obj;
	SaveProgress();
	CGame::GetInstance()->InitiateSwitchScene(portal->GetSceneId());
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
	if (mushroom->GetState() == MUSHROOM_STATE_IDLE) return;
	int type = mushroom->GetType();
	if (level < MARIO_LEVEL_BIG) {
		SetLevel(MARIO_LEVEL_BIG);
		isResizing = true;
		CGame::GetInstance()->PauseGame();
		if (!resizeTimer->IsRunning())
			resizeTimer->Start();
	}
	if (type == MUSHROOM_TYPE_RED) {
		AddScore(x, y - MARIO_BIG_BBOX_HEIGHT / 2, FLYING_SCORE_TYPE_1000, true);
	}
	else if (type == MUSHROOM_TYPE_GREEN) {
		AddLife(x, y - MARIO_BIG_BBOX_HEIGHT / 2, true);
		AddScore(x, y - MARIO_BIG_BBOX_HEIGHT / 2, FLYING_SCORE_TYPE_1000, false);
	}
	mushroom->Delete();
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	if (leaf->GetState() == LEAF_STATE_IDLE) return;

	if (level == MARIO_LEVEL_BIG) {
		isTransforming = true;
		CGame::GetInstance()->PauseGame();
		if (!transformTimer->IsRunning())
			transformTimer->Start();
	}

	if (level < MARIO_LEVEL_TANOOKI) {
		SetLevel(MARIO_LEVEL_TANOOKI);
	}
	AddScore(x, y - MARIO_BIG_BBOX_HEIGHT / 2, FLYING_SCORE_TYPE_1000, true);
	leaf->Delete();
}

void CMario::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e)
{
	CPiranhaPlant* piranhaPlant = dynamic_cast<CPiranhaPlant*>(e->obj);

	if (tailWhipTimer->IsRunning() && piranhaPlant->GetState() != PIRANHA_PLANT_STATE_HIDDEN && piranhaPlant->GetState() != PIRANHA_PLANT_STATE_DIE) {
		piranhaPlant->TakeDamageFrom(this);
	}
	else if (piranhaPlant->GetState() != PIRANHA_PLANT_STATE_HIDDEN && piranhaPlant->GetState() != PIRANHA_PLANT_STATE_DIE)
	{
		TakeDamageFrom(piranhaPlant);
	}
}

void CMario::OnCollisionWithBullet(LPCOLLISIONEVENT e)
{
	TakeDamageFrom(e->obj);
}

void CMario::OnCollisionWithBoBro(LPCOLLISIONEVENT e)
{
	CBoBro* bro = dynamic_cast<CBoBro*>(e->obj);

	// jump on top >> kill bro and deflect a bit 
	if (e->ny < 0)
	{
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		stompingStreak++;
		// this is new behavior, bro when stomp earns more point, but TakeDamageFrom doesnt really have an idea of what kind of damage is taken
		// so for now, set it up like this is fine
		bro->SetState(BOOMERANG_BROTHER_STATE_DIE);
		AddScore(x, y, FLYING_SCORE_TYPE_1000, true);
	}
	else if (tailWhipTimer->IsRunning() && e->ny == 0) {
		bro->TakeDamageFrom(this);
	}

	else // hit by bro
	{
		if (bro->GetState() != BOOMERANG_BROTHER_STATE_DIE)
		{
			TakeDamageFrom(bro);
		}
	}

}

void CMario::OnCollisionWithPipe(LPCOLLISIONEVENT e)
{
	CPipe* pipe = dynamic_cast<CPipe*>(e->obj);
	float pipeToX = pipe->GetToX();
	float pipeToY = pipe->GetToY();
	if (pipeToX > 0 && pipeToY > 0) {
		if (pipe->GetUpsideDown()) {
			if (e->ny > 0 && CGame::GetInstance()->GetUpKeyBeingPressed()) {
				toX = pipeToX;
				toY = pipeToY;
				isAscendingAfterTravellingThroughPipe = pipe->IsAscendingAfter();
				SetState(MARIO_STATE_ASCENDING_THROUGH_PIPE);
			}
		}
		else {
			if (e->ny < 0 && isSitting) {
				toX = pipeToX;
				toY = pipeToY;
				isAscendingAfterTravellingThroughPipe = pipe->IsAscendingAfter();
				SetState(MARIO_STATE_DESCENDING_THROUGH_PIPE);
				isSitting = false;
			}
		}
	}
}
void CMario::OnCollisionWithSwitch(LPCOLLISIONEVENT e) {
	CSwitch* _switch = dynamic_cast<CSwitch*>(e->obj);
	_switch->SetState(SWITCH_STATE_PRESSING);
}

void CMario::OnCollisionWithBrick(LPCOLLISIONEVENT e) {
	if (level == MARIO_LEVEL_SMALL)
		e->obj->SetState(BRICK_STATE_BOUNCING);
	else {
		e->obj->SetState(BRICK_STATE_BROKEN);
	}
}
void CMario::OnCollisionWithLift(LPCOLLISIONEVENT e)
{
	CLift* lift = dynamic_cast<CLift*>(e->obj);
	if (e->ny < 0) {
		if (lift->GetState() != LIFT_STATE_FALLING)
			lift->SetState(LIFT_STATE_FALLING);
	}
}
//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	// is jumping
	if (!isOnPlatform)
	{
		if (isHolding) {
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_HOLD_JUMP_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_HOLD_JUMP_LEFT;
		}
		else if (abs(vx) >= MARIO_RUNNING_SPEED)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_DIVE_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_DIVE_LEFT;
		}
		else
		{
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_SMALL_JUMP_RIGHT;
			}
			else {
				aniId = ID_ANI_MARIO_SMALL_JUMP_LEFT;
			}
		}
	}
	else
		if (vx > 0.0f || state == MARIO_STATE_RUNNING_RIGHT) {
			if (isHolding)
				aniId = ID_ANI_MARIO_SMALL_HOLD_WALK_RIGHT;
			else if (ax < 0)
				aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
			else if (vx >= MARIO_RUNNING_SPEED)
				aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
		}
		else if (vx < 0.0f || state == MARIO_STATE_RUNNING_LEFT) {
			if (isHolding)
				aniId = ID_ANI_MARIO_SMALL_HOLD_WALK_LEFT;
			else if (ax > 0.0f)
				aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
			else if (vx <= -MARIO_RUNNING_SPEED)
				aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
			else
				aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
		}
		else if (vx == 0.0f)
		{
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				if (isHolding) aniId = ID_ANI_MARIO_SMALL_HOLD_IDLE_RIGHT;
			}
			else {
				aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
				if (isHolding) aniId = ID_ANI_MARIO_SMALL_HOLD_IDLE_LEFT;
			}
		}
	if (kickTimer.IsRunning()) {
		if (nx >= 0)
			aniId = ID_ANI_MARIO_SMALL_KICK_RIGHT;
		else
			aniId = ID_ANI_MARIO_SMALL_KICK_LEFT;
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
	// is jumping
	if (!isOnPlatform)
	{
		if (isHolding) {
			if (nx >= 0)
				aniId = ID_ANI_MARIO_BIG_HOLD_JUMP_RIGHT;
			else
				aniId = ID_ANI_MARIO_BIG_HOLD_JUMP_LEFT;
		}
		else if (abs(vx) >= MARIO_RUNNING_SPEED)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_BIG_DIVE_RIGHT;
			else
				aniId = ID_ANI_MARIO_BIG_DIVE_LEFT;
		}
		else
		{
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_BIG_JUMP_DOWN_RIGHT;
				if (vy <= 0.0f)
					aniId = ID_ANI_MARIO_BIG_JUMP_UP_RIGHT;
			}
			else {
				aniId = ID_ANI_MARIO_BIG_JUMP_DOWN_LEFT;
				if (vy <= 0.0f)
					aniId = ID_ANI_MARIO_BIG_JUMP_UP_LEFT;
			}
		}
	}
	//is sitting on the ground
	else
		if (isSitting)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_BIG_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_BIG_SIT_LEFT;
		}
	//is not sitting on the ground, can hold, can kick
	// idle, can hold
		else
			if (vx > 0.0f || state == MARIO_STATE_RUNNING_RIGHT) {
				if (isHolding)
					aniId = ID_ANI_MARIO_BIG_HOLD_WALK_RIGHT;
				else if (ax < 0)
					aniId = ID_ANI_MARIO_BIG_BRACE_RIGHT;
				else if (vx >= MARIO_RUNNING_SPEED)
					aniId = ID_ANI_MARIO_BIG_RUNNING_RIGHT;
				else
					aniId = ID_ANI_MARIO_BIG_WALKING_RIGHT;
			}
			else if (vx < 0.0f || state == MARIO_STATE_RUNNING_LEFT) {
				if (isHolding)
					aniId = ID_ANI_MARIO_BIG_HOLD_WALK_LEFT;
				else if (ax > 0.0f)
					aniId = ID_ANI_MARIO_BIG_BRACE_LEFT;
				else if (vx <= -MARIO_RUNNING_SPEED)
					aniId = ID_ANI_MARIO_BIG_RUNNING_LEFT;
				else
					aniId = ID_ANI_MARIO_BIG_WALKING_LEFT;
			}
			else if (vx == 0.0f)
			{
				if (nx >= 0) {
					aniId = ID_ANI_MARIO_BIG_IDLE_RIGHT;
					if (isHolding) aniId = ID_ANI_MARIO_BIG_HOLD_IDLE_RIGHT;
				}
				else {
					aniId = ID_ANI_MARIO_BIG_IDLE_LEFT;
					if (isHolding) aniId = ID_ANI_MARIO_BIG_HOLD_IDLE_LEFT;
				}
			}
	if (kickTimer.IsRunning()) {
		if (nx >= 0)
			aniId = ID_ANI_MARIO_BIG_KICK_RIGHT;
		else
			aniId = ID_ANI_MARIO_BIG_KICK_LEFT;
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_BIG_IDLE_RIGHT;

	return aniId;
}
//
// Get animation ID for tanooki Mario
// 
int CMario::GetAniIdTanooki()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (isHolding) {
			if (nx >= 0)
				aniId = ID_ANI_MARIO_TAIL_HOLD_JUMP_RIGHT;
			else
				aniId = ID_ANI_MARIO_TAIL_HOLD_JUMP_LEFT;
		}
		else if (flyTimer.IsRunning()) {
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_TAIL_FLY_RIGHT;
			}
			else {
				aniId = ID_ANI_MARIO_TAIL_FLY_LEFT;
			}
		}
		else if (tailWagTimer.IsRunning()) {
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_TAIL_WAGGING_RIGHT;
			}
			else {
				aniId = ID_ANI_MARIO_TAIL_WAGGING_LEFT;
			}
		}
		else if (abs(vx) >= MARIO_RUNNING_SPEED)
		{
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_TAIL_DIVE_DOWN_RIGHT;
				if (vy <= 0.0f)
					aniId = ID_ANI_MARIO_TAIL_DIVE_UP_RIGHT;
			}
			else {
				aniId = ID_ANI_MARIO_TAIL_DIVE_UP_LEFT;
				if (vy <= 0.0f)
					aniId = ID_ANI_MARIO_TAIL_DIVE_UP_LEFT;
			}
		}
		else
		{
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_TAIL_JUMP_DOWN_RIGHT;
				if (vy <= 0.0f)
					aniId = ID_ANI_MARIO_TAIL_JUMP_UP_RIGHT;
			}
			else {
				aniId = ID_ANI_MARIO_TAIL_JUMP_DOWN_LEFT;
				if (vy <= 0.0f)
					aniId = ID_ANI_MARIO_TAIL_JUMP_UP_LEFT;
			}
		}
	}
	//is on the ground
	else
		if (isSitting)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_TAIL_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_TAIL_SIT_LEFT;
		}
	//is not sitting on the ground, can hold, can kick
	// idle, can hold
		else

			if (vx > 0.0f || state == MARIO_STATE_RUNNING_RIGHT) {
				if (isHolding)
					aniId = ID_ANI_MARIO_TAIL_HOLD_WALKING_RIGHT;
				else if (ax < 0)
					aniId = ID_ANI_MARIO_TAIL_BRACE_RIGHT;
				else if (vx >= MARIO_RUNNING_SPEED)
					aniId = ID_ANI_MARIO_TAIL_RUNNING_RIGHT;
				else
					aniId = ID_ANI_MARIO_TAIL_WALKING_RIGHT;
			}
			else if (vx < 0.0f || state == MARIO_STATE_RUNNING_LEFT) {
				if (isHolding)
					aniId = ID_ANI_MARIO_TAIL_HOLD_WALKING_LEFT;
				else if (ax > 0.0f)
					aniId = ID_ANI_MARIO_TAIL_BRACE_LEFT;
				else if (vx <= -MARIO_RUNNING_SPEED)
					aniId = ID_ANI_MARIO_TAIL_RUNNING_LEFT;
				else
					aniId = ID_ANI_MARIO_TAIL_WALKING_LEFT;
			}
			else if (vx == 0.0f)
			{
				if (nx >= 0) {
					aniId = ID_ANI_MARIO_TAIL_IDLE_RIGHT;
					if (isHolding) aniId = ID_ANI_MARIO_TAIL_HOLD_IDLE_RIGHT;
				}
				else {
					aniId = ID_ANI_MARIO_TAIL_IDLE_LEFT;
					if (isHolding) aniId = ID_ANI_MARIO_TAIL_HOLD_IDLE_LEFT;
				}
			}
	if (kickTimer.IsRunning()) {
		if (nx >= 0)
			aniId = ID_ANI_MARIO_TAIL_KICK_RIGHT;
		else
			aniId = ID_ANI_MARIO_TAIL_KICK_LEFT;
	}
	else if (tailWhipTimer->IsRunning()) {
		if (nx >= 0)
			aniId = ID_ANI_MARIO_TAIL_WHIPPING_RIGHT;
		else
			aniId = ID_ANI_MARIO_TAIL_WHIPPING_LEFT;
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_TAIL_IDLE_RIGHT;

	return aniId;
}



int CMario::GetAniIdTransform(int level) {
	return ID_ANI_MARIO_TRANSFORM;
}

int CMario::GetAniIdResize(int level) {
	if (level == MARIO_LEVEL_BIG) {
		if (nx == -1)
			return ID_ANI_MARIO_ENLARGING_LEFT;
		else
			return ID_ANI_MARIO_ENLARGING_RIGHT;
	}
	else if (level == MARIO_LEVEL_SMALL) {
		if (nx == -1)
			return ID_ANI_MARIO_SHIRNKING_LEFT;
		else
			return ID_ANI_MARIO_SHIRNKING_RIGHT;
	}
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (isTransforming)
		aniId = GetAniIdTransform(level);
	else if (isResizing)
		aniId = GetAniIdResize(level);
	else if (isAscendingThroughPipe || isDescendingThroughPipe) {
		aniId = ID_ANI_MARIO_TRAVELLING_THROUGH_PIPE;
	}
	else if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_TANOOKI) {
		if (untouchable) {
			if ((int)(invincibleTimer.getAccumulated() / (MARIO_UNTOUCHABLE_TIME / 20)) % 2 == 0)
				aniId = GetAniIdTanooki();
			else
				aniId = ID_ANI_MARIO_INVISIBLE;
		}
		else
			aniId = GetAniIdTanooki();
	}
	else if (level == MARIO_LEVEL_BIG) {
		if (untouchable) {
			if ((int)(invincibleTimer.getAccumulated() / (MARIO_UNTOUCHABLE_TIME / 20)) % 2 == 0)
				aniId = GetAniIdBig();
			else
				aniId = ID_ANI_MARIO_INVISIBLE;
		}
		else
			aniId = GetAniIdBig();
	}
	else if (level == MARIO_LEVEL_SMALL) {
		if (untouchable) {
			if ((int)(invincibleTimer.getAccumulated() / (MARIO_UNTOUCHABLE_TIME / 20)) % 2 == 0)
				aniId = GetAniIdSmall();
			else
				aniId = ID_ANI_MARIO_INVISIBLE;
		}
		else
			aniId = GetAniIdSmall();
	}

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	//DebugOutTitle(L"State: %d", state);
	//DebugOut(L"maxVx: %f\n", maxVx);
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_DESCENDING_THROUGH_PIPE:
		travellingThroughPipeTimer->Start();
		isDescendingThroughPipe = true;
		break;
	case MARIO_STATE_ASCENDING_THROUGH_PIPE:
		travellingThroughPipeTimer->Start();
		isAscendingThroughPipe = true;
		break;
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
			if (abs(this->vx) == MARIO_RUNNING_SPEED) {
				vy = -MARIO_JUMP_RUN_SPEED_Y;
				if (level == MARIO_LEVEL_TANOOKI)
					if (!flyTimer.IsRunning())
						flyTimer.Start();
			}
			else
				vy = -MARIO_JUMP_SPEED_Y;
			//airborn, gravity will be lifted, drag reduces
			ay = MARIO_LIFTED_GRAVITY;
			dragX = MARIO_AIR_DRAG_X;
			isOnPlatform = false;
		}
		else if (level == MARIO_LEVEL_TANOOKI)
			tailWagTimer.Start();
		//if (!tailWagTimer.IsRunning())
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
		life--;
		level = MARIO_LEVEL_SMALL;
		dieTimer->Start();
		CGame::GetInstance()->PauseGame();
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level >= MARIO_LEVEL_BIG)
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

void CMario::AddLife(float x, float y, bool showEffect)
{
	life++;
	if (showEffect) {
		if (showEffect)
		{
			// create a score effect popup at (x, y)
			CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			scene->AddObject(new CFlyingScore(x, y, FLYING_SCORE_TYPE_1_LIFE));
		}

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

void CMario::AddCoin(int value)
{
	coin += value;
}

void CMario::HandleTimer(DWORD dt) {
	if (invincibleTimer.IsRunning()) {
		invincibleTimer.Tick(dt);
	}
	if (kickTimer.IsRunning()) {
		kickTimer.Tick(dt);
	}
	if (tailWhipTimer->IsRunning())
	{
		tailWhipTimer->Tick(dt);
	}
	if (tailWagTimer.IsRunning()) {
		tailWagTimer.Tick(dt);
	}
	if (flyTimer.IsRunning()) {
		flyTimer.Tick(dt);
	}
	if (isOnPlatformTimer.IsRunning()) {
		isOnPlatformTimer.Tick(dt);
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
	if (tailWhipTimer->HasPassed(MARIO_WHIPPING_TAIL_TIME))
	{
		tailWhipTimer->Reset();
	}
	if (tailWagTimer.HasPassed(MARIO_WAGGING_TAIL_TIME)) {
		tailWagTimer.Reset();
	}
	if (flyTimer.HasPassed(MARIO_FLYING_TIME)) {
		flyTimer.Reset();
	}
	if (isOnPlatformTimer.HasPassed(MARIO_IS_ON_PLATFORM_TIME)) {
		isOnPlatformTimer.Reset();
	}
}

void CMario::Attack()
{
	if (level == MARIO_LEVEL_TANOOKI
		&& !tailWhipTimer->IsRunning()
		&& !isSitting)
	{
		tailWhipTimer->Start();
	}
}

int CMario::GetSpeedStacks() {
	int speedStacks = (int)(abs(vx) / (MARIO_RUNNING_SPEED / MARIO_MAX_SPEED_STACKS));
	// just to be safe it wont go overboard, plus the stack stays full during flying
	if (speedStacks > MARIO_MAX_SPEED_STACKS || flyTimer.IsRunning())
		speedStacks = MARIO_MAX_SPEED_STACKS;
	return speedStacks;
}

