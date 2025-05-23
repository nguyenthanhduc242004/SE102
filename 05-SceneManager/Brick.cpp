#include "Brick.h"
#include "PlayScene.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state == BRICK_STATE_BOUNCING) {
		ULONGLONG time = GetTickCount64() - bouncingStart;
		int pxMoved = floor(time / (BRICK_BOUNCE_TIME / 2 / BRICK_BOUNCE_HEIGHT));
		if (time < BRICK_BOUNCE_TIME / 2)
			y = y0 - pxMoved;
		else if (BRICK_BOUNCE_TIME / 2 <= time && time < BRICK_BOUNCE_TIME)
			y = y0 - 2 * BRICK_BOUNCE_HEIGHT + pxMoved;
		else {
			y = y0;
		}
	}

	CGameObject::Update(dt, coObjects);
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BRICK_STATE_BOUNCING:
		bouncingStart = GetTickCount64();
		break;
	case BRICK_STATE_BROKEN:
		CPlayScene* playScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		float brickX, brickY;
		GetPosition(brickX, brickY);
		playScene->AddObject(new CBrickDebris(brickX, brickY, -1, -1));
		playScene->AddObject(new CBrickDebris(brickX, brickY, -1, 1));
		playScene->AddObject(new CBrickDebris(brickX, brickY, 1, -1));
		playScene->AddObject(new CBrickDebris(brickX, brickY, 1, 1));
		Delete();
		break;
	}
}
