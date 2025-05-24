#include "QuestionBlock.h"
#include "BouncingCoin.h"
#include "Mushroom.h"
#include "Game.h"
#include "Leaf.h"

CQuestionBlock::CQuestionBlock(float x, float y) :CGameObject(x, y)
{
	y0 = y;
	disabled_start = -1;
}

CQuestionBlock::CQuestionBlock(float x, float y, int itemID) : CQuestionBlock(x, y)
{
	y0 = y;
	disabled_start = -1;
	this->itemID = itemID;
	SetState(QUESTION_BLOCK_STATE_ACTIVE);
}

void CQuestionBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - QUESTION_BLOCK_WIDTH / 2;
	top = y - QUESTION_BLOCK_HEIGHT / 2;
	right = left + QUESTION_BLOCK_WIDTH;
	bottom = top + QUESTION_BLOCK_HEIGHT;
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Bounce the QuestionBlock a bit
	if (state == QUESTION_BLOCK_STATE_DISABLED) {
		ULONGLONG time = GetTickCount64() - disabled_start;
		int pxMoved = floor(time / (QUESTION_BLOCK_BOUNCE_TIME / 2 / QUESTION_BLOCK_BOUNCE_HEIGHT));
		if (time < QUESTION_BLOCK_BOUNCE_TIME / 2)
			y = y0 - pxMoved;
		else if (QUESTION_BLOCK_BOUNCE_TIME / 2 <= time && time < QUESTION_BLOCK_BOUNCE_TIME)
			y = y0 - 2 * QUESTION_BLOCK_BOUNCE_HEIGHT + pxMoved;
		else {
			y = y0;
			if (hp > 0) {
				SetState(QUESTION_BLOCK_STATE_ACTIVE);
			}
		}
	}

	CGameObject::Update(dt, coObjects);
}


void CQuestionBlock::Render()
{
	int aniId = ID_ANI_QUESTION_BLOCK_ACTIVE;;
	if (state == QUESTION_BLOCK_STATE_DISABLED)
	{
		aniId = ID_ANI_QUESTION_BLOCK_DISABLED;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CQuestionBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QUESTION_BLOCK_STATE_DISABLED:
		switch (itemID)
		{
		case ITEM_COIN:
			item = new CBouncingCoin(x, y - (QUESTION_BLOCK_HEIGHT + BOUNCING_COIN_HEIGHT) / 2);
			break;
		case ITEM_COIN_x5:

			break;
		case ITEM_CUSTOM:

			break;
		case ITEM_LEAF:
			item = new CLeaf(x, y - QUESTION_BLOCK_ITEM_Y_OFFSET);
			break;
		case ITEM_MUSHROOM_GREEN:
			item = new CMushroom(x, y - QUESTION_BLOCK_ITEM_Y_OFFSET, MUSHROOM_TYPE_GREEN);
			break;
		case ITEM_MUSHROOM_RED:
			item = new CMushroom(x, y - QUESTION_BLOCK_ITEM_Y_OFFSET, MUSHROOM_TYPE_RED);
			break;
		case ITEM_SWITCH:
			item = new CSwitch(x, y - QUESTION_BLOCK_HEIGHT);
			break;
		}
		if (item == NULL) break;
		hp--;
		CGame::GetInstance()->GetCurrentScene()->AddObject(item);
		//remove and overlay it on top of the item
		CGame::GetInstance()->GetCurrentScene()->RemoveObject(this);
		CGame::GetInstance()->GetCurrentScene()->AddObject(this);
		item->SetState(QUESTION_BLOCK_ITEM_STATE_SPAWNING);
		disabled_start = GetTickCount64();
		break;
	case QUESTION_BLOCK_STATE_ACTIVE:
		break;
	}
}

