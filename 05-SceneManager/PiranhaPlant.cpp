#include "PiranhaPlant.h"
#include "Mario.h"
#include "Bullet.h"
#include "Game.h"
#include "Boomerang.h"

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == PIRANHA_PLANT_STATE_DIE) {
		if (dieTimer.HasPassed(PIRANHA_PLANT_DIE_TIME)) {
			this->Delete();
		}
		else if (dieTimer.IsRunning())
			dieTimer.Tick(dt);
	}

	//CPiranhaPlant is also a slap-on-like object, it can go through blocking objects(pipe) ---> update its position here, so when it's colliding with the pipe, it wont stop, OnNoCollision causes the stuck bug
	x += vx * dt;
	y += vy * dt;

	CGame* game = CGame::GetInstance();

	CMario* mario;
	mario = (CMario*)((CPlayScene*)game->GetCurrentScene())->GetPlayer();

	float marioX, marioY;
	mario->GetPosition(marioX, marioY);

	float piranhaPlantHeight = PIRANHA_PLANT_HEAD_HEIGHT + PIRANHA_PLANT_STEM_HEIGHT * stem_height;
	if (!(game->IsInCamera(x - PIRANHA_PLANT_WIDTH / 2, y + piranhaPlantHeight / 2) || game->IsInCamera(x - PIRANHA_PLANT_WIDTH / 2, y - piranhaPlantHeight / 2)
		&& game->IsInCamera(x + PIRANHA_PLANT_WIDTH / 2, y + piranhaPlantHeight / 2) || game->IsInCamera(x + PIRANHA_PLANT_WIDTH / 2, y - piranhaPlantHeight / 2))) {
		if (state != PIRANHA_PLANT_STATE_HIDDEN) {
			SetState(PIRANHA_PLANT_STATE_HIDDEN);
			// To make piranha plant ascend as soon as it's in camera again
			state_start = 0;
		}
	}
	else if (state == PIRANHA_PLANT_STATE_HIDDEN && GetTickCount64() - state_start > PIRANHA_PLANT_STATE_TIME 
		&& abs(piranhaPlantHeadX - marioX) >= 32) {
		SetState(PIRANHA_PLANT_STATE_ASCENDING);
	} 
	else if (state == PIRANHA_PLANT_STATE_ASCENDING && y <= y0) {
		SetState(PIRANHA_PLANT_STATE_FULLY_EXPOSED);
	} 
	else if (state == PIRANHA_PLANT_STATE_FULLY_EXPOSED && GetTickCount64() - state_start > PIRANHA_PLANT_STATE_TIME) {
		SetState(PIRANHA_PLANT_STATE_DESCENDING);
	} 
	else if (state == PIRANHA_PLANT_STATE_DESCENDING && y >= y0 + PIRANHA_PLANT_HEAD_HEIGHT + PIRANHA_PLANT_STEM_HEIGHT * stem_height) {
		SetState(PIRANHA_PLANT_STATE_HIDDEN);
	}

	if (state == PIRANHA_PLANT_STATE_FULLY_EXPOSED && GetTickCount64() - state_start > PIRANHA_PLANT_STATE_TIME / 2) {
		if (!hasFired && CGame::GetInstance()->IsInCamera(x, y)) {
			float bulletVX, bulletVY;
			// Mario in bottom left of piranha plant head
			if (marioX <= piranhaPlantHeadX && marioY >= piranhaPlantHeadY) {
				bulletVX = -BULLET_SPEED;
				bulletVY = BULLET_SPEED;
			}
			// Mario in top left of piranha plant head
			else if (marioX <= piranhaPlantHeadX && marioY < piranhaPlantHeadY) {
				bulletVX = -BULLET_SPEED;
				bulletVY = -BULLET_SPEED;
			}
			// Mario in bottom right of piranha plant head
			else if (marioX > piranhaPlantHeadX && marioY >= piranhaPlantHeadY) {
				bulletVX = BULLET_SPEED;
				bulletVY = BULLET_SPEED;
			}
			// Mario in top right of piranha plant head
			else {
				bulletVX = BULLET_SPEED;
				bulletVY = -BULLET_SPEED;
			}

			if (abs(piranhaPlantHeadX - marioX) >= PIRANHA_PLANT_DISTANCE_CLOSE) {
				CGame::GetInstance()->GetCurrentScene()->AddObject(new CBullet(piranhaPlantHeadX, piranhaPlantHeadY, bulletVX, bulletVY / 3));
			}
			else {
				CGame::GetInstance()->GetCurrentScene()->AddObject(new CBullet(piranhaPlantHeadX, piranhaPlantHeadY, bulletVX, bulletVY));
			}
			hasFired = true;
		}
	}

	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}

