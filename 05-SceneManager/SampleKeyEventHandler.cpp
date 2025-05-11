#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = (CMario *)((LPPLAYSCENE)game->GetCurrentScene())->GetPlayer();
	if (KeyCode == DIK_R) {
		game->ReloadCurrentScene();
		game->ResumeGame();
		return;
	}
	if (KeyCode == DIK_ESCAPE) {
		if (game->GetCurrentGameState() == GAME_RUNNING) {
			game->PauseGame();
		}
		else if (game->GetCurrentGameState() == GAME_PAUSED) {
			game->ResumeGame();
		}
	}
	if (game->GetCurrentGameState() != GAME_RUNNING) return;
	switch (KeyCode)
	{
	case DIK_A:
		mario->SetIsReadyToHold(true);
		mario->Attack();
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_TANOOKI);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)game->GetCurrentScene())->GetPlayer();

	if (game->GetCurrentGameState() != GAME_RUNNING) return;
	switch (KeyCode)
	{
	case DIK_A:
		mario->SetIsReadyToHold(false);
		mario->SetIsHolding(false);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;

	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)game->GetCurrentScene())->GetPlayer();

	if (game->GetCurrentGameState() != GAME_RUNNING) return;
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}