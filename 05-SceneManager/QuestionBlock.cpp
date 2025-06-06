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
	if (state == QUESTION_BLOCK_STATE_BOUNCING) {
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
			else {
				SetState(QUESTION_BLOCK_STATE_DISABLED);
			}
		}
	}

	CGameObject::Update(dt, coObjects);
}
void CQuestionBlock::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (CMario* mario = dynamic_cast<CMario*>(e->obj)) {
		if (!e->nx && !e->ny && !e->dx && !e->dy) {
			float ml, mt, mr, mb, mx, my, l, t, r, b;
			mario->GetBoundingBox(ml, mt, mr, mb);
			mario->GetPosition(mx, my);
			GetBoundingBox(l, t, r, b);

			// Calculate overlap on both axes
			float overlapLeft = mr - l;
			float overlapRight = r - ml;
			float overlapTop = mb - t;
			float overlapBottom = b - mt;

			// Find the minimal translation axis
			float minOverlapX = (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
			float minOverlapY = (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;

			if (std::abs(minOverlapX) < std::abs(minOverlapY)) {
				// Push mario along X
				mx += minOverlapX;
			}
			else {
				// Push mario along Y
				my += minOverlapY;
			}
			mario->SetPosition(mx, my);
		}
	}
}


void CQuestionBlock::Render()
{
	int aniId = ID_ANI_QUESTION_BLOCK_ACTIVE;;
	if (state == QUESTION_BLOCK_STATE_DISABLED || state == QUESTION_BLOCK_STATE_BOUNCING)
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
		break;
	case QUESTION_BLOCK_STATE_BOUNCING:
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

void CQuestionBlock::TakeDamageFrom(LPGAMEOBJECT obj)
{
	if (state == QUESTION_BLOCK_STATE_ACTIVE) {
		if (GetItemID() == ITEM_LEAF && ((CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer())->GetLevel() == MARIO_LEVEL_SMALL) {
			SetItemId(ITEM_MUSHROOM_RED);
		}
			SetState(QUESTION_BLOCK_STATE_BOUNCING);
	}
}