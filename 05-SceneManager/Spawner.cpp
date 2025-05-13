#include "Spawner.h"

void CSpawner::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BRICK_BBOX_WIDTH / 2;
	top = y - BRICK_BBOX_HEIGHT / 2;
	right = left + BRICK_BBOX_WIDTH;
	bottom = top + BRICK_BBOX_HEIGHT;
}

void CSpawner::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//CGame* game = CGame::GetInstance();

	//CMario* mario;
	//mario = (CMario*)((CPlayScene*)game->GetCurrentScene())->GetPlayer();

	//float marioX, marioY;
	//mario->GetPosition(marioX, marioY);

	// hasSpawned is set to false if first: hasSpawned was true and second: spawner is outside of camera ---> avoid setting hasSpawned to false when it is already false;
	if (isDead)
		return;

	if (hasSpawned && obj->IsDeleted()
		&& !CGame::GetInstance()->IsInCamera(x - RESPAWN_OFFSET_CAM_X, DEFAULT_CAM_Y) 
		&& !CGame::GetInstance()->IsInCamera(x + RESPAWN_OFFSET_CAM_X, DEFAULT_CAM_Y)) {
		hasSpawned = false;
	}
	else if (!hasSpawned
		&& (CGame::GetInstance()->IsInCamera(x - RESPAWN_OFFSET_CAM_X, DEFAULT_CAM_Y) || CGame::GetInstance()->IsInCamera(x + RESPAWN_OFFSET_CAM_X, DEFAULT_CAM_Y))) {
		Spawn();
	}

	CGameObject::Update(dt, coObjects);
}

void CSpawner::Spawn()
{
	switch (NPC_ID) {
	case OBJECT_TYPE_GOOMBA:
	{
		obj = new CGoomba(x, y, token3, token4); 
		break;
	}
	case OBJECT_TYPE_KOOPA:
	{
		obj = new CKoopa(x, y, token3, token4);
		break;
	}
	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", NPC_ID);
		return;
	}
	// get directon to spawn
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)scene->GetPlayer();
	float mX, mY;
	mario->GetPosition(mX, mY);
	//by default, NPC will go to the left, so if mario returns from the right, imagine the npc turning around to look to the right
	if (mX > x)
		dynamic_cast<CMoveable*>(obj)->ReverseDirection();
	scene->AddObject(obj);
	hasSpawned = true;
}
