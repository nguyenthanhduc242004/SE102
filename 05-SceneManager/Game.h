#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <unordered_map>
#include <string>
#include <functional>
#include <dinput.h>
#include "Texture.h"
#include "KeyEventHandler.h"
#include "Scene.h"

using namespace std;

#define DIRECTINPUT_VERSION 0x0800

#define MAX_FRAME_RATE 100
#define KEYBOARD_BUFFER_SIZE 1024
#define KEYBOARD_STATE_SIZE 256



/*
	Our simple game framework
*/
enum GameState {
	GAME_RUNNING,
	GAME_PAUSED,
	GAME_OVER,
};

enum FadeState {
	FADE_NONE,
	FADE_OUT,  // going to black
	FADE_WAIT, // black, do something
	FADE_IN    // back to normal
};

class CGame
{
	static CGame* __instance;
	HWND hWnd;									// Window handle

	int backBufferWidth = 0;					// Backbuffer width & height, will be set during Direct3D initialization
	int backBufferHeight = 0;

	ID3D10Device* pD3DDevice = NULL;
	IDXGISwapChain* pSwapChain = NULL;
	ID3D10RenderTargetView* pRenderTargetView = NULL;
	ID3D10BlendState* pBlendStateAlpha = NULL;			// To store alpha blending state

	LPD3DX10SPRITE spriteObject;						// Sprite handling object, BIG MYSTERY: it has to be in this place OR will lead to access violation in D3D11.dll ????

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[KEYBOARD_STATE_SIZE];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	HINSTANCE hInstance;

	ID3D10SamplerState* pPointSamplerState;

	unordered_map<int, LPSCENE> scenes;
	int current_scene = -1;
	int next_scene = -1;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

	GameState currentState;
	int life = 4;
	int score = 0;
	int coin = 0;
	int level = 1;
	BOOLEAN isUpKeyBeingPressed = false;
	float fadeAlpha = 0.0f;
	FadeState fadeState = FADE_NONE;
	float fadeSpeed = 1.3f; // alpha units per second

	function<void()> onFadeComplete = nullptr; // what to do at full black
	LPTEXTURE blackTexture = nullptr;  // At the class level
public:
	// Init DirectX, Sprite Handler
	void Init(HWND hWnd, HINSTANCE hInstance);

	//
	// Draw a portion or ALL the texture at position (x,y) on the screen. (x,y) is at the CENTER of the image
	// rect : if NULL, the whole texture will be drawn
	//        if NOT NULL, only draw that portion of the texture 
	void Draw(float x, float y, LPTEXTURE tex, RECT* rect = NULL, float alpha = 1.0f, int sprite_width = 0, int sprite_height = 0);

	void Draw(float x, float y, LPTEXTURE tex, int l, int t, int r, int b, float alpha = 1.0f, int sprite_width = 0, int sprite_height = 0)
	{
		RECT rect;
		rect.left = l;
		rect.top = t;
		rect.right = r;
		rect.bottom = b;
		this->Draw(x, y, tex, &rect, alpha, sprite_width, sprite_height);
	}

	LPTEXTURE LoadTexture(LPCWSTR texturePath);

	// Keyboard related functions 
	void InitKeyboard();
	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }


	ID3D10Device* GetDirect3DDevice() { return this->pD3DDevice; }
	IDXGISwapChain* GetSwapChain() { return this->pSwapChain; }
	ID3D10RenderTargetView* GetRenderTargetView() { return this->pRenderTargetView; }

	ID3DX10Sprite* GetSpriteHandler() { return this->spriteObject; }

	ID3D10BlendState* GetAlphaBlending() { return pBlendStateAlpha; };

	int GetBackBufferWidth() { return backBufferWidth; }
	int GetBackBufferHeight() { return backBufferHeight; }

	static CGame* GetInstance();

	void SetPointSamplerState();

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }
	void GetCamPos(float& x, float& y) { x = cam_x; y = cam_y; }

	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void Load(LPCWSTR gameFile);
	void SwitchScene();
	void InitiateSwitchScene(int scene_id);
	void ReloadCurrentScene();
	void _ParseSection_TEXTURES(string line);

	// GameStateMachine
	void PauseGame() {
		if (currentState == GAME_RUNNING) {
			currentState = GAME_PAUSED;
		}
	}
	void ResumeGame() {
		currentState = GAME_RUNNING;
	}
	void EndGame() {
		currentState = GAME_OVER;
	}
	GameState GetCurrentGameState() {
		return currentState;
	}
	//

	bool IsInCamera(float x, float y) {
		if (x < cam_x) return false;
		if (x > cam_x + backBufferWidth) return false;
		if (y < cam_y) return false;
		if (y > cam_y + backBufferHeight) return false;
		return true;
	}
	~CGame();

	int GetScore() {
		return score;
	}
	int GetLife() {
		return life;
	}
	int GetCoin() {
		return coin;
	}
	int GetLevel() {
		return level;
	}
	void SetScore(int score) {
		this->score = score;
	}
	void SetLife(int life) {
		this->life = life;
	}
	void SetCoin(int coin) {
		this->coin = coin;
	}
	void SetLevel(int level) {
		this->level = level;
	}
	void SetUpKeyBeingPressed(BOOLEAN isUpKeyBeingPressed) { this->isUpKeyBeingPressed = isUpKeyBeingPressed; }
	BOOLEAN GetUpKeyBeingPressed() { return this->isUpKeyBeingPressed; }

	void StartFadeTransition(std::function<void()> onComplete);
	void UpdateFade(DWORD dt);
	void RenderFadeOverlay();
};
typedef CGame* LPGAME;

