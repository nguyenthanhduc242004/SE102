#include "Switch.h"

void CSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == SWITCH_STATE_PRESSING) {
		SetState(SWITCH_STATE_PRESSED);
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		for (LPGAMEOBJECT obj : *coObjects) {
			if (dynamic_cast<CBrick*>(obj)) {
				float oX, oY;
				obj->GetPosition(oX, oY);
				obj->Delete();
				scene->AddObject(new CCoin(oX, oY));
			}
		}
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CSwitch::Render()
{
	if (!isSpawned)	return;
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = ID_ANI_SWITCH_IDLE;

	if (state == SWITCH_STATE_PRESSED)
		aniId = ID_ANI_SWITCH_PRESSED;

	animations->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CSwitch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - SWITCH_BBOX_WIDTH / 2;
	top = y - SWITCH_BBOX_HEIGHT / 2;
	right = left + SWITCH_BBOX_WIDTH;
	bottom = top + SWITCH_BBOX_HEIGHT;
}

void CSwitch::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SWITCH_STATE_IDLE:
		isSpawned = false;
		break;
	case SWITCH_STATE_SPAWNING:
		isSpawned = true;
		break;
	case SWITCH_STATE_PRESSING:
		break;
	case SWITCH_STATE_PRESSED:
		y += SWITCH_BBOX_HEIGHT - SWITCH_BBOX_HEIGHT_PRESSED;
		break;
	}
}