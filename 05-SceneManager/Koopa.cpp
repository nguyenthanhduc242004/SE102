#include "Koopa.h"

#include <typeinfo>

CKoopa::CKoopa(float x, float y, int color, int type) : CGameObject(x, y) {
	this->color = color;
	this->type = type;
	nx = -1;
	y0 = y;
	SetState(KOOPAS_STATE_WALKING);
	probe = new CProbe(x, y);
	//CGame::GetInstance()->GetCurrentScene()->AddObject(probe);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - KOOPAS_BBOX_WIDTH / 2;
	right = x + KOOPAS_BBOX_WIDTH / 2;

	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_REVIVING || state == KOOPAS_STATE_SPINNING || state == KOOPAS_STATE_FIRST_BOUNCE || state == KOOPAS_STATE_SECOND_BOUNCE)
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
	if (!CGame::GetInstance()->IsInCamera(x - RESPAWN_OFFSET_CAM_X, DEFAULT_CAM_Y) &&
		!CGame::GetInstance()->IsInCamera(x + RESPAWN_OFFSET_CAM_X, DEFAULT_CAM_Y))
		return;

	vy += ay * dt;

	HandleTimer(dt);
	if (isHeld)	HandleBeingHeld();

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

	float probeX = x + (nx > 0 ? KOOPAS_BBOX_WIDTH / 4 : -KOOPAS_BBOX_WIDTH / 4);
	float probeY = y + KOOPAS_BBOX_HEIGHT / 4 - PROBE_BB_HEIGHT;
	if (probe) { 
		probe->SetSoftPosition(probeX, probeY); 
		probe->Update(dt, coObjects);
	}

}

