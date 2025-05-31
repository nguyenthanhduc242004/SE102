#pragma once
#include "GameObject.h"
#include "Mario.h"

#define BOUNCING_COIN_GRAVITY 0.002f
#define BOUNCING_COIN_INITIAL_SPEED 0.47f


#define BOUNCING_COIN_WIDTH 8
#define BOUNCING_COIN_HEIGHT 16

#define BOUNCING_COIN_BOUNCE_TIME 500

#define ID_ANI_BOUNCING_COIN 40000

#define BOUNCING_COIN_STATE_IDLE	QUESTION_BLOCK_ITEM_STATE_IDLE
#define BOUNCING_COIN_STATE_SPAWNING	QUESTION_BLOCK_ITEM_STATE_SPAWNING

class CBouncingCoin : public CGameObject, public CMoveable
{
protected:
	float y0;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return state != BOUNCING_COIN_STATE_IDLE; };
	virtual int IsBlocking() { return 0; }

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
	void SetState(int state);
};