#pragma once
#include "GameObject.h"
#include "DeltaTimer.h"
#include "PlayScene.h"
#include "Mario.h"
#include "DeltaTimer.h"
#define BOOM_BBOX_WIDTH           10
#define BOOM_BBOX_HEIGHT           10

#define BOOM_STATE_IDLE        0
#define BOOM_STATE_1           1
#define BOOM_STATE_2           2
#define BOOM_STATE_3           3
#define BOOM_STATE_4           4
#define BOOM_STATE_5           5
#define BOOM_STATE_6           6
#define BOOM_STATE_7           7
#define BOOM_STATE_8           8

#define BOOM_SPEED_X           0.07f
#define BOOM_SPEED_Y           0.02f
#define BOOM_GRAVITY           0.00005f
#define BOOM_CHANGE_TIME       750   // ms
#define BOOM_PHASE_COUNT       6

#define BOOMERANG_ANI_ID				90
#define BOOMERANG_ANI_IDLE_RIGHT		0
#define BOOMERANG_ANI_RIGHT				1
#define BOOMERANG_ANI_IDLE_LEFT			2
#define BOOMERANG_ANI_LEFT				3

class CBoomerang : public CGameObject, public CMoveable
{
    CDeltaTimer phaseTimer;
    float x0;
    bool isSpawned = false;

public:
    CBoomerang(float x, float y);
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    virtual void Render() override;
    virtual void SetState(int state) override;
    void HandleTimer(DWORD dt);

    void GetSpeed(float& vx, float& vy) {
        vx = this->vx;
        vy = this->vy;
    }
    void SetSpeed(float vx, float vy) {
        this->vx = vx;
        this->vy = vy;
    }
};