// --- Render()
void CKoopa::Render()
{
	int aniId = -1;

	if (color == KOOPAS_COLOR_GREEN) {
		aniId = GetAniIdGreen();
	}
	else if (color == KOOPAS_COLOR_RED) {
		aniId = GetAniIdRed();
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;

	if (type == KOOPAS_TYPE_WING && color == KOOPAS_COLOR_RED) {
		if ((y - y0) > KOOPAS_RED_PARATROOPA_MAX_RANGE_Y) vy = -KOOPAS_WALKING_SPEED;
		if ((y - y0) < -KOOPAS_RED_PARATROOPA_MAX_RANGE_Y) vy = KOOPAS_WALKING_SPEED;
	}
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

int CKoopa::GetAniIdGreen()
{
	int aniId = -1;
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		if (type == KOOPAS_TYPE_WING) {
			aniId = (nx < 0)
				? ID_ANI_KOOPAS_GREEN_PARATROOPA_WALKING_LEFT
				: ID_ANI_KOOPAS_GREEN_PARATROOPA_WALKING_RIGHT;
		}
		else {
			aniId = (nx < 0)
				? ID_ANI_KOOPAS_GREEN_WALKING_LEFT
				: ID_ANI_KOOPAS_GREEN_WALKING_RIGHT;
		}
		break;
	case KOOPAS_STATE_SHELL:
		aniId = ID_ANI_KOOPAS_GREEN_SHELL;
		break;
	case KOOPAS_STATE_REVIVING:
		aniId = ID_ANI_KOOPAS_GREEN_SHAKE;
		break;
	case KOOPAS_STATE_SPINNING:
		aniId = (nx < 0)
			? ID_ANI_KOOPAS_GREEN_SPINNING_LEFT
			: ID_ANI_KOOPAS_GREEN_SPINNING_RIGHT;
		break;
	case KOOPAS_STATE_DIE_WITH_BOUNCE:
	case KOOPAS_STATE_SECOND_BOUNCE:
	case KOOPAS_STATE_FIRST_BOUNCE:
		aniId = ID_ANI_KOOPAS_GREEN_SHELL_UPSIDE_DOWN;
		break;
	}
	return aniId;
}

int CKoopa::GetAniIdRed()
{
	int aniId = -1;
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		if (type == KOOPAS_TYPE_WING) {
			aniId = (nx < 0)
				? ID_ANI_KOOPAS_RED_PARATROOPA_WALKING_LEFT
				: ID_ANI_KOOPAS_RED_PARATROOPA_WALKING_RIGHT;
		}
		else {
			aniId = (nx < 0)
				? ID_ANI_KOOPAS_RED_WALKING_LEFT
				: ID_ANI_KOOPAS_RED_WALKING_RIGHT;
		}
		break;
	case KOOPAS_STATE_SHELL:
		if (isUpsideDown)
			aniId = ID_ANI_KOOPAS_RED_SHELL_UPSIDE_DOWN;
		else
			aniId = ID_ANI_KOOPAS_RED_SHELL;
		break;
	case KOOPAS_STATE_REVIVING:
		if (isUpsideDown)
			aniId = ID_ANI_KOOPAS_RED_SHAKE_UPSIDE_DOWN;
		else
			aniId = ID_ANI_KOOPAS_RED_SHAKE;
		break;
	case KOOPAS_STATE_SPINNING:
		if (nx < 0) {
			if (isUpsideDown)
				aniId = ID_ANI_KOOPAS_RED_SPINNING_LEFT_UPSIDE_DOWN;
			else
				aniId = ID_ANI_KOOPAS_RED_SPINNING_LEFT;
		}
		else {
			if (isUpsideDown)
				aniId = ID_ANI_KOOPAS_RED_SPINNING_RIGHT_UPSIDE_DOWN;
			else
				aniId = ID_ANI_KOOPAS_RED_SPINNING_RIGHT;
		}
		break;
	case KOOPAS_STATE_DIE_WITH_BOUNCE:
	case KOOPAS_STATE_SECOND_BOUNCE:
	case KOOPAS_STATE_FIRST_BOUNCE:
		aniId = ID_ANI_KOOPAS_RED_SHELL_UPSIDE_DOWN;
		break;
	}
	return aniId;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj)) return;

	//check all npc / which this npc can go through/interact ---> solve Mario triggering collision event multiple times because of IsBlocking setting
	if (dynamic_cast<CBoBro*>(e->obj)) {
		OnCollisionWithBoBro(e);
	}
	if (dynamic_cast<CKoopa*>(e->obj)) {
		OnCollisionWithKoopa(e);
		return;
	}
	if (dynamic_cast<CGoomba*>(e->obj)) {
		OnCollisionWithGoomba(e);
		return;
	}
	if (dynamic_cast<CPiranhaPlant*>(e->obj)) {
		CPiranhaPlant* piranhaPlant = dynamic_cast<CPiranhaPlant*>(e->obj);

		if (piranhaPlant->GetState() != PIRANHA_PLANT_STATE_DIE) {
			CMario* mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			spinningKillStreak++;
			mario->AddScoreBasedOnStreak(spinningKillStreak, e->obj);
		}

		piranhaPlant->TakeDamageFrom(this);

		return;
	}
	// IsBlocking will be used only to check map objects ---> NPCs are not blocking, Koopa can go through them.
	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0)
	{
		if (type == KOOPAS_TYPE_WING && color == KOOPAS_COLOR_RED) {
			vy = -vy;
		}
		else {
			vy = 0;
			if (e->ny < 0) {
				if (type == KOOPAS_TYPE_WING && state == KOOPAS_STATE_WALKING)
					vy = -KOOPAS_JUMPING_SPEED;
				if (state == KOOPAS_STATE_FIRST_BOUNCE) {
					SetState(KOOPAS_STATE_SECOND_BOUNCE);
				}
				else if (state == KOOPAS_STATE_SECOND_BOUNCE) {
					SetState(KOOPAS_STATE_SHELL);
				}
			}
		}
	}

	//static overlap arguments, when Koopa is passively collided, or it is overlapping
	//use for when Mario kicking it directly into a block
	if (e->nx == 0 && e->ny == 0 && e->dx == 0 && e->dy == 0) {
		//if (dynamic_cast<CQuestionBlock*>(e->obj) || dynamic_cast<CBrick*>(e->obj)) {
		//	TakeDamageFrom(e->obj);
		//	return;
		//}

		// Shell is held, basically invincible
		if (isHeld) return;
		// overlapping platforms are allowed, except CSideCollidablePlatform
		if (dynamic_cast<CPlatform*>(e->obj) != NULL && dynamic_cast<CSideCollidablePlatform*>(e->obj) == NULL && dynamic_cast<CBlockPlatform*>(e->obj) == NULL) return;
		// center of shell goes inside bounding box, kill, else, dont, still count as hitting against surface
		float l, t, r, b;
		e->obj->GetBoundingBox(l, t, r, b);
		if (x < l) return;
		if (y - KOOPAS_HOLDING_Y_OFFSET < t) return;
		if (x > r) return;
		if (y - KOOPAS_HOLDING_Y_OFFSET > b) return;
		// judge overlaping vertical more harshly
		// killed by overlapping kick
		nx = 0;
		TakeDamageFrom(NULL);
		return;
	}
	//	universal edge detection for all block type
	if (state != KOOPAS_STATE_SPINNING && state != KOOPAS_STATE_DIE_WITH_BOUNCE && state != KOOPAS_STATE_FIRST_BOUNCE) {
		if (color == KOOPAS_COLOR_RED && type == KOOPAS_TYPE_NORMAL && !IsGroundAhead()) {
			probe->Reset();
			ReverseDirection();

		}
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
		ReverseDirection();
	}
}
void CKoopa::OnCollisionWithPlatform(LPCOLLISIONEVENT e) {
	if (e->nx != 0 && (dynamic_cast<CSideCollidablePlatform*>(e->obj) || dynamic_cast<CBlockPlatform*>(e->obj))) {
		ReverseDirection();
	}
}
void CKoopa::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e)
{
	if (state == KOOPAS_STATE_SPINNING) {
		CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);
		if (e->nx != 0) {
			questionBlock->TakeDamageFrom(this);
		}
	}
	if (state == KOOPAS_STATE_WALKING && e->obj->GetState() == QUESTION_BLOCK_STATE_BOUNCING) {
		TakeDamageFrom(e->obj);
	}
	if (e->nx != 0) {
		ReverseDirection();
	}
}
void CKoopa::OnCollisionWithBrick(LPCOLLISIONEVENT e) {
	if (state == KOOPAS_STATE_SPINNING) {
		if (e->nx != 0) {
			e->obj->SetState(BRICK_STATE_BROKEN);
		}
	}
	if (e->nx != 0) {
		ReverseDirection();
	}
}

