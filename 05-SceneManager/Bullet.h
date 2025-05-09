#pragma once
#include "GameObject.h"

#define BULLET_SPEED 0.05f

#define BULLET_WIDTH	8
#define BULLET_HEIGHT	8

#define ID_ANI_BULLET_LEFT	60001
#define ID_ANI_BULLET_RIGHT	60002

class CBullet : public CGameObject, public CMoveable
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
public:
	CBullet(float x, float y, float vx, float vy) : CGameObject(x, y) {
		this->vx = vx;
		this->vy = vy;
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