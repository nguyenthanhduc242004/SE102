#include "TanookiTail.h"

void CTanookiTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - TANOOKI_TAIL_BBOX_WIDTH / 2;
	top = y - TANOOKI_TAIL_BBOX_HEIGHT / 2 + 4;
	right = left + TANOOKI_TAIL_BBOX_WIDTH;
	bottom = top + TANOOKI_TAIL_BBOX_HEIGHT;
}

void CTanookiTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//HandleTimer(dt);

	/*CMario* mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float marioX, marioY;
	mario->GetPosition(marioX, marioY);
	SetPosition(marioX, marioY);*/

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTanookiTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	CMario* mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CDeltaTimer* marioTailWhipTimer = mario->GetTailWhipTimer();
	if (marioTailWhipTimer->IsRunning() && !hasHit) {
		if (e->nx == 0 && e->ny == 0 && e->dx == 0 && e->dy == 0) {
			if (CBrick* brick = dynamic_cast<CBrick*>(e->obj)) {
				brick->SetState(BRICK_STATE_BROKEN);
				hasHit = true;
			}
			else if (CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj)) {
				if (questionBlock->GetState() == QUESTION_BLOCK_STATE_ACTIVE) {
					questionBlock->TakeDamageFrom(this);
					hasHit = true;
				}
			}
			else if (CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj)) {
				goomba->TakeDamageFrom(this);
				hasHit = true;
			}
			else if (CPiranhaPlant* piranhaPlant = dynamic_cast<CPiranhaPlant*>(e->obj)) {
				piranhaPlant->TakeDamageFrom(this);
				hasHit = true;
			}
			else if (CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj)) {
				koopa->TakeDamageFrom(this);
				hasHit = true;
			}
		}
		//Collision with Goomba, Koopa, BoBro
	}
	else if (!marioTailWhipTimer->IsRunning()) {
		hasHit = false;
	}
}

//void CTanookiTail::HandleTimer(DWORD dt)
//{
//	/*if (spawningTimer.IsRunning()) {
//		spawningTimer.Tick(dt);
//	}
//	if (glidingTimer.IsRunning()) {
//		glidingTimer.Tick(dt);
//	}*/
//}


//void CTanookiTail::SetState(int state)
//{
//	CGameObject::SetState(state);
//	/*CMario* mario;
//	switch (state)
//	{
//	case TanookiTail_STATE_IDLE:
//		break;
//	case TanookiTail_STATE_SPAWNING:
//		y0 = y;
//		spawningTimer.Start();
//		break;
//	case TanookiTail_STATE_FALLING:
//		vx = TanookiTail_GLIDING_SPEED;
//		vy = 0.0f;
//		ay = TanookiTail_GLIDING_GRAVITY;
//		glidingTimer.Start();
//		break;
//	}*/
//}