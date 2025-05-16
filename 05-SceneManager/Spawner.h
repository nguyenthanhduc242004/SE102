#pragma once
#include "PlayScene.h"

class CSpawner : public CGameObject
{
public:
	CGameObject* obj = NULL;
private:
	bool hasSpawned;
	bool isDead = false;
	int NPC_ID;
	int token3;
	int token4;
	int token5;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() {
		//RenderBoundingBox();
	}

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

	void Spawn();
public:
	CSpawner(float x, float y, int NPC_ID) : CGameObject(x, y) {
		hasSpawned = false;
		this->NPC_ID = NPC_ID;
	}
	CSpawner(float x, float y, int NPC_ID, int token3) : CSpawner(x, y, NPC_ID) {
		this->token3 = token3;
	}
	CSpawner(float x, float y, int NPC_ID, int token3, int token4) : CSpawner(x, y, NPC_ID, token3) {
		this->token4 = token4;
	}
	CSpawner(float x, float y, int NPC_ID, int token3, int token4, int token5) : CSpawner(x, y, NPC_ID, token3, token4) {
		this->token5 = token5;
	}

	void SetDead(bool isDead) { this->isDead = isDead; }
};