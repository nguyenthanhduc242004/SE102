#pragma once
#include "GameObject.h"
#include "PlayScene.h"
#include "Animation.h"
#include "Animations.h"
#include "DeltaTimer.h"
#include "debug.h"
#include "FlyingScore.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.00037f
#define MARIO_ACCEL_RUN_X	0.00032f

#define MARIO_DRAG_X		0.0002f
#define MARIO_AIR_DRAG_X		0.00001f

#define MARIO_JUMP_SPEED_Y		0.25f
#define MARIO_JUMP_RUN_SPEED_Y	0.28f
#define MARIO_FALL_SPEED_Y		0.25f
#define MARIO_FLY_SPEED				0.135f
#define MARIO_FALL_SLOW_SPEED	0.05f

#define MARIO_GRAVITY			GRAVITY
#define MARIO_LIFTED_GRAVITY	0.00043f

#define MARIO_JUMP_DEFLECT_SPEED  0.36f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601
//
//#pragma region ANIMATION_ID
//
//#define ID_ANI_MARIO_IDLE_RIGHT 400
//#define ID_ANI_MARIO_IDLE_LEFT 401
//
//#define ID_ANI_MARIO_WALKING_RIGHT 500
//#define ID_ANI_MARIO_WALKING_LEFT 501
//
//#define ID_ANI_MARIO_RUNNING_RIGHT 600
//#define ID_ANI_MARIO_RUNNING_LEFT 601
//
//#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
//#define ID_ANI_MARIO_JUMP_WALK_LEFT 701
//
//#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
//#define ID_ANI_MARIO_JUMP_RUN_LEFT 801
//
//#define ID_ANI_MARIO_SIT_RIGHT 900
//#define ID_ANI_MARIO_SIT_LEFT 901
//
//#define ID_ANI_MARIO_BRACE_RIGHT 1000
//#define ID_ANI_MARIO_BRACE_LEFT 1001
//
//
//// SMALL MARIO
//#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
//#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102
//
//#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
//#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201
//
//#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
//#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301
//
//#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
//#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401
//
//#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
//#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501
//
//#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
//#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601
//
//#pragma endregion
//


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_DIE 999

// ==== SMALL MARIO ====
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 100
#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1000
#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1010
#define ID_ANI_MARIO_SMALL_JUMP_RIGHT 1020
#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1030
#define ID_ANI_MARIO_SMALL_HOLD_IDLE_RIGHT 10500
#define ID_ANI_MARIO_SMALL_HOLD_WALK_RIGHT 10510
#define ID_ANI_MARIO_SMALL_HOLD_JUMP_RIGHT 10530
#define ID_ANI_MARIO_SMALL_DIVE_RIGHT 10620
#define ID_ANI_MARIO_SMALL_KICK_RIGHT 10570

#define ID_ANI_MARIO_SMALL_IDLE_LEFT 101
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1001
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1011
#define ID_ANI_MARIO_SMALL_JUMP_LEFT 1021
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1031
#define ID_ANI_MARIO_SMALL_HOLD_IDLE_LEFT 10501
#define ID_ANI_MARIO_SMALL_HOLD_WALK_LEFT 10511
#define ID_ANI_MARIO_SMALL_HOLD_JUMP_LEFT 10531
#define ID_ANI_MARIO_SMALL_DIVE_LEFT 10621
#define ID_ANI_MARIO_SMALL_KICK_LEFT 10571

// ==== BIG MARIO ====
#define ID_ANI_MARIO_BIG_IDLE_RIGHT 110
#define ID_ANI_MARIO_BIG_WALKING_RIGHT 1100
#define ID_ANI_MARIO_BIG_RUNNING_RIGHT 1110
#define ID_ANI_MARIO_BIG_JUMP_UP_RIGHT 1120
#define ID_ANI_MARIO_BIG_JUMP_DOWN_RIGHT 1123
#define ID_ANI_MARIO_BIG_BRACE_RIGHT 1130
#define ID_ANI_MARIO_BIG_SIT_RIGHT 1140
#define ID_ANI_MARIO_BIG_HOLD_IDLE_RIGHT 11500
#define ID_ANI_MARIO_BIG_HOLD_WALK_RIGHT 11510
#define ID_ANI_MARIO_BIG_HOLD_JUMP_RIGHT 11530
#define ID_ANI_MARIO_BIG_DIVE_RIGHT 11620
#define ID_ANI_MARIO_BIG_KICK_RIGHT 11570

