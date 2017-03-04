#include "Player.h"
#include <dinput.h>
#include "GameData.h"

Player::Player(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	//any special set up for Player goes here
	m_fudge = Matrix::CreateRotationY(XM_PI);

	m_pos.y = 10.0f;

	SetDrag(0.7);
	SetPhysicsOn(true);
}

Player::~Player()
{
	//tidy up anything I've created
}


void Player::Tick(GameData* _GD)
{

	//TURN AND FORWARD CONTROL HERE
	Vector3 forwardMove = 20.0f * Vector3::Forward;
	Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	forwardMove = Vector3::Transform(forwardMove, rotMove);

	if (_GD->m_keyboardState[DIK_W] & 0x80)
	{
		m_acc += forwardMove;
	}
	if (_GD->m_keyboardState[DIK_S] & 0x80)
	{
		m_acc -= forwardMove;
	}

	//change orinetation of player
	float rotSpeed = 2.0f * _GD->m_dt;
	if (_GD->m_keyboardState[DIK_A] & 0x80)
	{
		m_yaw += rotSpeed;
	}
	if (_GD->m_keyboardState[DIK_D] & 0x80)
	{
		m_yaw -= rotSpeed;
	}

	//move player up and down
	if (_GD->m_keyboardState[DIK_R] & 0x80)
	{
		m_acc.y += 20.0f;
	}

	if (_GD->m_keyboardState[DIK_F] & 0x80)
	{
		m_acc.y -= 20.0f;
	}

	//limit motion of the player
	float length = m_pos.Length();
	float maxLength = 500.0f;
	if (length > maxLength)
	{
		m_pos.Normalize();
		m_pos *= maxLength;
		m_vel *= -0.9; //VERY simple bounce back
	}

	// Set players pos as that of the predator
	_GD->predatorPos = m_pos;

	//apply my base behaviour
	CMOGO::Tick(_GD);
}