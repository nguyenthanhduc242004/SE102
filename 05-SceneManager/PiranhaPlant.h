#pragma once
#include "GameObject.h"
#include <algorithm>

#define PIRANHA_PLANT_WIDTH  16
#define PIRANHA_PLANT_HEAD_HEIGHT 16
#define PIRANHA_PLANT_STEM_HEIGHT 8

//#define ID_ANI_PIRANHA_PLANT_RED		30300
//#define ID_ANI_PIRANHA_PLANT_GREEN	30400
//
//#define PIRANHA_PLANT_GRAVITY		GRAVITY
#define PIRANHA_PLANT_SPEED			0.05f
//#define PIRANHA_PLANT_SPAWNING_SPEED 0.012f
//

#define PIRANHA_PLANT_STATE_HIDDEN			0		
#define PIRANHA_PLANT_STATE_ASCENDING		1		
#define PIRANHA_PLANT_STATE_FULLY_EXPOSED	2
#define PIRANHA_PLANT_STATE_DESCENDING		3

#define PIRANHA_PLANT_STATE_TIME	1500

#define PIRANHA_PLANT_DISTANCE_CLOSE	80

#define PIRANHA_PLANT_TYPE_RED_SHOOTABLE		0
#define PIRANHA_PLANT_TYPE_GREEN_SHOOTABLE		1
#define PIRANHA_PLANT_TYPE_GREEN_UNSHOOTABLE	2

#define SPRITE_ID_PIRANHA_PLANT_RED_SHOOTABLE_MOUTH_OPENING_LEFT_DOWNWARD	191111
#define SPRITE_ID_PIRANHA_PLANT_RED_SHOOTABLE_MOUTH_OPENING_LEFT_UPWARD		191121
#define SPRITE_ID_PIRANHA_PLANT_RED_SHOOTABLE_MOUTH_OPENING_RIGHT_DOWNWARD	191211
#define SPRITE_ID_PIRANHA_PLANT_RED_SHOOTABLE_MOUTH_OPENING_RIGHT_UPWARD	191221
//#define SPRITE_ID_PIRANHA_PLANT_GREEN_SHOOTABLE_MOUTH_OPENING_LEFT	192101
//#define SPRITE_ID_PIRANHA_PLANT_GREEN_SHOOTABLE_MOUTH_OPENING_RIGHT	192201
#define SPRITE_ID_PIRANHA_PLANT_STEM	194001

#define ID_ANI_PIRANHA_PLANT_RED_SHOOTABLE_LEFT		51101
#define ID_ANI_PIRANHA_PLANT_RED_SHOOTABLE_RIGHT	51102
//#define ID_ANI_PIRANHA_PLANT_GREEN_SHOOTABLE_LEFT	52002
//#define ID_ANI_PIRANHA_PLANT_GREEN_SHOOTABLE_RIGHT	52002
//#define ID_ANI_PIRANHA_PLANT_GREEN_UNSHOOTABLE		53001



#define PIRANHA_PLANT_BULLET_SPEED	0.05f

class CPiranhaPlant : public CGameObject, public CMoveable
{
protected:
	ULONGLONG state_start;
	int stem_height;		// Unit: cells
	int type;
	float y0;
	boolean hasFired = false;
	float piranhaPlantHeadX, piranhaPlantHeadY;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);


public:
	CPiranhaPlant(float x, float y, int stem_height, int PIRANHA_PLANT_TYPE) : CGameObject(x, y) {
		this->stem_height = stem_height;
		type = PIRANHA_PLANT_TYPE;
		y0 = y;
		piranhaPlantHeadX = x;
		piranhaPlantHeadY = y + PIRANHA_PLANT_STEM_HEIGHT / 2 - PIRANHA_PLANT_STEM_HEIGHT * stem_height - PIRANHA_PLANT_HEAD_HEIGHT / 2;
		SetState(PIRANHA_PLANT_STATE_HIDDEN);
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

