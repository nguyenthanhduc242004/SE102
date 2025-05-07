#pragma once
#include "DeltaTimer.h"
#include "GameObject.h"
#include "Platform.h"
#include "QuestionBlock.h"
#include "Brick.h"
#include "PlayScene.h"
#include "debug.h"
#include "Game.h"
#include "Platform.h"
#include "Mario.h"

#define KOOPAS_GRAVITY             GRAVITY
#define KOOPAS_WALKING_SPEED       0.05f
#define KOOPAS_SPINNING_SPEED      0.2f
#define KOOPAS_KILL_Y_BOUNCE   0.3f

#define KOOPAS_BBOX_WIDTH          16
#define KOOPAS_BBOX_HEIGHT         24
#define KOOPAS_BBOX_HEIGHT_SHELL   14

#define KOOPAS_HOLDING_Y_OFFSET		KOOPAS_BBOX_HEIGHT_SHELL / 3
#define KOOPAS_HOLDING_SMALL_MARIO_Y_ADJUST		(MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2

#define KOOPAS_DYING_TIMEOUT 500.0f
#define KOOPAS_SHELL_TIMEOUT 40000.0f
#define KOOPAS_REVIVING_TIMEOUT 2000.0f

#define KOOPAS_STATE_WALKING    100
#define KOOPAS_STATE_SHELL      200
#define KOOPAS_STATE_REVIVING   300
#define KOOPAS_STATE_SPINNING   400
#define KOOPAS_STATE_DIE        500
#define KOOPAS_STATE_DIE_WITH_BOUNCE  600

#define ID_ANI_KOOPAS_WALKING_LEFT    6000
#define ID_ANI_KOOPAS_WALKING_RIGHT   6100
#define ID_ANI_KOOPAS_SHELL           6200
#define ID_ANI_KOOPAS_REVIVING        6300
#define ID_ANI_KOOPAS_SPINNING_LEFT   6400
#define ID_ANI_KOOPAS_SPINNING_RIGHT  6500
#define ID_ANI_KOOPAS_DIE             6600
#define ID_ANI_KOOPAS_DIE_WITH_BOUNCE   6700

class CKoopa : public CGameObject, public CMoveable, public CDamageable
{
protected:
	bool isHeld;
	CDeltaTimer shellTimer;
	CDeltaTimer revivingTimer;
	CDeltaTimer dyingTimer;

	// core overrides
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	virtual void Render() override;

	// collisions
	virtual int  IsCollidable() override { return state != KOOPAS_STATE_DIE && state != KOOPAS_STATE_DIE_WITH_BOUNCE; }
	virtual int  IsBlocking()   override { return 0; }
	virtual void OnNoCollision(DWORD dt) override;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) override;

	// specific collision handlers
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithGoomba(LPCOLLISIONEVENT e);

	bool IsGroundAhead(LPCOLLISIONEVENT e);

	void HandleTimer(DWORD dt);
	void HandleBeingHeld();

public:
	CKoopa(float x, float y) : CGameObject(x, y) {
		nx = -1;
		SetState(KOOPAS_STATE_WALKING);
		isHeld = false;
	}

	virtual void SetState(int state) override;
	virtual void SetSpeed(float vx, float vy) override { this->vx = vx; this->vy = vy; }
	virtual void GetSpeed(float& vx, float& vy) override { vx = this->vx; vy = this->vy; }
	virtual void TakeDamageFrom(LPGAMEOBJECT obj) override;
	void SetIsHeld(BOOLEAN held) { 
		isHeld = held;
		ay = 0.0f;
	}
	BOOLEAN IsHeld() const { return isHeld; }
};
