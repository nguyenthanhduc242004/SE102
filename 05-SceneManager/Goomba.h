#pragma once
#include "GameObject.h"
#include "Platform.h"
#include "DeltaTimer.h"
#include "Koopa.h"

#define GOOMBA_GRAVITY GRAVITY
#define GOOMBA_WALKING_SPEED 0.05f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DYING_TIMEOUT 500.0f

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_WITH_BOUNCE  300

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_GOOMBA_DIE_WITH_BOUNCE 5002

class CGoomba : public CGameObject, public CMoveable, public CDamageable
{
protected:
	CDeltaTimer dyingTimer;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DIE_WITH_BOUNCE; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void HandleTimer(DWORD dt);
public: 	
	CGoomba(float x, float y);
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
};