#pragma once
#include "DeltaTimer.h"
#include "GameObject.h"
#include "Platform.h"
#include "QuestionBlock.h"
#include "Brick.h"
#include "PlayScene.h"


#define KOOPAS_GRAVITY             GRAVITY
#define KOOPAS_WALKING_SPEED       0.05f
#define KOOPAS_SPINNING_SPEED      0.2f

#define KOOPAS_BBOX_WIDTH          16
#define KOOPAS_BBOX_HEIGHT         24
#define KOOPAS_BBOX_HEIGHT_SHELL   14

//#define KOOPAS_DIE_DEFLECT_SPEED   0.3f
#define KOOPAS_DYING_TIMEOUT 500.0f
#define KOOPAS_SHELL_TIMEOUT       4000.0f
#define KOOPAS_REVIVING_TIMEOUT		2000.0f

#define KOOPAS_STATE_WALKING_LEFT   100
#define KOOPAS_STATE_WALKING_RIGHT  101
#define KOOPAS_STATE_SHELL          200
#define KOOPAS_STATE_REVIVING		300
#define KOOPAS_STATE_SPINNING_LEFT       400
#define KOOPAS_STATE_SPINNING_RIGHT       500
#define KOOPAS_STATE_DIE            600

#define ID_ANI_KOOPAS_WALKING_LEFT      6000
#define ID_ANI_KOOPAS_WALKING_RIGHT      6100
#define ID_ANI_KOOPAS_SHELL        6200
#define ID_ANI_KOOPAS_REVIVING        6300
#define ID_ANI_KOOPAS_SPINNING     6400
#define ID_ANI_KOOPAS_DIE          6500

class CKoopa : public CGameObject, public CMoveable
{
protected:
	bool isHeld;
	CDeltaTimer shellTimer;
	CDeltaTimer revivingTimer;
	CDeltaTimer dyingTimer;
	//CDeltaTimer respawnTimer;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return state != KOOPAS_STATE_DIE; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithGoomba(LPCOLLISIONEVENT e);

	virtual bool IsGroundAhead(LPCOLLISIONEVENT e);
	void HandleTimer(DWORD dt);
public:
	CKoopa(float x, float y) : CGameObject(x, y) {
		vx = -KOOPAS_WALKING_SPEED;
		ay = KOOPAS_GRAVITY;
		nx = -1;
		SetState(KOOPAS_STATE_WALKING_LEFT);
		vx = 0;
	}
	virtual void SetState(int state);

	virtual void SetSpeed(float vx, float vy) override {
		this->vx = vx;
		this->vy = vy;
	}

	virtual void GetSpeed(float& vx, float& vy) override {
		vx = this->vx;
		vy = this->vy;
	}
};
