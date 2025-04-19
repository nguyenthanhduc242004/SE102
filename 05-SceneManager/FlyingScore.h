#pragma once
#include "GameObject.h"

#define FLYING_SCORE_GRAVITY 0.0001f
#define FLYING_SCORE_INITIAL_SPEED 0.1f


#define FLYING_SCORE_WIDTH 17
#define FLYING_SCORE_HEIGHT 9

#define FLYING_SCORE_FLYING_TIME 700

#define FLYING_SCORE_TYPE_100 100
#define FLYING_SCORE_TYPE_200 200
#define FLYING_SCORE_TYPE_400 400
#define FLYING_SCORE_TYPE_800 800
#define FLYING_SCORE_TYPE_1000 1000
#define FLYING_SCORE_TYPE_2000 2000
#define FLYING_SCORE_TYPE_4000 4000
#define FLYING_SCORE_TYPE_8000 8000

#define SPRITE_ID_FLYING_SCORE_100 160001
#define SPRITE_ID_FLYING_SCORE_200 160002
#define SPRITE_ID_FLYING_SCORE_400 160003
#define SPRITE_ID_FLYING_SCORE_800 160004
#define SPRITE_ID_FLYING_SCORE_1000 160005
#define SPRITE_ID_FLYING_SCORE_2000 160006
#define SPRITE_ID_FLYING_SCORE_4000 160007
#define SPRITE_ID_FLYING_SCORE_8000 160008

class CFlyingScore : public CGameObject, public CMoveable
{
protected:
	//float ax;				
	//float ay; 

	//ULONGLONG active_start;

	ULONGLONG created_start;
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