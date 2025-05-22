#pragma once
#include "PlayScene.h"
#include "Font.h"

#define HUD_PANEL_SPRITE_ID     777777    
#define HUD_P_ANIM_ID           5006
#define HUD_ARROW_SPRITE_ID     50005    
#define HUD_DIGIT_WIDTH         8

#define HUD_CAMERA_RELATIVE_X                   0
#define HUD_CAMERA_RELATIVE_Y                   -16

#define HUD_PANEL_OFFSET_X        -136 + 9
#define HUD_PANEL_OFFSET_Y      -3
#define HUD_OFFSET_TIME_X       132
#define HUD_OFFSET_SCORE_X      60
#define HUD_OFFSET_COIN_X       140
#define HUD_OFFSET_LIFE_X       36
#define HUD_OFFSET_P_X          112
#define HUD_ROW1_Y              0
#define HUD_ROW2_Y              8

class CMario;
class CHUD : public CGameObject {
    int life, coin, score;
    int remainTime;
    int runStacks;

    CFont* font;
    LPSPRITE panelSprite;
    LPSPRITE arrowSprite;
    LPANIMATION pAnim;

    vector<LPSPRITE> timeSprites;
    vector<LPSPRITE> scoreSprites;
    vector<LPSPRITE> coinSprites;
    vector<LPSPRITE> lifeSprites;

public:
    CHUD();
    void Render();
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
    void Update(DWORD dt, CMario* mario);
    int IsBlocking() { return 0; }


	void Clear() {
		for (size_t i = 0; i < lifeSprites.size(); i++)
			delete lifeSprites[i];
		lifeSprites.clear();
		for (size_t i = 0; i < coinSprites.size(); i++)
			delete coinSprites[i];
		coinSprites.clear();
		for (size_t i = 0; i < scoreSprites.size(); i++)
			delete scoreSprites[i];
		scoreSprites.clear();
		for (size_t i = 0; i < timeSprites.size(); i++)
			delete timeSprites[i];
        timeSprites.clear();
	}

    void SetRemainTime(int remainTime) {
        this->remainTime = remainTime;
    }
};