#define ID_ANI_MARIO_BIG_IDLE_LEFT 111
#define ID_ANI_MARIO_BIG_WALKING_LEFT 1101
#define ID_ANI_MARIO_BIG_RUNNING_LEFT 1111
#define ID_ANI_MARIO_BIG_JUMP_UP_LEFT 1121
#define ID_ANI_MARIO_BIG_JUMP_DOWN_LEFT 1124
#define ID_ANI_MARIO_BIG_BRACE_LEFT 1131
#define ID_ANI_MARIO_BIG_SIT_LEFT 1141
#define ID_ANI_MARIO_BIG_HOLD_IDLE_LEFT 11501
#define ID_ANI_MARIO_BIG_HOLD_WALK_LEFT 11511
#define ID_ANI_MARIO_BIG_HOLD_JUMP_LEFT 11531
#define ID_ANI_MARIO_BIG_DIVE_LEFT 11621
#define ID_ANI_MARIO_BIG_KICK_LEFT 11571

// ===== TAIL MARIO  =====

#define ID_ANI_TAIL_MARIO_IDLE_RIGHT        120
#define ID_ANI_TAIL_MARIO_IDLE_LEFT         121
#define ID_ANI_TAIL_MARIO_WALK_RIGHT        1200
#define ID_ANI_TAIL_MARIO_WALK_LEFT         1201
#define ID_ANI_TAIL_MARIO_RUN_RIGHT         1210
#define ID_ANI_TAIL_MARIO_RUN_LEFT          1211
#define ID_ANI_TAIL_MARIO_JUMP_UP_RIGHT     1220
#define ID_ANI_TAIL_MARIO_JUMP_UP_LEFT      1221
#define ID_ANI_TAIL_MARIO_JUMP_DOWN_RIGHT   1223
#define ID_ANI_TAIL_MARIO_JUMP_DOWN_LEFT    1224
#define ID_ANI_TAIL_MARIO_BRAKE_RIGHT       1230
#define ID_ANI_TAIL_MARIO_BRAKE_LEFT        1231
#define ID_ANI_TAIL_MARIO_SIT_RIGHT         1240
#define ID_ANI_TAIL_MARIO_SIT_LEFT          1241
#define ID_ANI_TAIL_MARIO_HOLD_IDLE_RIGHT   12500
#define ID_ANI_TAIL_MARIO_HOLD_IDLE_LEFT    12501
#define ID_ANI_TAIL_MARIO_HOLD_WALK_RIGHT   12510
#define ID_ANI_TAIL_MARIO_HOLD_WALK_LEFT    12511
#define ID_ANI_TAIL_MARIO_HOLD_JUMP_RIGHT   12530
#define ID_ANI_TAIL_MARIO_HOLD_JUMP_LEFT    12531
#define ID_ANI_TAIL_MARIO_FLY_UP_RIGHT      12620
#define ID_ANI_TAIL_MARIO_FLY_UP_LEFT       12621
#define ID_ANI_TAIL_MARIO_FLY_DOWN_RIGHT    12630
#define ID_ANI_TAIL_MARIO_FLY_DOWN_LEFT     12631
#define ID_ANI_TAIL_MARIO_FLY_FLAP_RIGHT    12640
#define ID_ANI_TAIL_MARIO_FLY_FLAP_LEFT     12641
#define ID_ANI_TAIL_MARIO_KICK_RIGHT        12570
#define ID_ANI_TAIL_MARIO_KICK_LEFT         12571
#define ID_ANI_TAIL_MARIO_TURN_RIGHT        1280
#define ID_ANI_TAIL_MARIO_TURN_LEFT         1281

#define ID_ANI_TAIL_MARIO_FLAP_RIGHT        1226
#define ID_ANI_TAIL_MARIO_FLAP_LEFT         1228

