#include "Boomerang.h"


CBoomerang::CBoomerang(float x, float y)
    : CGameObject(x, y)
{
    x0 = x;
    ay = BOOM_GRAVITY;
    nx = -1;
    isSpawned = false;
    //SetState(BOOM_STATE_IDLE);
    SetState(BOOM_STATE_1);
}

void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x + BOOM_BBOX_WIDTH/2;
    top = y - BOOM_BBOX_HEIGHT;
    right = left + BOOM_BBOX_WIDTH;
    bottom = top + BOOM_BBOX_HEIGHT;
}



void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (!isSpawned) return;

    x += vx * dt;
    y += vy * dt;

    CGame* game = CGame::GetInstance();

    if (!(game->IsInCamera(x - BOOM_BBOX_WIDTH, y + BOOM_BBOX_HEIGHT) || game->IsInCamera(x - BOOM_BBOX_WIDTH, y - BOOM_BBOX_HEIGHT)
        && game->IsInCamera(x + BOOM_BBOX_WIDTH, y + BOOM_BBOX_HEIGHT) || game->IsInCamera(x + BOOM_BBOX_WIDTH, y - BOOM_BBOX_HEIGHT)))
    {
        isDeleted = true;
    }

    vy += ay * dt;
    vx += ax * dt;


    HandleTimer(dt);

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CBoomerang::HandleTimer(DWORD dt) {
    // boomerang exists for as long as timer is running, so timer will alway run when boomerang exist, no need checking
    phaseTimer.Tick(dt);

    if (state == BOOM_STATE_1 && phaseTimer.HasPassed(BOOM_CHANGE_TIME)) {
        SetState(BOOM_STATE_2);
    }
    else if (state > BOOM_STATE_1 && state < BOOM_STATE_8 && phaseTimer.HasPassed(BOOM_CHANGE_TIME / 3)) {
        SetState(state + 1);
    }
    else if (state == BOOM_STATE_8 && phaseTimer.HasPassed(BOOM_CHANGE_TIME)) {
        Delete();
    }

}
void CBoomerang::Render()
{
    if (!isSpawned) return;
    int ani = (nx > 0)
        ? (state == BOOM_STATE_IDLE ? BOOMERANG_ANI_IDLE_RIGHT : BOOMERANG_ANI_RIGHT)
        : (state == BOOM_STATE_IDLE ? BOOMERANG_ANI_IDLE_LEFT : BOOMERANG_ANI_LEFT);
    //for now
    CAnimations::GetInstance()->Get(ID_ANI_BULLET_LEFT)->Render(x, y);
}

void CBoomerang::SetState(int s)
{
    CGameObject::SetState(s);
    phaseTimer.Reset();
    switch (s) {
    case BOOM_STATE_IDLE:
        vx = vy = 0; break;
    case BOOM_STATE_1:
        vx = nx * BOOM_SPEED_X; vy = -BOOM_SPEED_Y; phaseTimer.Start(); isSpawned = true; break;
    case BOOM_STATE_2:
        vy = 0; break;
    case BOOM_STATE_3:
        vy = BOOM_SPEED_Y; break;
    case BOOM_STATE_4:
        vy = BOOM_SPEED_Y * 1.25f; break;
    case BOOM_STATE_5:
        vx = -nx * BOOM_SPEED_X; vy = BOOM_SPEED_Y; break;
    case BOOM_STATE_6:
        vy = 0; break;
    case BOOM_STATE_7:
        vy = -BOOM_SPEED_Y; break;
    case BOOM_STATE_8:
        vy = -BOOM_SPEED_Y * 1.25f; break;
    }
}
