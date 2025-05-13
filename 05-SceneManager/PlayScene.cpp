#include <iostream>
#include <fstream>
#include "AssetIDs.h"
#include "PlayScene.h"


using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
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
		objs.push_back(player);

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	}
	case OBJECT_TYPE_GOOMBA: {
		int color = (int)atof(tokens[3].c_str());
		boolean isParagoomba = (boolean)atof(tokens[4].c_str());
		LPSPAWNER spawner = new CSpawner(x, y, OBJECT_TYPE_GOOMBA, color, isParagoomba);
		objs.push_back(spawner);
		spawners.push_back(spawner);
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

		objs.push_back(new CBlockPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		));

		break;
	}
	case OBJECT_TYPE_BACKGROUND:
	{
		std::cout << "Reading Background!";
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_id_begin = atoi(tokens[6].c_str());
		int sprite_id_middle = atoi(tokens[7].c_str());
		int sprite_id_end = atoi(tokens[8].c_str());

		objs.push_back(new CBackground(
			x, y,
			cell_width, cell_height, length,
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

		objs.push_back(new CPipe(x, y, height));

		break;
	}
	case OBJECT_TYPE_PIRANHA_PLANT:
	{
		int stem_height = atoi(tokens[3].c_str());
		int type = atoi(tokens[4].c_str());

		objs.push_back(new CPiranhaPlant(x, y, stem_height, type));
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		objs.push_back(new CPortal(x, y, r, b, scene_id));
	}
	break;
	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	//obj->SetPosition(x, y);

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
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	/*if (CGame::GetInstance()->GetCurrentGameState() == GAME_PAUSED)
		return;*/

	float x, y;

	// We know that Mario is the first object in the list hence we won't add him into the collidable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		objects[i]->GetPosition(x, y);
		if (CGame::GetInstance()->GetCurrentGameState() == GAME_PAUSED && typeid(*objects[i]) != typeid(CFlyingScore))
			continue;
		if (y >= LOWER_BOUND_DEATHZONE) {
			objects[i]->Delete();
			continue;
		}
		coObjects.push_back(objects[i]);
	}


	CMario* mario = dynamic_cast<CMario*>(player);

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

	if (cx < LEFT_BOUND_CAM_X) cx = LEFT_BOUND_CAM_X;

	CGame::GetInstance()->SetCamPos(cx, DEFAULT_CAM_Y /*cy*/);

	// Mario transform logic
	CDeltaTimer* marioTransformTimer = mario->GetTransformTimer();
	if (marioTransformTimer->IsRunning()) {
		marioTransformTimer->Tick(dt);
	}
	if (marioTransformTimer->HasPassed(MARIO_RESIZING_TIME)) {
		marioTransformTimer->Reset();
		CGame::GetInstance()->ResumeGame();
		mario->SetTransforming(false);
		
	}

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
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
	for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
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