// TODO: Do spinningKillStreak score logic later
void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	bool thisSpinning = (state == KOOPAS_STATE_SPINNING);
	bool otherSpinning = (koopa->state == KOOPAS_STATE_SPINNING);
	bool otherWalking = (koopa->state == KOOPAS_STATE_WALKING);
	if (thisSpinning) {
		if (otherSpinning) {
			nx = -nx;
			TakeDamageFrom(koopa);
		}
		koopa->nx = -nx;
		koopa->TakeDamageFrom(this);
		return;
	}
	if (isHeld) {
		//this bounces backward
		nx = -nx;
		TakeDamageFrom(koopa);
		//koopa bounces away from this
		koopa->nx = -nx;
		koopa->TakeDamageFrom(this);
		return;
	}
	// clearly this is not spinning, thus it is walking, but for other koopa, we dont know, and remember, we only turn koopa if koopa is WALKING into this koopa
	if (e->nx != 0) {
		ReverseDirection();
		if (!otherWalking) return;
		koopa->ReverseDirection();
	}
}
void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (state == KOOPAS_STATE_SPINNING) {
		//DebugOut(L"Koopa direction when hit: %d\n", nx);
		goomba->SetDirection(nx);
		goomba->TakeDamageFrom(this);
		CMario* mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		spinningKillStreak++;
		mario->AddScoreBasedOnStreak(spinningKillStreak, e->obj);
		return;
	}
	if (isHeld) {
		//this bounces backward
		nx = -nx;
		TakeDamageFrom(goomba);
		//goomba bounces away from this
		goomba->SetDirection(-nx);
		goomba->TakeDamageFrom(this);
		return;
	}
	if (e->nx != 0) {
		ReverseDirection();
		goomba->ReverseDirection();
	}
}
void CKoopa::OnCollisionWithBoBro(LPCOLLISIONEVENT e)
{
	CBoBro* bro = dynamic_cast<CBoBro*>(e->obj);

	if (state == KOOPAS_STATE_SPINNING) {
		//DebugOut(L"Koopa direction when hit: %d\n", nx);
		bro->SetDirection(nx);
		bro->TakeDamageFrom(this);
		CMario* mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		spinningKillStreak++;
		mario->AddScoreBasedOnStreak(spinningKillStreak, e->obj);
		return;
	}
	if (isHeld) {
		//this bounces backward
		nx = -nx;
		TakeDamageFrom(bro);
		//goomba bounces away from this
		bro->SetDirection(-nx);
		bro->TakeDamageFrom(this);
		return;
	}
	if (e->nx != 0) {
		ReverseDirection();
		bro->ReverseDirection();
	}
}

