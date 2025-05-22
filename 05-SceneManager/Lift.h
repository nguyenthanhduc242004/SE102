#pragma once
#include "SideCollidablePlatform.h"
#include "DeltaTimer.h"
#include "Mario.h"
#include "PlayScene.h"

#define LIFT_START_FALLING_TIME		250
#define LIFT_RESPAWN_TIME			5000

#define LIFT_STATE_GLIDING	0
#define	LIFT_STATE_FALLING	1
#define LIFT_SPEED_X	0.05f


class CLift : public CSideCollidablePlatform, public CMoveable
{
protected:
	CDeltaTimer respawnTimer;
	CDeltaTimer startFallingTimer;
	float x0, y0;
	float maxRange;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void HandleTimer(DWORD dt);
	// Dont actually delete, just let it sit out of camera
	virtual void Delete() override {
		ay = 0;
		vy = 0;
	}
public:
	CLift(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end, float maxRange)
		: CSideCollidablePlatform(x, y, cell_width, cell_height, length, sprite_id_begin, sprite_id_middle, sprite_id_end) {
		x0 = x;
		y0 = y;
		this->maxRange = maxRange;
		nx = -1;
		SetState(LIFT_STATE_GLIDING);
	}

	void SetState(int state);
	void GetSpeed(float& vx, float& vy) {
		vx = this->vx;
		vy = this->vy;
	}
	void SetSpeed(float vx, float vy) {
		this->vx = vx;
		this->vy = vy;
	}
};

