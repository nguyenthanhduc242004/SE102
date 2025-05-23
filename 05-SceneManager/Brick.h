#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "DeltaTimer.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_STATE_BROKEN	0
#define BRICK_STATE_BOUNCING 1

#define BRICK_BOUNCE_HEIGHT		8
#define BRICK_BOUNCE_TIME		200

class CBrick : public CGameObject {
protected:
	float y0;
	ULONGLONG bouncingStart = -1;
public:
	CBrick(float x, float y) : CGameObject(x, y) {
		y0 = y;
	}
	virtual void SetState(int state);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsCollidable() { return 1; };
};