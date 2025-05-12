#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#pragma region itemID
#define ITEM_COIN				0
#define ITEM_MUSHROOM_RED		1
#define ITEM_LEAF				2
#define ITEM_MUSHROOM_GREEN		3
#define ITEM_COIN_x5			4
#define ITEM_SWITCH				5	
#define ITEM_CUSTOM				6
#pragma endregion


#define ID_ANI_QUESTION_BLOCK_DISABLED 30000
#define ID_ANI_QUESTION_BLOCK_ACTIVE 30001

#define QUESTION_BLOCK_WIDTH 16
#define QUESTION_BLOCK_HEIGHT 16

#define QUESTION_BLOCK_STATE_DISABLED 100
#define QUESTION_BLOCK_STATE_ACTIVE 200


#define QUESTION_BLOCK_BOUNCE_TIME 200

#define QUESTION_BLOCK_BOUNCE_HEIGHT 8
#define QUESTION_BLOCK_ITEM_Y_OFFSET 4

#define QUESTION_BLOCK_ITEM_STATE_IDLE	0
#define QUESTION_BLOCK_ITEM_STATE_SPAWNING	1


class CQuestionBlock : public CGameObject {
protected:
	ULONGLONG disabled_start;
	// The original coordinate of the object (For bouncing logic)
	float y0;

	int itemID;
	//	The item the QuestionBlock will be storing
	CGameObject* item;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; }

public:
	CQuestionBlock(float x, float y);
	CQuestionBlock(float x, float y, int itemID);
	virtual void SetState(int state);
	int GetItemID() {
		return itemID;
	}
	void SetItemId(int itemID) {
		this->itemID = itemID;
	}
};
