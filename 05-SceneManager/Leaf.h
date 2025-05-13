#pragma once
#include "GameObject.h"
#include "QuestionBlock.h"
#include "Utils.h"
#include "Mario.h"
#include "PlayScene.h"

#define LEAF_BBOX_WIDTH  16
#define LEAF_BBOX_HEIGHT 16

#define ID_ANI_LEAF_LEFT		70300
#define ID_ANI_LEAF_RIGHT		70400

#define LEAF_STATE_IDLE		QUESTION_BLOCK_ITEM_STATE_IDLE
#define LEAF_STATE_SPAWNING		QUESTION_BLOCK_ITEM_STATE_SPAWNING
#define LEAF_STATE_FALLING		QUESTION_BLOCK_ITEM_STATE_SPAWNING + 3

#define LEAF_GRAVITY		0.00012f
#define LEAF_GLIDING_GRAVITY	LEAF_GRAVITY/5
#define LEAF_SPAWNING_SPEED			0.2f
#define LEAF_GLIDING_SPEED			0.04f

#define LEAF_START_FALLING_HEIGHT	 96

#define LEAF_GLIDING_TURNING_TIME	700

#define ID_ANI_LEAF_SPAWNING	80001
#define ID_ANI_LEAF_FALLING		80002

class CLeaf : public CGameObject, public CMoveable
{
protected:
	CDeltaTimer spawningTimer;
	CDeltaTimer glidingTimer;
	bool isSpawned = false;
	float y0;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return state != LEAF_STATE_IDLE; };
	virtual int IsBlocking() { return 0; }

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void HandleTimer(DWORD dt);
public:
	CLeaf(float x, float y) : CGameObject(x, y) {
		y0 = 0.0f;
		state = LEAF_STATE_IDLE;
	}
	void SetState(int state);

	virtual void SetSpeed(float vx, float vy) override {
		this->vx = vx;
		this->vy = vy;
	}
	virtual void GetSpeed(float& vx, float& vy) override {
		vx = this->vx;
		vy = this->vy;
	}
};

