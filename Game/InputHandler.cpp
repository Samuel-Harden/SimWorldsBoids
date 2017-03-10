#include "InputHandler.h"

#include "BoidManager.h"
#include "FreeCamera.h"
#include "TPSCamera.h"

#include "GameData.h"



InputHandler::InputHandler(HWND& _hWnd, HINSTANCE& _hInstance)
{
	m_pKeyboard = nullptr;
	m_pDirectInput = nullptr;

	HRESULT hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_pKeyboard->SetCooperativeLevel(_hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	hr = m_pMouse->SetCooperativeLevel(_hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}



InputHandler::~InputHandler()
{
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
}



unsigned char& InputHandler::getKeyboardState()
{
	return m_keyboardState[256];
}



unsigned char& InputHandler::getPreviousKeyboardState()
{
	return m_prevKeyboardState[256];
}



DIMOUSESTATE& InputHandler::getMouseState()
{
	return m_mouseState;
}



// Big ugly function handles inputs and checks states...
bool InputHandler::Tick(GameData* _GD, BoidManager* _boidManager, 
	FreeCamera* _freeCam, TPSCamera* _tpsCam)
{
	// GAME STATE CHECKS
	// If the player is on the main menu
	if (game_state == GameState::GS_MAIN_MENU)
	{
		// On pressing escape send signal back to application class to shutdown
		if ((m_keyboardState[DIK_ESCAPE] & 0x80) &&
			!(m_prevKeyboardState[DIK_ESCAPE] & 0x80))
		{
			// return false to exit the application
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
				_GD->m_currentCam = _tpsCam;
				ShowCursor(false);

				return true;
			}

			if (camera_state == CameraState::TPS_CAMERA)
			{
				camera_state = FREE_CAMERA;
				_GD->m_currentCam = _freeCam;
				ShowCursor(true);
				return true;
			}
		}
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
			_boidManager->resetPreyBoids();
			return true;
		}
	}

	return true;
}



void InputHandler::playTick(GameData* _GD, BoidManager* _boidManager,
	FreeCamera* _freeCam)
{
	
	if ((m_mouseState.rgbButtons[0] & 0x80)/* &&
		!(m_prevMouseState.rgbButtons[0] & 0x80)*/)
	{
		_boidManager->spawnBoid();
	}

	if ((m_mouseState.rgbButtons[1] & 0x80))
	{
		_freeCam->allowRotation(_GD);
	}

	if ((-m_mouseState.lZ & 0X80))
	{
		_freeCam->increaseZoom();
	}

	if ((m_mouseState.lZ & 0X80))
	{
		_freeCam->decreaseZoom();
	}
}



bool InputHandler::readKeyboard()
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



bool InputHandler::readMouse()
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