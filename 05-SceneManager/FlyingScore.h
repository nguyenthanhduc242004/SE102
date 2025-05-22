#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#include "DeltaTimer.h"

#define FLYING_SCORE_GRAVITY 0.0001f
#define FLYING_SCORE_INITIAL_SPEED 0.1f


#define FLYING_SCORE_WIDTH 17
#define FLYING_SCORE_HEIGHT 8

#define FLYING_SCORE_FLYING_TIME 700

#define FLYING_SCORE_TYPE_100 100
#define FLYING_SCORE_TYPE_200 200
#define FLYING_SCORE_TYPE_400 400
#define FLYING_SCORE_TYPE_800 800
#define FLYING_SCORE_TYPE_1000 1000
#define FLYING_SCORE_TYPE_2000 2000
#define FLYING_SCORE_TYPE_4000 4000
#define FLYING_SCORE_TYPE_8000 8000
#define FLYING_SCORE_TYPE_1_LIFE 1


class CFlyingScore : public CGameObject, public CMoveable
{
protected:
	CDeltaTimer liveTimer;
	int type;
	int sprite_id;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

public:
	CFlyingScore(float x, float y, int type);

	virtual void SetSpeed(float vx, float vy) override {
		this->vx = vx;
		this->vy = vy;
	}
	virtual void GetSpeed(float& vx, float& vy) override {
		vx = this->vx;
		vy = this->vy;
	}
};