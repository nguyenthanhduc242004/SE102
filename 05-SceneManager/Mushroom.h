#pragma once
#include "GameObject.h"
#include <algorithm>
#include "Brick.h"
#include "QuestionBlock.h"

#define MUSHROOM_WIDTH  16
#define MUSHROOM_HEIGHT 16

#define ID_ANI_MUSHROOM_RED		30300
#define ID_ANI_MUSHROOM_GREEN	30400

#define MUSHROOM_GRAVITY		GRAVITY
#define MUSHROOM_SPEED			0.06f
#define MUSHROOM_SPAWNING_SPEED 0.012f

#define MUSHROOM_STATE_IDLE		QUESTION_BLOCK_ITEM_STATE_IDLE
#define MUSHROOM_STATE_SPAWNING		QUESTION_BLOCK_ITEM_STATE_SPAWNING
#define MUSHROOM_STATE_WALKING		QUESTION_BLOCK_ITEM_STATE_SPAWNING + 3

#define MUSHROOM_TYPE_RED		3
#define MUSHROOM_TYPE_GREEN		5

class CMushroom : public CGameObject, public CMoveable
{
protected:
	ULONGLONG spawning_start;
	int type;
	float y0;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);


public:
	CMushroom(float x, float y) : CGameObject(x, y) {
		y0 = 0.0f;
		type = MUSHROOM_TYPE_RED;
	}
	CMushroom(float x, float y, int MUSHROOM_TYPE) : CMushroom(x, y) {
		type = MUSHROOM_TYPE;
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
	int GetType() {
		return type;
	}
};

