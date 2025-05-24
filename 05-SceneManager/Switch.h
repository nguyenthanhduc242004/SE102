#pragma once
#include "GameObject.h"
#include "PlayScene.h"

#define SWITCH_BBOX_WIDTH  16
#define SWITCH_BBOX_HEIGHT 16
#define SWITCH_BBOX_HEIGHT_PRESSED 10

#define ID_ANI_SWITCH_IDLE		77301
#define ID_ANI_SWITCH_PRESSED		77402

#define SWITCH_STATE_IDLE		QUESTION_BLOCK_ITEM_STATE_IDLE
#define SWITCH_STATE_SPAWNING		QUESTION_BLOCK_ITEM_STATE_SPAWNING
#define SWITCH_STATE_PRESSING		QUESTION_BLOCK_ITEM_STATE_SPAWNING + 2
#define SWITCH_STATE_PRESSED		QUESTION_BLOCK_ITEM_STATE_SPAWNING + 3

class CSwitch : public CGameObject
{
protected:
	float y0;
	bool isSpawned = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return state == SWITCH_STATE_SPAWNING; };
	virtual int IsBlocking() { return 0; }

	void HandleTimer(DWORD dt);
public:
	CSwitch(float x, float y) : CGameObject(x, y) {
		y0 = y;
		SetState(SWITCH_STATE_IDLE);
	}
	void SetState(int state);

};

