#pragma once
#include "GameObject.h"
#include "Platform.h"
#include "DeltaTimer.h"
#include "Koopa.h"

//#define GOOMBA_GRAVITY GRAVITY
#define GOOMBA_GRAVITY 0.0005f
#define GOOMBA_WALKING_SPEED 0.03f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 16
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define PARAGOOMBA_WIDTH 20
#define PARAGOOMBA_HEIGHT 24

#define GOOMBA_DYING_TIMEOUT 500.0f

#define GOOMBA_STATE_HIDDEN	0
#define GOOMBA_STATE_RESPAWNABLE 1
#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_WITH_BOUNCE  300
#define PARAGOOMBA_STATE_WALKING 400
#define PARAGOOMBA_STATE_PRE_FLY 500
#define PARAGOOMBA_STATE_FLYING 600

#define PARAGOOMBA_WALKING_X_OFFSET 24
#define PARAGOOMBA_TOTAL_FLIES 5
#define PARAGOOMBA_PRE_FLY_TIME 1000
#define PARAGOOMBA_PRE_FLY_VY 0.08f

#define PARAGOOMBA_FLY_VY 0.18f

#define ID_ANI_BROWN_GOOMBA_WALKING			5101
#define SPRITE_ID_BROWN_GOOMBA_DIE				33101
#define SPRITE_ID_YELOW_GOOMBA_DIE_UPSIDE_DOWN	33102

#define ID_ANI_RED_GOOMBA_WALKING 5211
#define SPRITE_ID_RED_GOOMBA_DIE				33201
#define SPRITE_ID_RED_GOOMBA_DIE_UPSIDE_DOWN	33202

#define ID_ANI_RED_PARAGOOMBA_WALKING 5221
#define ID_ANI_RED_PARAGOOMBA_PRE_FLY 5222
#define ID_ANI_RED_PARAGOOMBA_FLYING 5223



class CGoomba : public CGameObject, public CMoveable, public CDamageable
{
protected:
	CDeltaTimer dyingTimer;
	CDeltaTimer paragoombaStateTimer;
	// 0: brown; 1: red
	int color;
	boolean isParagoomba;
	boolean isParagoombaInitially;
	int fliesLeft = PARAGOOMBA_TOTAL_FLIES;
	float x0, y0;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DIE_WITH_BOUNCE; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithBoBro(LPCOLLISIONEVENT e);

	void HandleTimer(DWORD dt);
public: 	
	CGoomba(float x, float y, int color, boolean isParagoomba);
	virtual void SetState(int state);

	virtual void SetSpeed(float vx, float vy) override {
		this->vx = vx;
		this->vy = vy;
	}
	virtual void GetSpeed(float& vx, float& vy) override {
		vx = this->vx;
		vy = this->vy;
	}
	void TakeDamageFrom(LPGAMEOBJECT obj) override;
	void TakeDamageFromTanookiTail();

	void SetSpawnerDead(bool isDead);
};