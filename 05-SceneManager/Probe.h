#pragma once
#include "GameObject.h"
#include "PlayScene.h"
#include "Debug.h"

#define PROBE_BB_WIDTH 3
#define PROBE_BB_HEIGHT 3
#define PROBE_FALL_THRESHOLD 9
class CProbe : public CGameObject, public CMoveable
{
protected:
	float x0, y0;	//soft position, probe will strictly stick to x0, and y will be reseted to y0 if go over a threshold
	bool isGroundAhead;
	bool isActive;


public:
	CProbe(float x, float y) : CGameObject(x, y) {
		isGroundAhead = false;
		isActive = false;
		ay = GRAVITY;
		x0 = 0.0f;
		y0 = 0.0f;
	};

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual int IsCollidable() { return 1; };

	virtual void OnNoCollision(DWORD dt) {
		if (isActive) isGroundAhead = false;
	};

	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {
		if (!e->obj->IsBlocking()) return;
		if (!e->nx && !e->ny && !e->dx && !e->dy) return;
		vy = 0;
		isGroundAhead = true;
		isActive = true;
	};

	virtual int IsBlocking() { return 0; }

	bool IsGroundAhead() {
		// idea is that if the npc doesnt care (isActive == false) then the npc should 
		// by default treat itself as if it is on the ground (by default)--->is ground ahead to them? they dont care--->yes (true)
		// not airborn (non-default behavior)
		return (isActive) ? isGroundAhead : true;
	}
	void GetSpeed(float& vx, float& vy) {
		vx = this->vx;
		vy = this->vy;
	}
	void SetSpeed(float vx, float vy) {
		this->vx = vx;
		this->vy = vy;
	}
	void SetSoftPosition(float x0, float y0) {
		this->x0 = x0;
		this->y0 = y0;
	}
	void Reset() {
		isActive = false;
		x = x0;
		y = y0;
	}
};

