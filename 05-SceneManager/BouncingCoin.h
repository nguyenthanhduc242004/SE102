#pragma once
#include "GameObject.h"

#define BOUNCING_COIN_GRAVITY 0.002f
#define BOUNCING_COIN_INITIAL_SPEED 0.47f


#define BOUNCING_COIN_WIDTH 8
#define BOUNCING_COIN_HEIGHT 16

#define BOUNCING_COIN_BOUNCE_TIME 500

#define ID_ANI_BOUNCING_COIN 40000

class CBouncingCoin : public CGameObject, public CMoveable
{
protected:
	//float ax;				
	//float ay; 

	//ULONGLONG active_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e);

public:
	CBouncingCoin(float x, float y);

	virtual void SetSpeed(float vx, float vy) override {
		this->vx = vx;
		this->vy = vy;
	}
	virtual void GetSpeed(float& vx, float& vy) override {
		vx = this->vx;
		vy = this->vy;
	}
};