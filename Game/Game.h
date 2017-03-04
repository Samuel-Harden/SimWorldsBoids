#ifndef _GAME_H_
#define _GAME_H_

//=================================================================
//Basic Game Manager Class
//=================================================================

#include <windows.h>
#include <list>
#include <dinput.h>
#include "Effects.h"
#include "CommonStates.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "Audio.h"

using std::list;
using std::unique_ptr;
using namespace DirectX;

// Forward declarations
class GameObject;
class GameObject2D;
class Camera;
class FreeCamera;
class TPSCamera;
struct GameData;
struct DrawData;
struct DrawData2D;
class Light;
class BoidManager;

class Game
{
public:

	Game(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance);
	virtual ~Game();

	bool Tick(); //tick the game state

	void Draw(ID3D11DeviceContext* _pd3dImmediateContext); //render the current game state

protected:
	DWORD m_playTime; //amount of time since the game started

	FreeCamera* m_freeCam; //principle camera
	TPSCamera* m_TPScam;//TPS cam
	Camera* current_cam;
	Light* m_light; //base light

	list<GameObject *> m_GameObjects; //data structure storing all GameObjects of this Game
	list<GameObject2D *> m_GameObject2Ds;//ditto 2D objects

	std::unique_ptr<BoidManager> m_boidManager;

	//required for the CMO model rendering system
	CommonStates* m_states;
	IEffectFactory* m_fxFactory;

	//direct input stuff
	bool Game::readKeyboard(); //Get current Keyboard state
	bool Game::readMouse();    //Get current Keyboard state

	IDirectInput8*			m_pDirectInput;
	IDirectInputDevice8*	m_pKeyboard;
	IDirectInputDevice8*	m_pMouse;
	unsigned char			m_keyboardState[256];
	unsigned char			m_prevKeyboardState[256];
	DIMOUSESTATE			m_mouseState;
	DIMOUSESTATE			m_prevMouseState;
	HWND m_hWnd;

	GameData* m_GD;			//Data to be shared to all Game Objects as they are ticked
	DrawData* m_DD;			//Data to be shared to all Game Objects as they are drawn
	DrawData2D * m_DD2D;	//Data to be passed by game to all 2D Game Objects via Draw 

	//sound stuff
	std::unique_ptr<AudioEngine> m_audioEngine;

	//Tick functions for each state
	void PlayTick();

private:
	int maxBoids;

	float screenWidth;
	float screenHeight;

	void displayMainMenu();
	void displayPauseMenu();

	void displayText(const char* input, int& posX, int& posY);
};


#endif