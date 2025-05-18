#include "BouncingCoin.h"
#include "QuestionBlock.h"
#include "Game.h"
#include "FlyingScore.h"
#include "debug.h"


CBouncingCoin::CBouncingCoin(float x, float y) :CGameObject(x, y) {
	state = BOUNCING_COIN_STATE_IDLE;
}

void CBouncingCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BOUNCING_COIN_WIDTH / 2;
	top = y - BOUNCING_COIN_HEIGHT / 2;
	right = left + BOUNCING_COIN_WIDTH;
	bottom = top + BOUNCING_COIN_HEIGHT;
}

void CBouncingCoin::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CBouncingCoin::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	// Return when a BouncingCoin collide with a BouncingCoin??? -> No need
	//if (dynamic_cast<CBouncingCoin*>(e->obj)) return;

	/*if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}*/

	if (dynamic_cast<CQuestionBlock*>(e->obj))
		OnCollisionWithQuestionBlock(e);
}

void CBouncingCoin::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e)
{
	CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);

	// hit the top of QuestionBlock >> Delete coin and show score received
	if (e->ny < 0)
	{
		float x, y;
		questionBlock->GetPosition(x, y);
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->AddScore(x, y - (QUESTION_BLOCK_HEIGHT + FLYING_SCORE_HEIGHT) / 2, FLYING_SCORE_TYPE_100, true);
		mario->AddCoin(1);
		isDeleted = true;
	}
}

void CBouncingCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CBouncingCoin::Render()
{
	if (state == BOUNCING_COIN_STATE_IDLE) return;
	CAnimations::GetInstance()->Get(ID_ANI_BOUNCING_COIN)->Render(x, y);
	//RenderBoundingBox();
}

void CBouncingCoin::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case BOUNCING_COIN_STATE_IDLE:
		break;
	case BOUNCING_COIN_STATE_SPAWNING:
		ay = BOUNCING_COIN_GRAVITY;
		vy = -BOUNCING_COIN_INITIAL_SPEED;
		break;
	}
}