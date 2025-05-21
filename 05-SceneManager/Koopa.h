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
#define KOOPAS_WING_GRAVITY				0.0007f
#define KOOPAS_WALKING_SPEED       0.036f
#define KOOPAS_SPINNING_SPEED      0.2f
#define KOOPAS_KILL_Y_BOUNCE   0.4f
#define KOOPAS_KILL_SECOND_Y_BOUNCE 0.1f;
#define KOOPAS_JUMPING_SPEED		0.22f

#define KOOPAS_BBOX_WIDTH          15
#define KOOPAS_BBOX_HEIGHT         24
#define KOOPAS_BBOX_HEIGHT_SHELL   14

#define KOOPAS_HOLDING_Y_OFFSET		KOOPAS_BBOX_HEIGHT_SHELL / 3
#define KOOPAS_HOLDING_SMALL_MARIO_Y_ADJUST		(MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2

#define KOOPAS_DYING_TIMEOUT 500.0f
#define KOOPAS_SHELL_TIMEOUT 4500
#define KOOPAS_REVIVING_TIMEOUT 1500

#define KOOPAS_STATE_WALKING    100
#define KOOPAS_STATE_SHELL      200
#define KOOPAS_STATE_REVIVING   300
#define KOOPAS_STATE_SPINNING   400
#define KOOPAS_STATE_DIE_WITH_BOUNCE        500
#define KOOPAS_STATE_FIRST_BOUNCE  600
#define KOOPAS_STATE_SECOND_BOUNCE  700

#define KOOPAS_COLOR_GREEN	0
#define KOOPAS_COLOR_RED	1

#define KOOPAS_TYPE_NORMAL	0
#define KOOPAS_TYPE_WING	1

// Red Koopas Animations
#define ID_ANI_KOOPAS_RED_WALKING_RIGHT				  36000
#define ID_ANI_KOOPAS_RED_SPINNING_RIGHT			  36020
#define ID_ANI_KOOPAS_RED_SPINNING_RIGHT_UPSIDE_DOWN  36023
#define ID_ANI_KOOPAS_RED_PARATROOPA_WALKING_RIGHT    36200
#define ID_ANI_KOOPAS_RED_WALKING_LEFT				  36001
#define ID_ANI_KOOPAS_RED_SPINNING_LEFT				  36021
#define ID_ANI_KOOPAS_RED_SPINNING_LEFT_UPSIDE_DOWN   36024
#define ID_ANI_KOOPAS_RED_PARATROOPA_WALKING_LEFT     36201
#define ID_ANI_KOOPAS_RED_SHELL					      36070
#define ID_ANI_KOOPAS_RED_SHELL_UPSIDE_DOWN			  36071
#define ID_ANI_KOOPAS_RED_SHAKE						  36083
#define ID_ANI_KOOPAS_RED_SHAKE_UPSIDE_DOWN			  36084

// Green Koopas Animations
#define ID_ANI_KOOPAS_GREEN_WALKING_RIGHT			   36100
#define ID_ANI_KOOPAS_GREEN_SPINNING_RIGHT		       36120
#define ID_ANI_KOOPAS_GREEN_PARATROOPA_WALKING_RIGHT   36300
#define ID_ANI_KOOPAS_GREEN_WALKING_LEFT		       36101
#define ID_ANI_KOOPAS_GREEN_SPINNING_LEFT			   36121
#define ID_ANI_KOOPAS_GREEN_PARATROOPA_WALKING_LEFT    36301
#define ID_ANI_KOOPAS_GREEN_SHELL				       36170
#define ID_ANI_KOOPAS_GREEN_SHAKE				       36183
#define ID_ANI_KOOPAS_GREEN_SHELL_UPSIDE_DOWN		   36193

class CKoopa : public CGameObject, public CMoveable, public CDamageable
{
protected:
	int color;
	int type;
	bool isHeld;
	CDeltaTimer shellTimer;
	CDeltaTimer revivingTimer;
	CDeltaTimer dyingTimer;
	boolean isUpsideDown = false;
	int spinningKillStreak = 0;

	// core overrides
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	virtual void Render() override;

	// collisions
	virtual int  IsCollidable() override { return state != KOOPAS_STATE_DIE_WITH_BOUNCE; }
	virtual int  IsBlocking()   override { return 0; }
	virtual void OnNoCollision(DWORD dt) override;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) override;

	// specific collision handlers
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBoBro(LPCOLLISIONEVENT e);

	bool IsGroundAhead(LPCOLLISIONEVENT e);

	void HandleTimer(DWORD dt);
	void HandleBeingHeld();

	int GetAniIdGreen();
	int GetAniIdRed();

public:
	CKoopa(float x, float y, int color, int type);
	virtual void SetState(int state) override;
	virtual void GetPosition(float& x, float& y) { x = this->x; y = this->y; };
	virtual void SetSpeed(float vx, float vy) override { this->vx = vx; this->vy = vy; }
	virtual void GetSpeed(float& vx, float& vy) override { vx = this->vx; vy = this->vy; }
	virtual void TakeDamageFrom(LPGAMEOBJECT obj) override;
	void SetIsHeld(BOOLEAN held) { 
		isHeld = held;
		ay = 0.0f;
	}
	BOOLEAN IsHeld() const { return isHeld; }
};
