#include "Game.h"
//DXTK headers
#include "SimpleMath.h"

//system headers
#include <windows.h>
#include <time.h>

//our headers
#include "ObjectList.h"
#include "GameData.h"
#include "drawdata.h"
#include "DrawData2D.h"

#include "BoidManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Game::Game(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance) 
{
	//srand(time(NULL));
	
	//set up audio
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audioEngine.reset(new AudioEngine(eflags));

	//Create DirectXTK spritebatch stuff
	ID3D11DeviceContext* pd3dImmediateContext;
	_pd3dDevice->GetImmediateContext(&pd3dImmediateContext);
	m_DD2D = new DrawData2D();
	m_DD2D->m_Sprites.reset(new SpriteBatch(pd3dImmediateContext));
	m_DD2D->m_Font.reset(new SpriteFont(_pd3dDevice, L"..\\Assets\\italic.spritefont"));

	//seed the random number generator
	srand((UINT)time(NULL));

	//Direct Input Stuff
	m_hWnd = _hWnd;
	m_pKeyboard = nullptr;
	m_pDirectInput = nullptr;

	HRESULT hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_pKeyboard->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	hr = m_pMouse->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//create GameData struct and populate its pointers
	m_GD = new GameData;
	m_GD->m_keyboardState = m_keyboardState;
	m_GD->m_prevKeyboardState = m_prevKeyboardState;
	m_GD->m_mouseState = &m_mouseState;

	//set up DirectXTK Effects system
	m_fxFactory = new EffectFactory(_pd3dDevice);

	//Tell the fxFactory to look to the correct build directory to pull stuff in from
#ifdef DEBUG
	((EffectFactory*)m_fxFactory)->SetDirectory(L"../Debug");
#else
	((EffectFactory*)m_fxFactory)->SetDirectory(L"../Release");
#endif

	// Create other render resources here
	m_states = new CommonStates(_pd3dDevice);

	//init render system for VBGOs
	VBGO::Init(_pd3dDevice);

	//find how big my window is to correctly calculate my aspect ratio
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	float AR = (float)width / (float)height;

	screenWidth = width;
	screenHeight = height;

	//create a base light
	m_light = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
	m_GameObjects.push_back(m_light);

	//add Player
	Player* pPlayer = new Player("BirdModelV1.cmo", _pd3dDevice, m_fxFactory);
	m_GameObjects.push_back(pPlayer);

	//create a base camera
	m_freeCam = new FreeCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::UnitY, Vector3(0.0f, 10.0f, 120.0f));
	m_freeCam->SetPos(Vector3(0.0f, 0.0f, 150.0f));
	m_GameObjects.push_back(m_freeCam);

	//add a secondary camera
	m_TPScam = new TPSCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, pPlayer, Vector3::UnitY, Vector3(0.0f, 5.0f, 60.0f));
	m_GameObjects.push_back(m_TPScam);

	//create DrawData struct and populate its pointers
	m_DD = new DrawData;
	m_DD->m_pd3dImmediateContext = nullptr;
	m_DD->m_states = m_states;
	m_DD->m_cam = m_freeCam;
	m_DD->m_light = m_light;

	m_GameObjects.push_back(new Tree(4, 4, .6f, 10.0f *Vector3::Up, XM_PI / 6.0f, "JEMINA vase -up.cmo", _pd3dDevice, m_fxFactory));

	maxBoids = 100;
	m_boidManager = std::make_unique<BoidManager>(_pd3dDevice, maxBoids);
	
	game_state = GameState::GS_MAIN_MENU;
	camera_state = CameraState::FREE_CAMERA;
	current_cam = m_freeCam;
};


Game::~Game() 
{
	//delete Game Data & Draw Data
	delete m_GD;
	delete m_DD;

	//tidy up VBGO render system
	VBGO::CleanUp();

	//tidy away Direct Input Stuff
	if (m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		m_pKeyboard->Release();
	}
	if (m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
	}
	if (m_pDirectInput)
	{
		m_pDirectInput->Release();
	}

	//get rid of the game objects here
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		delete (*it);
	}
	m_GameObjects.clear();


	//and the 2D ones
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		delete (*it);
	}
	m_GameObject2Ds.clear();

	//clear away CMO render system
	delete m_states;
	delete m_fxFactory;

	delete m_DD2D;

};

