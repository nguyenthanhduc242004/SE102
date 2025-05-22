#include "QuestionBlock.h"
#include "ItemBrick.h"
#include "Brick.h"

CItemBrick::CItemBrick(float x, float y) : CQuestionBlock(x, y)
{
	y0 = y;
	disabled_start = -1;
}

CItemBrick::CItemBrick(float x, float y, int itemID, int hp) : CQuestionBlock(x, y, itemID)
{
	this->hp = hp;
	y0 = y;
	disabled_start = -1;
	this->itemID = itemID;
	SetState(QUESTION_BLOCK_STATE_ACTIVE);
}

void CItemBrick::Render()
{
	int aniId = ID_ANI_BRICK;;
	if (state == QUESTION_BLOCK_STATE_DISABLED && hp == 0)
	{
		aniId = ID_ANI_QUESTION_BLOCK_DISABLED;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

