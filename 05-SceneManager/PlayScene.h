#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Utils.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Background.h"
#include "BackgroundCloud.h"
#include "Box.h"
#include "SideCollidablePlatform.h"
#include "BlockPlatform.h"
#include "QuestionBlock.h"
#include "Mushroom.h"
#include "Pipe.h"
#include "Koopa.h"
#include "PiranhaPlant.h"
#include "Bullet.h"
#include "SampleKeyEventHandler.h"
#include "Spawner.h"
#include "HUD.h"
#include "BoBro.h"
#include "Lift.h"
#include "ItemBrick.h"
#include "BackgroundTree.h"

#define SCENE_SETTING_INDEPENDENT_CAM	1
#define SCENE_SETTING_PLAYTIME			2
#define SCENE_SETTING_CAMERA_LEFT_BOUND 3
#define	SCENE_SETTING_LOWER_DEATH_BOUND	4


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_SETTINGS  0
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

#define DEFAULT_CAM_Y 260.0f	
#define LEFT_BOUND_CAM_X 93		
#define RESPAWN_OFFSET_CAM_X 70
#define LOWER_BOUND_DEATHZONE 700.0f		//this is fixed
#define DEFAULT_PLAYTIME        300	
#define CAMERA_MOVE_X_PER_MS	0.02f

class CHUD;
class CSpawner;
typedef CSpawner* LPSPAWNER;

class CPlayScene : public CScene
{
protected:
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;

	vector<LPGAMEOBJECT> objects;
	vector<LPSPAWNER> spawners;
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);

	CDeltaTimer playSceneTimer;
	int remainTime;
	int playSceneTime = DEFAULT_PLAYTIME; // second, when use timer (Tick)--->multiply by 1000. for now, not used, just go by the default
	bool isCameraIndependent = false;	//also know as isSceneAthletic, since the scene will move constantly
	float camLeftBound = LEFT_BOUND_CAM_X;
	float lowerDeathBound = LOWER_BOUND_DEATHZONE;
	CHUD* hud;
public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void AddObject(LPGAMEOBJECT object);
	virtual void RemoveObject(LPGAMEOBJECT object);


	LPGAMEOBJECT GetPlayer() { return player; }
	vector<LPSPAWNER> GetSpawners() { return spawners; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

