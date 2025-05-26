#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "Mario.h"
#include "PlayScene.h"

#define TANOOKI_TAIL_BBOX_WIDTH  32
#define TANOOKI_TAIL_BBOX_HEIGHT 12

class CTanookiTail : public CGameObject
{
protected:
	bool hasHit = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() {
		//RenderBoundingBox();
	};

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	//void HandleTimer(DWORD dt);
public:
	CTanookiTail(float x, float y) : CGameObject(x, y) {}
	//void SetState(int state);
};

