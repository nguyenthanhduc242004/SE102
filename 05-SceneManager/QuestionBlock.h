#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_QUESTION_BLOCK_DISABLED 30000
#define ID_ANI_QUESTION_BLOCK_ACTIVE 30001

#define QUESTION_BLOCK_WIDTH 16
#define QUESTION_BLOCK_HEIGHT 16

#define QUESTION_BLOCK_STATE_DISABLED 100
#define QUESTION_BLOCK_STATE_ACTIVE 200


#define QUESTION_BLOCK_BOUNE_TIME 200

#define QUESTION_BLOCK_BOUNCE_HEIGHT 8



class CQuestionBlock : public CGameObject {
protected:
	ULONGLONG lastFrameTime;
	ULONGLONG disabled_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; }

public:
	CQuestionBlock(float x, float y);
	virtual void SetState(int state);
};
