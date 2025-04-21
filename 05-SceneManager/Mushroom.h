#pragma once
#include "GameObject.h"
#include <algorithm>
#include "Brick.h"

#define MUSHROOM_BBOX_WIDTH  16
#define MUSHROOM_BBOX_HEIGHT 16

#define ID_ANI_MUSHROOM_RED		30300
#define ID_ANI_MUSHROOM_GREEN	30400

#define MUSHROOM_GRAVITY		0.0012f
#define MUSHROOM_SPEED			0.06f

#define MUSHROOM_STATE_IDLE		0
#define MUSHROOM_STATE_UP		100
#define MUSHROOM_STATE_WALKING		111

#define MUSHROOM_TYPE_RED		3
#define MUSHROOM_TYPE_GREEN		5

class CMushroom : public CGameObject , public CMoveable
{
protected:
	BOOLEAN isSpawned;
	int type;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CMushroom(float x, float y) : CGameObject(x, y) {
		isSpawned = true;
		maxVx = MUSHROOM_SPEED;
		ax = 0.0f;
		ay = MUSHROOM_GRAVITY;
		type = MUSHROOM_TYPE_RED;
		//very temporary
		//CMario* mario;
		//mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		//vx = mario->GetDirection() * MUSHROOM_SPEED;
		vx = MUSHROOM_SPEED;

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

