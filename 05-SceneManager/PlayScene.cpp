#include <iostream>
#include <fstream>
#include "AssetIDs.h"
#include "PlayScene.h"

#include "Game.h"
#include <typeinfo>

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	hud = NULL;
	remainTime = 0;
	key_handler = new CSampleKeyHandler(this);
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}
void CPlayScene::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);
	//skip invalid settings, need one for what setting and the other for value
	if (tokens.size() < 2) return;
	int setting = atoi(tokens[0].c_str());
	int value = atoi(tokens[1].c_str());
	switch (setting)
	{
	case SCENE_SETTING_INDEPENDENT_CAM:	//cam_indie
		isCameraIndependent = value;
		break;
	case SCENE_SETTING_PLAYTIME:	//playtime
		playSceneTime = value;
		break;
	case SCENE_SETTING_CAMERA_LEFT_BOUND:	//start_x
		camLeftBound = value;
		break;
	case SCENE_SETTING_LOWER_DEATH_BOUND:	//lower_death_bound
		lowerDeathBound = value;
		break;
	case SCENE_SETTING_CAMERA_RIGHT_BOUND:	//interval_x (if mario can go to secret scene) or end_x
		camRightBounds.push_back(value);
		break;
	}
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());


	//CGameObject *obj = NULL;
	vector<LPGAMEOBJECT> objs;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		player = new CMario(x, y);
		//objs.push_back(player);
		//DebugOut(L"X: %f", x);
		//DebugOut(L" Y: %f\n", y);
		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	}
	case OBJECT_TYPE_GOOMBA: {
		int color = (int)atof(tokens[3].c_str());
		boolean isParagoomba = (boolean)atof(tokens[4].c_str());
		LPSPAWNER spawner = new CSpawner(x, y, OBJECT_TYPE_GOOMBA, color, isParagoomba);
		objs.push_back(spawner);
		spawners.push_back(spawner);
		//objs.push_back(new CBoBro(x, y - 5));
		break;
	}

	case OBJECT_TYPE_BO_BRO: {
		objs.push_back(new CBoBro(x, y));
		break;
	}

	case OBJECT_TYPE_BACKGROUND_TREE: {
		int cellWidth = 16;
		int cellHeight = 16;
		int color = atoi(tokens[3].c_str());	// 0: transparent; 1: green
		int length = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());

		int id = -1;
		int spriteId = -1;

		// 1: headLeft; 2: headRight
		// 3: bodyLeft; 4: bodyMiddle; 5:bodyRight
		for (int i = 1; i <= height; i++) {
			for (int j = 1; j <= length; j++) {
				id = atoi(tokens[5 + (i - 1) * length + j].c_str());
				switch (id) {
				case 1:
					if (color == 0) {
						spriteId = SPRITE_ID_BACKGROUND_GREEN_TREE_HEAD_LEFT;
					}
					else if (color == 1) {
						spriteId = SPRITE_ID_BACKGROUND_TRANSPARENT_TREE_HEAD_LEFT;
					}
					break;
				case 2:
					if (color == 0) {
						spriteId = SPRITE_ID_BACKGROUND_GREEN_TREE_HEAD_RIGHT;
					}
					else if (color == 1) {
						spriteId = SPRITE_ID_BACKGROUND_TRANSPARENT_TREE_HEAD_RIGHT;
					}
					break;
				case 3:
					if (color == 0) {
						spriteId = SPRITE_ID_BACKGROUND_GREEN_TREE_BODY_LEFT;
					}
					else if (color == 1) {
						spriteId = SPRITE_ID_BACKGROUND_TRANSPARENT_TREE_BODY_LEFT;
					}
					break;
				case 4:
					if (color == 0) {
						spriteId = SPRITE_ID_BACKGROUND_GREEN_TREE_BODY_MIDDLE;
					}
					else if (color == 1) {
						spriteId = SPRITE_ID_BACKGROUND_TRANSPARENT_TREE_BODY_MIDDLE;
					}
					break;
				case 5:
					if (color == 0) {
						spriteId = SPRITE_ID_BACKGROUND_GREEN_TREE_BODY_RIGHT;
					}
					else if (color == 1) {
						spriteId = SPRITE_ID_BACKGROUND_TRANSPARENT_TREE_BODY_RIGHT;
					}
					break;
				case 6:
					if (color == 0) {
						spriteId = SPRITE_ID_BACKGROUND_GREEN_TREE_HEAD_LEFT_ABOVE_BODY_MIDDLE;
					}
					else if (color == 1) {
						spriteId = SPRITE_ID_BACKGROUND_TRANSPARENT_TREE_HEAD_LEFT_ABOVE_BODY_MIDDLE;
					}
					break;
				case 7:
					if (color == 0) {
						spriteId = SPRITE_ID_BACKGROUND_GREEN_TREE_HEAD_RIGHT_ABOVE_BODY_MIDDLE;
					}
					else if (color == 1) {
						spriteId = SPRITE_ID_BACKGROUND_TRANSPARENT_TREE_HEAD_RIGHT_ABOVE_BODY_MIDDLE;
					}
					break;
				}

				if (spriteId != -1) {
					objs.push_back(new CBackground(
						x + (j - 1) * cellWidth, y - (i - 1) * cellHeight, cellWidth, cellHeight, 1, 1,
						spriteId, spriteId, spriteId
					));
					spriteId = -1;
				}
			}
		}
		break;
	}

	case OBJECT_TYPE_KOOPA:
	{
		int color = (int)atof(tokens[3].c_str());
		int type = (int)atof(tokens[4].c_str());
		LPSPAWNER spawner = new CSpawner(x, y, OBJECT_TYPE_KOOPA, color, type);
		objs.push_back(spawner);
		spawners.push_back(spawner);
		break;
	}
	case OBJECT_TYPE_BRICK:
	{
		// number of iterations into a maze
		int itX = (int)atof(tokens[3].c_str());
		int itY = (int)atof(tokens[4].c_str());

		// distance between objects following directX top-left axis
		int offsetX = (int)atof(tokens[5].c_str());
		int offsetY = (int)atof(tokens[6].c_str());
		for (int i = 0; i < itX; i++) {
			for (int j = 0; j < itY; j++) {
				objs.push_back(new CBrick(x + i * offsetX, y + j * offsetY));
			}
		}
		break;
	}
	case OBJECT_TYPE_COIN:
	{
		// number of iterations into a maze
		int itX = (int)atof(tokens[3].c_str());
		int itY = (int)atof(tokens[4].c_str());

		// distance between objects following directX top-left axis
		int offsetX = (int)atof(tokens[5].c_str());
		int offsetY = (int)atof(tokens[6].c_str());
		for (int i = 0; i < itX; i++) {
			for (int j = 0; j < itY; j++) {
				objs.push_back(new CCoin(x + i * offsetX, y + j * offsetY));
			}
		}
		break;
	}
	case OBJECT_TYPE_QUESTION_BLOCK:
	{
		// number of iterations into a maze
		int itX = (int)atof(tokens[3].c_str());
		int itY = (int)atof(tokens[4].c_str());

		// distance between objects following directX top-left axis
		int offsetX = (int)atof(tokens[5].c_str());
		int offsetY = (int)atof(tokens[6].c_str());

		//0: coin, 3:mushroom red, 5:mushroom green
		int itemID = (int)atof(tokens[7].c_str());
		for (int i = 0; i < itX; i++) {
			for (int j = 0; j < itY; j++) {
				objs.push_back(new CQuestionBlock(x + i * offsetX, y + j * offsetY, itemID));
			}
		}
		break;
	}
	case OBJECT_TYPE_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		objs.push_back(new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		));

		break;
	}
	case OBJECT_TYPE_SIDE_COLLIDABLE_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		objs.push_back(new CSideCollidablePlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		));

		break;
	}
	case OBJECT_TYPE_BLOCK_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		objectsAfterMario.push_back(new CBlockPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		));

		break;
	}
	case OBJECT_TYPE_BACKGROUND:
	{
		//std::cout << "Reading Background!";
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int height = atoi(tokens[6].c_str());
		int sprite_id_begin = atoi(tokens[7].c_str());
		int sprite_id_middle = atoi(tokens[8].c_str());
		int sprite_id_end = atoi(tokens[9].c_str());

		objs.push_back(new CBackground(
			x, y,
			cell_width, cell_height, length, height,
			sprite_id_begin, sprite_id_middle, sprite_id_end
		));

		break;
	}
	case OBJECT_TYPE_BACKGROUND_CLOUD:
	{
		int length = atoi(tokens[3].c_str());

		objs.push_back(new CBackgroundCloud(x, y, length));

		break;
	}
	case OBJECT_TYPE_BOX:
	{
		int length_width = atoi(tokens[3].c_str());
		int length_height = atoi(tokens[4].c_str());
		int color = atoi(tokens[5].c_str());

		objs.push_back(new CBox(
			x, y,
			length_width, length_height,
			color
		));

		break;
	}
	case OBJECT_TYPE_PIPE:
	{
		int height = atoi(tokens[3].c_str());
		int color = atoi(tokens[4].c_str());
		boolean isUpsideDown = atoi(tokens[5].c_str());
		float toX = atoi(tokens[6].c_str());
		float toY = atoi(tokens[7].c_str());
		boolean isAscendingAfter = atoi(tokens[8].c_str());

		objectsAfterMario.push_back(new CPipe(x, y, height, color, isUpsideDown, toX, toY, isAscendingAfter));

		break;
	}
	case OBJECT_TYPE_PIRANHA_PLANT:
	{
		int stem_height = atoi(tokens[3].c_str());
		int type = atoi(tokens[4].c_str());

		objs.push_back(new CPiranhaPlant(x, y, stem_height, type));
		break;
	}
	case OBJECT_TYPE_LIFT:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		objs.push_back(new CLift(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		));


		break;
	}
	case OBJECT_TYPE_ITEM_BRICK: {
		// number of iterations into a maze
		int itX = (int)atof(tokens[3].c_str());
		int itY = (int)atof(tokens[4].c_str());

		// distance between objects following directX top-left axis
		int offsetX = (int)atof(tokens[5].c_str());
		int offsetY = (int)atof(tokens[6].c_str());

		//0: coin, 3:mushroom red, 5:mushroom green
		int itemID = (int)atof(tokens[7].c_str());
		int hp = (int)atof(tokens[8].c_str());
		for (int i = 0; i < itX; i++) {
			for (int j = 0; j < itY; j++) {
				objs.push_back(new CItemBrick(x + i * offsetX, y + j * offsetY, itemID, hp));
			}
		}
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		objs.push_back(new CPortal(x, y, r, b, scene_id));
		break;
	}
	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup

	for (LPGAMEOBJECT obj : objs) {
		objects.push_back(obj);
	}
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::AddObject(LPGAMEOBJECT object) {
	this->objects.push_back(object);
}

