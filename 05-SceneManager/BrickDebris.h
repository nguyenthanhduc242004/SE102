#pragma once
#include "GameObject.h"
#include "Mario.h"

#define BRICK_DEBRIS_GRAVITY 0.002f
#define BRICK_DEBRIS_INITIAL_VX 0.1f
#define BRICK_DEBRIS_INITIAL_VY 0.4f


#define BRICK_DEBRIS_WIDTH 8
#define BRICK_DEBRIS_HEIGHT 8

#define ID_ANI_BRICK_DEBRIS 90001

class CBrickDebris : public CGameObject, public CMoveable
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

public:
	CBrickDebris(float x, float y, int nx, int ny) : CGameObject(x, y) {
		vx = nx * BRICK_DEBRIS_INITIAL_VX;
		vy = ny == 1 ? -BRICK_DEBRIS_INITIAL_VY : -BRICK_DEBRIS_INITIAL_VY / 2;
		ay = BRICK_DEBRIS_GRAVITY;
	};

	virtual void SetSpeed(float vx, float vy) override {
		this->vx = vx;
		this->vy = vy;
	}
	virtual void GetSpeed(float& vx, float& vy) override {
		vx = this->vx;
		vy = this->vy;
	}
};