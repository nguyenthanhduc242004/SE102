#include "HUD.h"


void CHUD::Update(DWORD dt, CMario* mario) {
	if (!mario) return;

	// update raw stats
	life = mario->GetLife();
	score = mario->GetScore();
	coin = mario->GetCoin();

	timer.Tick(dt);
	remainTime = (int)(ceil(DEFAULT_PLAYTIME - (timer.getAccumulated() / 1000)));

	// running meter stacks (0–MARIO_RUNNING_STACKS)
	runStacks = mario->GetSpeedStacks();

	// build digit sprite sequences
	auto toSprites = [&](int value, int width) {
		string s = to_string(value);
		while (s.size() < width) s = "0" + s;
		return font->StringToSprite(s);
		};

	timeSprites = toSprites(remainTime, 3);
	scoreSprites = toSprites(score, 6);
	coinSprites = toSprites(coin, 2);
	lifeSprites = toSprites(life, 2);
}

CHUD::CHUD() : CGameObject()
{
	// grab panel + arrow + P-anim from your sprite/anim manager:
	panelSprite = CSprites::GetInstance()->Get(HUD_PANEL_SPRITE_ID);
	arrowSprite = CSprites::GetInstance()->Get(HUD_ARROW_SPRITE_ID);
	pAnim = CAnimations::GetInstance()->Get(HUD_P_ANIM_ID);

	// initial values (or load from saved state)
	timer.Reset();                  // zero it
	timer.Start();                  // begin ticking
	remainTime = DEFAULT_PLAYTIME;
	runStacks = 0;

	// init font mapping
	font = new CFont();
}

void CHUD::Render() {
	// draw panel
	panelSprite->Draw(x + HUD_CAMERA_RELATIVE_X
		, y + HUD_CAMERA_RELATIVE_Y);
	// draw digits row2 (time + score + lives)
	for (int i = 0; i < timeSprites.size(); i++)
		timeSprites[i]->Draw(x + HUD_CAMERA_RELATIVE_X + HUD_OFFSET_TIME_X + i * HUD_DIGIT_WIDTH + HUD_PANEL_OFFSET_X,
			y + HUD_CAMERA_RELATIVE_Y + HUD_ROW2_Y + HUD_PANEL_OFFSET_Y);
	for (int i = 0; i < scoreSprites.size(); i++)
		scoreSprites[i]->Draw(x + HUD_CAMERA_RELATIVE_X + HUD_OFFSET_SCORE_X + i * HUD_DIGIT_WIDTH + HUD_PANEL_OFFSET_X,
			y + HUD_CAMERA_RELATIVE_Y + HUD_ROW2_Y + HUD_PANEL_OFFSET_Y);
	for (int i = 0; i < lifeSprites.size(); i++)
		lifeSprites[i]->Draw(x + HUD_CAMERA_RELATIVE_X + HUD_OFFSET_LIFE_X + i * HUD_DIGIT_WIDTH + HUD_PANEL_OFFSET_X,
			y + HUD_CAMERA_RELATIVE_Y + HUD_ROW2_Y + HUD_PANEL_OFFSET_Y);

	// draw coins in row1
	for (int i = 0; i < coinSprites.size(); i++)
		coinSprites[i]->Draw(x + HUD_CAMERA_RELATIVE_X + HUD_OFFSET_COIN_X + i * HUD_DIGIT_WIDTH + HUD_PANEL_OFFSET_X,
			y + HUD_CAMERA_RELATIVE_Y + HUD_ROW1_Y + HUD_PANEL_OFFSET_Y);

	// draw run-meter (arrows + “P”)
	for (int i = 0; i < runStacks; i++) {
			arrowSprite->Draw(x + HUD_CAMERA_RELATIVE_X + HUD_OFFSET_SCORE_X + i * HUD_DIGIT_WIDTH + HUD_PANEL_OFFSET_X,
				y + HUD_CAMERA_RELATIVE_Y + HUD_ROW1_Y + HUD_PANEL_OFFSET_Y);
			if (i == MARIO_MAX_SPEED_STACKS - 1)
			pAnim->Render(x + HUD_CAMERA_RELATIVE_X + HUD_OFFSET_P_X + HUD_PANEL_OFFSET_X,
				y + HUD_CAMERA_RELATIVE_Y + HUD_ROW1_Y + HUD_PANEL_OFFSET_Y);
	}
}
