#include "FreeCamera.h"
#include "GameData.h"

FreeCamera::FreeCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, GameObject* _target, Vector3 _up, Vector3 _dpos)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _up)
{
	m_targetObject = _target;
	m_dpos = _dpos;
}

FreeCamera::~FreeCamera()
{

}

void FreeCamera::Tick(GameData* _GD)
{
	// Set up position  of camera and target position of camera based on new
	// position and orientation of target object

	Matrix rotCam = Matrix::CreateFromYawPitchRoll(getYaw(), getPitch(), 0.0f);

	// Sets the positon of the camera
	m_pos = m_targetObject->getPos() + Vector3::Transform(m_dpos, rotCam);

	// Sets what the camera is looking at
	m_target = m_targetObject->getPos();

	// Then set up proj and view matrices
	Camera::Tick(_GD);
}



void FreeCamera::increaseZoom()
{
	m_dpos.z -= 10.0f;

	if (m_dpos.z < 50.0f)
	{
		m_dpos.z = 50.0f;
	}
}



void FreeCamera::decreaseZoom()
{
	m_dpos.z += 10.0f;

	if (m_dpos.z > 800.0f)
	{
		m_dpos.z = 800.0f;
	}
}



void FreeCamera::allowRotation(GameData* _GD)
{
	m_yaw -= 0.01f * _GD->m_mouseState->lX;
	m_pitch -= 0.01f * _GD->m_mouseState->lY;

	// Limit camera rotation
	if (m_pitch > 1.0f)
	{
		m_pitch = 1.0f;
	}
	if (m_pitch < -1.0f)
	{
		m_pitch = -1.0f;
	}
}