bool Game::Tick() 
{
	//Poll Keyboard
	readKeyboard();

	// Poll Mouse
	readMouse();

	//tick audio engine
	if (!m_audioEngine->Update())
	{
		// No audio device is active
		if (m_audioEngine->IsCriticalError())
		{
			//something has gone wrong with audio so QUIT!
			return false;
		}
	}

	// GAME STATE CHECKS
	// If the player is on the main menu
	if (game_state == GameState::GS_MAIN_MENU)
	{
		// On pressing escape send signal back to application class to shutdown
		if ((m_keyboardState[DIK_ESCAPE] & 0x80) &&
			!(m_prevKeyboardState[DIK_ESCAPE] & 0x80))
		{
			return false;
		}

		// On pressing escape send signal back to application class to shutdown
		if (m_keyboardState[DIK_RETURN] & 0x80)
		{
			game_state = GameState::GS_PLAY_GAME;
			return true;
		}
	}

	// If the player is 'In Game'
	if (game_state == GameState::GS_PLAY_GAME)
	{
		if ((m_keyboardState[DIK_P] & 0x80) &&
			!(m_prevKeyboardState[DIK_P] & 0x80))
		{
			game_state = GameState::GS_PAUSE;
			return true;
		}

		// Camera Switch
		if ((m_keyboardState[DIK_SPACE] & 0x80) &&
			!(m_prevKeyboardState[DIK_SPACE] & 0x80))
		{
			if (camera_state == CameraState::FREE_CAMERA)
			{
				camera_state = TPS_CAMERA;
				current_cam = m_TPScam;
				return true;
			}

			if (camera_state == CameraState::TPS_CAMERA)
			{
				camera_state = FREE_CAMERA;
				current_cam = m_freeCam;
				return true;
			}
		}

		PlayTick();
	}

	if (game_state == GameState::GS_PAUSE)
	{
		if ((m_keyboardState[DIK_P] & 0x80) &&
			!(m_prevKeyboardState[DIK_P] & 0x80))
		{
			game_state = GameState::GS_PLAY_GAME;
			return true;
		}
		if ((m_keyboardState[DIK_ESCAPE] & 0x80) &&
			!(m_prevKeyboardState[DIK_ESCAPE] & 0x80))
		{
			game_state = GameState::GS_MAIN_MENU;
			//resetGame(); // reset simulation
			return true;
		}
	}

	//lock the cursor to the centre of the window
	RECT window;
	GetWindowRect(m_hWnd, &window);
	SetCursorPos((window.left + window.right) >> 1, (window.bottom + window.top) >> 1);

	//calculate frame time-step dt for passing down to game objects
	DWORD currentTime = GetTickCount();
	m_GD->m_dt = min((float)(currentTime - m_playTime) / 1000.0f, 0.1f);
	m_playTime = currentTime;
	
	return true;
};

void Game::PlayTick()
{
	if ((m_mouseState.rgbButtons[0] & 0x80)/* &&
		!(m_prevMouseState.rgbButtons[0] & 0x80)*/)
	{
		m_boidManager->spawnBoid();
	}

	if ((-m_mouseState.lZ & 0X80))
	{

		m_freeCam->increaseZoom();
	}

	if ((m_mouseState.lZ & 0X80))
	{
		m_freeCam->decreaseZoom();
	}

	//update all objects
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Tick(m_GD);
	}
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		(*it)->Tick(m_GD);
	}

	m_boidManager->Tick(m_GD);
}

void Game::Draw(ID3D11DeviceContext* _pd3dImmediateContext) 
{
	//set immediate context of the graphics device
	m_DD->m_pd3dImmediateContext = _pd3dImmediateContext;

	m_DD->m_cam = current_cam;
	if (camera_state == CameraState::TPS_CAMERA)
	{
		m_DD->m_cam = m_TPScam;
	}

	//update the constant buffer for the rendering of VBGOs
	VBGO::UpdateConstantBuffer(m_DD);

	if (game_state == GameState::GS_MAIN_MENU)
	{
		displayMainMenu();
	}

	if (game_state == GameState::GS_PLAY_GAME)
	{
		// Draw the game objects first, then add HUD over the top

		//draw all objects
		for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
		{
			(*it)->Draw(m_DD);
		}
		// Draw sprite batch stuff 
		m_DD2D->m_Sprites->Begin();
		for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
		{
			(*it)->Draw(m_DD2D);
		}
		m_DD2D->m_Sprites->End();

		m_boidManager->Draw(m_DD);
	}

	if (game_state == GameState::GS_PAUSE)
	{
		displayPauseMenu();
	}

	//drawing text screws up the Depth Stencil State, this puts it back again!
	_pd3dImmediateContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
};



bool Game::readKeyboard()
{
	// Copy over old keyboard state
	memcpy(m_prevKeyboardState, m_keyboardState, sizeof(unsigned char) * 256);

	// clear out previous state
	ZeroMemory(&m_keyboardState, sizeof(m_keyboardState));

	// Read the keyboard device
	HRESULT hr = m_pKeyboard->GetDeviceState(sizeof(m_keyboardState),
		(LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired
		// then try to get control back
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pKeyboard->Acquire();
		}

		else
		{
			return false;
		}
	}
	return true;
}



bool Game::readMouse()
{
	// Set previous mouse state
	m_prevMouseState = m_mouseState;

	// clear out previous state
	ZeroMemory(&m_mouseState, sizeof(m_mouseState));

	// Read the mouse device
	HRESULT hr = m_pMouse->GetDeviceState(sizeof(m_mouseState),
		(LPVOID)&m_mouseState);
	if (FAILED(hr))
	{
		// If the mouse lost focus or was not acquired
		// then try to get control back
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pMouse->Acquire();
		}

		else
		{
			return false;
		}
	}
	return true;
}



void Game::displayMainMenu()
{
	// Draw sprite Batch stuff
	m_DD2D->m_Sprites->Begin();

	int posx = screenWidth / 10 * 1;
	int posy = screenHeight / 10 * 1;

	displayText("Boids Simulation\n 'By Samuel Harden'", posx, posy);

	m_DD2D->m_Sprites->End();
}



void Game::displayPauseMenu()
{
	m_DD2D->m_Sprites->Begin();

	int posx = screenWidth / 10 * 1;
	int posy = screenHeight / 10 * 1;

	displayText("Simulation Paused", posx, posy);

	m_DD2D->m_Sprites->End();
}



// Simple function that displays text, just pass a string and x and y coords
void Game::displayText(const char* input, int& posX, int& posY)
{
	m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar
		(input),
		XMFLOAT2((posY), (posY)),
		Colors::White);
}