#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class CItemBrick : public CQuestionBlock {
protected:
	virtual void Render();
public:
	CItemBrick(float x, float y);
	CItemBrick(float x, float y, int itemID, int hp);
};
