#pragma once
#include "GameObject.h"
#include "DeltaTimer.h"
#include "Boomerang.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Brick.h"
#include "Platform.h"
#include "Koopa.h"

#define BOOMERANG_BROTHER_BBOX_WIDTH           16
#define BOOMERANG_BROTHER_BBOX_HEIGHT          24

#define BOOMERANG_BROTHER_STATE_WALKING         1
#define BOOMERANG_BROTHER_STATE_DIE            2

#define BB_SPEED                0.035f
#define BB_GRAVITY              0.0005

#define BB_LIMIT_RANGE          60
#define BB_ACTIVE_RANGE         80

#define BB_AIM_TIME             500   // ms
#define BB_THROW_TIME           500
#define BB_RELOAD_TIME          2000
#define BB_CHARGE_TIME          3000
#define BB_MAX_BOOMS            2

#define ID_ANI_BOOMERANG_BROTHER_AIM_RIGHT       8000
#define ID_ANI_BOOMERANG_BROTHER_WALK_RIGHT       8005

#define ID_ANI_BOOMERANG_BROTHER_AIM_LEFT        8001
#define ID_ANI_BOOMERANG_BROTHER_WALK_LEFT        8006

class CBoomerang;
class CBoBro : public CGameObject, public CMoveable, public CDamageable
{
protected:
    CDeltaTimer aimTimer;
    CDeltaTimer throwTimer;
    CDeltaTimer reloadTimer;
    CDeltaTimer chargeTimer;

    CBoomerang* boomerang = NULL;
    int boomerangsThrown = 0;
    float x0;

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    
    virtual int IsCollidable() { return state != BOOMERANG_BROTHER_STATE_DIE; }
    virtual int IsBlocking() { return 0; }
    virtual void OnNoCollision(DWORD dt);

    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    void HoldBoomerang();
    void ThrowBoomerang();
    void HandleTimer(DWORD dt);
public:
    CBoBro(float x, float y);
    virtual void SetState(int state);
    virtual void TakeDamageFrom(LPGAMEOBJECT obj);
    void GetSpeed(float& vx, float& vy) {
        vx = this->vx;
        vy = this->vy;
    }
    void SetSpeed(float vx, float vy) {
        this->vx = vx;
        this->vy = vy;
    }
};