void CPiranhaPlant::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == PIRANHA_PLANT_STATE_DIE) {
		aniId = ID_ANI_PIRANHA_PLANT_DIE;
		animations->Get(aniId)->Render(x, yDie + PIRANHA_PLANT_STEM_HEIGHT / 2 - PIRANHA_PLANT_STEM_HEIGHT * stem_height - PIRANHA_PLANT_HEAD_HEIGHT / 2);
		return;
	}

	CSprites* s = CSprites::GetInstance();
	int spriteId = -1;

	float yy = y;

	for (int i = 0; i < this->stem_height; i++) {
		s->Get(SPRITE_ID_PIRANHA_PLANT_STEM)->Draw(x, yy);
		yy -= PIRANHA_PLANT_STEM_HEIGHT;
	}

	yy -= (PIRANHA_PLANT_HEAD_HEIGHT - PIRANHA_PLANT_STEM_HEIGHT) / 2;

	CMario* mario;
	mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	float marioX, marioY;
	mario->GetPosition(marioX, marioY);

	if (type == PIRANHA_PLANT_TYPE_RED_SHOOTABLE) {
		if (state == PIRANHA_PLANT_STATE_FULLY_EXPOSED) {
			// Mario in bottom left of piranha plant head
			if (marioX <= piranhaPlantHeadX && marioY >= piranhaPlantHeadY)
				spriteId = SPRITE_ID_PIRANHA_PLANT_RED_SHOOTABLE_MOUTH_OPENING_LEFT_DOWNWARD;
			// Mario in top left of piranha plant head
			else if (marioX <= piranhaPlantHeadX && marioY < piranhaPlantHeadY)
				spriteId = SPRITE_ID_PIRANHA_PLANT_RED_SHOOTABLE_MOUTH_OPENING_LEFT_UPWARD;
			// Mario in bottom right of piranha plant head
			else if (marioX > piranhaPlantHeadX && marioY >= piranhaPlantHeadY)
				spriteId = SPRITE_ID_PIRANHA_PLANT_RED_SHOOTABLE_MOUTH_OPENING_RIGHT_DOWNWARD;
			// Mario in top right of piranha plant head
			else
				spriteId = SPRITE_ID_PIRANHA_PLANT_RED_SHOOTABLE_MOUTH_OPENING_RIGHT_UPWARD;

			s->Get(spriteId)->Draw(x, yy);
		}
		else {
			if (marioX <= piranhaPlantHeadX)
				aniId = ID_ANI_PIRANHA_PLANT_RED_SHOOTABLE_LEFT;
			else 
				aniId = ID_ANI_PIRANHA_PLANT_RED_SHOOTABLE_RIGHT;

			animations->Get(aniId)->Render(x, yy);
		}
	}
	/*else if (type == PIRANHA_PLANT_TYPE_GREEN_SHOOTABLE) {
		int direction = mario->GetDirection();
		if (direction == 1) {
			aniId = ID_ANI_PIRANHA_PLANT_GREEN_SHOOTABLE_RIGHT;
			spriteId = SPRITE_ID_PIRANHA_PLANT_GREEN_SHOOTABLE_MOUTH_OPENING_RIGHT;
		}
		else if (direction == -1) {
			aniId = ID_ANI_PIRANHA_PLANT_GREEN_SHOOTABLE_LEFT;
			spriteId = SPRITE_ID_PIRANHA_PLANT_GREEN_SHOOTABLE_MOUTH_OPENING_RIGHT;
		}
	}
	else if (type == PIRANHA_PLANT_TYPE_GREEN_UNSHOOTABLE) {
		aniId = ID_ANI_PIRANHA_PLANT_GREEN_UNSHOOTABLE;
	}*/

	//RenderBoundingBox();
}



void CPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PIRANHA_PLANT_WIDTH / 2;
	bottom = y + PIRANHA_PLANT_STEM_HEIGHT/ 2;
	top = bottom - this->stem_height * PIRANHA_PLANT_STEM_HEIGHT - PIRANHA_PLANT_HEAD_HEIGHT;
	right = left + PIRANHA_PLANT_WIDTH;
}

void CPiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		
	case PIRANHA_PLANT_STATE_DIE:
		dieTimer.Start();
		yDie = y;
		break;
	case PIRANHA_PLANT_STATE_HIDDEN:
		state_start = GetTickCount64();
		y = y0 + PIRANHA_PLANT_HEAD_HEIGHT + PIRANHA_PLANT_STEM_HEIGHT * stem_height;
		vy = 0;
		hasFired = false;
		break;
	case PIRANHA_PLANT_STATE_ASCENDING:
		vy = -PIRANHA_PLANT_SPEED;
		break;
	case PIRANHA_PLANT_STATE_FULLY_EXPOSED:
		state_start = GetTickCount64();
		y = y0;
		vy = 0;
		break;
	case PIRANHA_PLANT_STATE_DESCENDING:
		vy = PIRANHA_PLANT_SPEED;
		break;
	}
}

void CPiranhaPlant::TakeDamageFrom(LPGAMEOBJECT obj)
{
	// damage caused directly by Mario's stomping
	if (CKoopa* koopa = dynamic_cast<CKoopa*>(obj)) {
		//this->Delete();
		SetState(PIRANHA_PLANT_STATE_DIE);
		return;
	}
	if (CMario* mario = dynamic_cast<CMario*>(obj)) {
		if (mario->GetLevel() == MARIO_LEVEL_TANOOKI)
			SetState(PIRANHA_PLANT_STATE_DIE);
		return;
	}
}