void CPlayScene::RemoveObject(LPGAMEOBJECT object) {
	auto it = std::find(objects.begin(), objects.end(), object);
	if (it != objects.end()) {
		objects.erase(it);
	}
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[SETTINGS]") { section = SCENE_SECTION_SETTINGS; continue; }
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	hud = new CHUD();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
	CGame::GetInstance()->ResumeGame();

	remainTime = playSceneTime;
	playSceneTimer.Start();
	//load the scene/ reload the scene->reset timer;
	// player is saved to push at the end, no hiding behind background
	objects.push_back(player);
	for (LPGAMEOBJECT obj : objectsAfterMario) {
		objects.push_back(obj);
	}
	objectsAfterMario.clear();
}

void CPlayScene::Update(DWORD dt)
{
	if (CGame::GetInstance()->GetCurrentGameState() == GAME_OVER)
		return;
	float x, y;
	// We know that Mario is the first object in the list hence we won't add him into the collidable object list
	// TO-DO: This is a "dirty" way, need a more organized way
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		//Conclusion contradicting the statement above: the first object is not mario, since before, Mario was at the start of the txt, but now, Mario is only paste in the playscene after the background to be visible.
		//Currently, Mario is in the index 133!
		objects[i]->GetPosition(x, y);
		if (y >= lowerDeathBound) {
			objects[i]->Delete();
			continue;
		}
		if (CGame::GetInstance()->GetCurrentGameState() == GAME_PAUSED && typeid(*objects[i]) != typeid(CFlyingScore))
			continue;
		coObjects.push_back(objects[i]);
	}


	CMario* mario = dynamic_cast<CMario*>(player);
	{
		if (playSceneTimer.IsRunning() && CGame::GetInstance()->GetCurrentGameState() != GAME_PAUSED) {
			playSceneTimer.Tick(dt);
			remainTime = (int)(ceil(playSceneTime - (playSceneTimer.getAccumulated() / 1000)));
		}
		hud->SetRemainTime(remainTime);
		if (remainTime < 0) {
			remainTime = 0;
			playSceneTimer.Reset();
			mario->SetState(MARIO_STATE_DIE);
		}
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (CGame::GetInstance()->GetCurrentGameState() == GAME_PAUSED) {
			if (typeid(*objects[i]) == typeid(CFlyingScore)) {
				objects[i]->Update(dt, &coObjects);
			}
			if (typeid(*objects[i]) == typeid(CMario) && player->GetState() == MARIO_STATE_DIE) {
				CDeltaTimer* marioDieTimer = mario->GetDieTimer();
				if (marioDieTimer->IsRunning()) {
					marioDieTimer->Tick(dt);
				}
				if (marioDieTimer->HasPassed(MARIO_DIE_TIME)) {
					objects[i]->Update(dt, &coObjects);
				}
			}
		}
		else {
			objects[i]->Update(dt, &coObjects);
		}
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;

	if (cx < camLeftBound) cx = camLeftBound;
	for (float camRightBound : camRightBounds)
	{
		if (cx > camRightBound && cx < camRightBound + game->GetBackBufferWidth()) {
			cx = camRightBound;
			if (isCameraIndependent) {
				isCameraIndependent = false;
				camLeftBound = cx;
			}
			break;
		}
	}

	if (isCameraIndependent || cy > DEFAULT_CAM_Y ||
		!((dynamic_cast<CMario*>(GetPlayer())->HasFlied() && cy < DEFAULT_CAM_Y) || cy < DEFAULT_CAM_Y - game->GetBackBufferHeight() / 2 - 48)) {
		cy = DEFAULT_CAM_Y;

	}

	if (isCameraIndependent) cx = camLeftBound + playSceneTimer.getAccumulated() * CAMERA_MOVE_X_PER_MS;

	game->SetCamPos(cx, cy);
	float ml, mt, mr, mb, mx, my;
	player->GetBoundingBox(ml, mt, mr, mb);
	player->GetPosition(mx, my);

	float screenLeft = cx;
	float screenRight = cx + game->GetBackBufferWidth();

	// Clamp Mario's position to stay within screen
	if (ml < screenLeft) {
		mx += screenLeft - ml; // Move right so left edge is at screen left
	}
	else if (mr > screenRight) {
		mx -= mr - screenRight; // Move left so right edge is at screen right
	}
	player->SetPosition(mx, my);

	// Set its gameobject position to the camera position (aligning x--->center, y--->bottom) then in the hud itself, tweak camera_relative x/y to draw
	hud->SetPosition(cx + (game->GetBackBufferWidth() / 2), cy + (game->GetBackBufferHeight()));
	hud->Update(dt, mario);

	// Mario transform logic
	CDeltaTimer* marioTrasnformTimer = mario->GetTrasnformTimer();
	if (marioTrasnformTimer->IsRunning()) {
		marioTrasnformTimer->Tick(dt);
	}
	if (marioTrasnformTimer->HasPassed(MARIO_TRANSFORM_TIME)) {
		marioTrasnformTimer->Reset();
		CGame::GetInstance()->ResumeGame();
		mario->SetTransforming(false);

	}

	// Mario resize logic
	CDeltaTimer* marioResizeTimer = mario->GetResizeTimer();
	if (marioResizeTimer->IsRunning()) {
		marioResizeTimer->Tick(dt);
	}
	if (marioResizeTimer->HasPassed(MARIO_RESIZING_TIME)) {
		marioResizeTimer->Reset();
		CGame::GetInstance()->ResumeGame();
		mario->SetResizing(false);

	}

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
	hud->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	CGame::GetInstance()->PauseGame();
	for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
	camRightBounds.clear();
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}