// ENLARGING
#define ID_ANI_MARIO_ENLARGING_RIGHT	17011
#define ID_ANI_MARIO_ENLARGING_LEFT		17021

#pragma endregion


#define GROUND_Y 160.0f

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TANOOKI 3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_UNTOUCHABLE_TIME 1000
#define MARIO_KICK_TIME	200	
#define MARIO_WHIPPING_TAIL_TIME		400
#define MARIO_WHIPPING_TAIL_FRAME_TIME	70
#define MARIO_WAGGING_TAIL_TIME		400
#define MARIO_FLYING_TIME			1750

#define MARIO_ENLARGING_TIME	1200


class CMario : public CGameObject, public CMoveable, public CDamageable {
	BOOLEAN isSitting;
	BOOLEAN isOnPlatform;
	BOOLEAN isHolding;
	BOOLEAN isReadyToHold;
	int level;
	int untouchable;
	int isBoosted;
	int score;
	int coin;
	float dragX;

	CDeltaTimer invincibleTimer;
	CDeltaTimer kickTimer;
	// Tail swing timers and state
	CDeltaTimer    tailWhipTimer;    
	CDeltaTimer    tailWhipFrameTimer; 
	int      tailWhipFrame;        // 0 = no swing, 1...5 = which whip sprite
	//CTanooki* tailSprite;      // the actual whip‐hitbox object
	CDeltaTimer	tailWagTimer;
	CDeltaTimer flyTimer;

	CDeltaTimer* enlargingTimer = new CDeltaTimer();
	bool isTransforming = false;



	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithBullet(LPCOLLISIONEVENT e);

	//int GetAniIdTanooki();
	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdTransform(int level);

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		isOnPlatform = false;
		isHolding = false;
		isReadyToHold = false;
		maxVy = MARIO_FALL_SPEED_Y;
		ay = MARIO_GRAVITY;
		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		score = 0;
		coin = 0;
		dragX = MARIO_DRAG_X;

		tailWhipFrame = 0;
		//tailSprite = new CTanooki(x, y);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	int GetLevel() {
		return level;
	}
	void SetLevel(int l) {
		// Adjust position to avoid falling off platform
		if (this->level == MARIO_LEVEL_SMALL)
		{
			y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
		}
		level = l;

	}
	void StartUntouchable() {
		untouchable = 1;
		invincibleTimer.Start();
	}
	void Kick() {
		untouchable = 1;
		kickTimer.Start();
	}
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void SetSpeed(float vx, float vy) override {
		this->vx = vx;
		this->vy = vy;
	}
	virtual void GetSpeed(float& vx, float& vy) override {
		vx = this->vx;
		vy = this->vy;
	}

	virtual void TakeDamageFrom(LPGAMEOBJECT obj) override {
		if (untouchable) return;
		if (level > MARIO_LEVEL_BIG)
		{
			level = MARIO_LEVEL_BIG;
			StartUntouchable();
		}
		else if (level > MARIO_LEVEL_SMALL)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
	// will only be called when Mario fall out of bound
	virtual void Delete() override {
		CGame* game = CGame::GetInstance();
		if (state != MARIO_STATE_DIE) {
			float x, y;
			game->GetCamPos(x, y);
			this->SetPosition(x + game->GetBackBufferWidth() / 2, y + game->GetBackBufferHeight());
			SetState(MARIO_STATE_DIE);
			return;
		}
		game->EndGame();
	}
	void AddScore(float x, float y, int value, bool showEffect);

	int GetScore() const { return score; }

	void HandleTimer(DWORD dt);

	BOOLEAN IsHolding() {
		return isHolding;
	}
	BOOLEAN IsReadyToHold() {
		return isReadyToHold;
	}
	void SetIsHolding(BOOLEAN isHolding) {
		this->isHolding = isHolding;
	}
	void SetIsReadyToHold(BOOLEAN isReadyToHold) {
		this->isReadyToHold = isReadyToHold;
	}

	void Attack();

	CDeltaTimer* GetEnlargingTimer() {
		return this->enlargingTimer;
	}

	void SetTransforming(boolean isTransforming) {
		this->isTransforming = isTransforming;
	}
};