bool CKoopa::IsGroundAhead()
{
	return probe->IsGroundAhead();
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
		if (isHeld) {
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			mario->TakeDamageFrom(this);
		}
		SetState(KOOPAS_STATE_WALKING);
	}
}
void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		isUpsideDown = false;
		vx = nx * KOOPAS_WALKING_SPEED;
		if (type == KOOPAS_TYPE_WING) {
			ay = KOOPAS_WING_GRAVITY;
			if (color == KOOPAS_COLOR_RED) {
				ay = 0;
				vx = 0;
				vy = KOOPAS_WALKING_SPEED;
			}
		}
		else {
			ay = KOOPAS_GRAVITY;
		}
		isHeld = false;
		break;
	case KOOPAS_STATE_SHELL:
		spinningKillStreak = 0;
		vx = 0.0f;
		ay = KOOPAS_GRAVITY;
		isHeld = false;
		shellTimer.Start();
		break;
	case KOOPAS_STATE_REVIVING:
		vx = 0.0f;
		revivingTimer.Start();
		break;
	case KOOPAS_STATE_SPINNING:
		vx = nx * KOOPAS_SPINNING_SPEED;
		ay = KOOPAS_WING_GRAVITY;
		isHeld = false;
		break;
	case KOOPAS_STATE_DIE_WITH_BOUNCE:
		isUpsideDown = true;
		vx = nx * KOOPAS_WALKING_SPEED;
		vy = -KOOPAS_KILL_Y_BOUNCE;
		ay = KOOPAS_GRAVITY;
		isHeld = false;
		dyingTimer.Start();
		SetSpawnerDead(true);
		break;
	case KOOPAS_STATE_FIRST_BOUNCE:
		spinningKillStreak = 0;
		isUpsideDown = true;
		vx = nx * KOOPAS_WALKING_SPEED;
		vy = -KOOPAS_KILL_Y_BOUNCE;
		ay = KOOPAS_GRAVITY;
		isHeld = false;
		break;
	case KOOPAS_STATE_SECOND_BOUNCE:
		vy = -KOOPAS_KILL_SECOND_Y_BOUNCE;
		break;
	}
}
void CKoopa::TakeDamageFrom(LPGAMEOBJECT obj)
{
	// damage caused directly by Mario's stomping
	if (CMario* mario = dynamic_cast<CMario*>(obj)) {
		if (type == KOOPAS_TYPE_WING) {
			vy = 0;
			ay = KOOPAS_GRAVITY;
			//mario->AddScore(x, y - (KOOPAS_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, FLYING_SCORE_TYPE_100, true);
			type = KOOPAS_TYPE_NORMAL;
			return;
		}
		if (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_SPINNING) {
			SetState(KOOPAS_STATE_SHELL);
			return;
		}
		return;
	}

	/*if (dynamic_cast<CQuestionBlock*>(obj) || dynamic_cast<CBrick*>(obj)) {
		if (state != KOOPAS_STATE_FIRST_BOUNCE) {
			float objectX, objectY;
			obj->GetPosition(objectX, objectY);
			CMario* mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			mario->AddScore(x, y - (KOOPAS_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, FLYING_SCORE_TYPE_100, true);
			if (x <= objectX) {
				nx = -1;
			}
			else {
				nx = 1;
			}
			SetState(KOOPAS_STATE_FIRST_BOUNCE);
			return;
		}
		return;
	}*/

	if (dynamic_cast<CTanookiTail*>(obj)) {
		if (state != KOOPAS_STATE_FIRST_BOUNCE) {
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (state != KOOPAS_STATE_SHELL)
				mario->AddScore(x, y - (KOOPAS_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, FLYING_SCORE_TYPE_100, true);
			float marioX, marioY;
			mario->GetPosition(marioX, marioY);
			if (marioX <= x)
				nx = 1;
			else
				nx = -1;
			SetState(KOOPAS_STATE_FIRST_BOUNCE);
			return;
		}
	}

	// Only Mario cant kill the koopa on his own, anything else will kill koopa
	if (state != KOOPAS_STATE_DIE_WITH_BOUNCE) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->AddScore(x, y - (KOOPAS_BBOX_HEIGHT + FLYING_SCORE_HEIGHT) / 2, FLYING_SCORE_TYPE_100, true);
		SetState(KOOPAS_STATE_DIE_WITH_BOUNCE);
	}
}

void CKoopa::Delete()
{
	probe->Delete();	//essential part of a koopa, only koopa can delete--->a custom delete with passed argument for future cases if yes, for some reason, still possible null
	probe = NULL;
	isDeleted = true;
}
/// Debug code
// Convert mangled name (char*) to wstring for DebugOut
	//std::string className = typeid(*e->obj).name();
	//std::wstring wideClassName(className.begin(), className.end()); // crude but readable
	//DebugOut(L"[INFO] Object: %s\n", wideClassName.c_str());
	//DebugOut(L"%d\n", GetTickCount64());
	//DebugOut(L"%d\n", (int)(state == KOOPAS_STATE_SHELL));
	///

void CKoopa::SetSpawnerDead(bool isDead) {
	vector<LPSPAWNER> spawners = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetSpawners();
	for (LPSPAWNER spawner : spawners) {
		if (spawner->obj == this) {
			spawner->SetDead(isDead);
			return;
		}
	}
}