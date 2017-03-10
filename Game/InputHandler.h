#pragma once

#include <dinput.h>

class BoidManager;
class FreeCamera;
class TPSCamera;
struct GameData;

class InputHandler
{
public:
	InputHandler(HWND& _hWnd, HINSTANCE& _hInstance);
	~InputHandler();

	bool readKeyboard(); //Get current Keyboard state
	bool readMouse();    //Get current Keyboard state

	unsigned char& getKeyboardState();
	unsigned char& getPreviousKeyboardState();
	DIMOUSESTATE& getMouseState();

	bool Tick(GameData* _GD, BoidManager* _boidManager, FreeCamera* _freeCam,
		TPSCamera* _tpsCam);

	void playTick(GameData* _GD, BoidManager* _boidManager, FreeCamera* _freeCam);

protected:

	IDirectInput8*			m_pDirectInput;
	IDirectInputDevice8*	m_pKeyboard;
	IDirectInputDevice8*	m_pMouse;
	unsigned char			m_keyboardState[256];
	unsigned char			m_prevKeyboardState[256];
	DIMOUSESTATE			m_mouseState;
	DIMOUSESTATE			m_prevMouseState;

private:

};