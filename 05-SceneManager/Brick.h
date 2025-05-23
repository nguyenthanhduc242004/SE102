#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_STATE_BROKEN	0

class CBrick : public CGameObject {
public:
	CBrick(float x, float y) : CGameObject(x, y) {}
	virtual void SetState(int state);
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsCollidable() { return 1; };
};