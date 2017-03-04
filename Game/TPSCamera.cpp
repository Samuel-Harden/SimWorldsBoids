#include "TPSCamera.h"
#include "GameData.h"

TPSCamera::TPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, Vector3 _up, Vector3 _dpos)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _up)
{
	//m_targetObject = Vector3::Zero;
	m_dpos = _dpos;
}

TPSCamera::~TPSCamera()
{

}

void TPSCamera::Tick(GameData* _GD)
{
	// Set up position  of camera and target position of camera based on new
	// position and orientation of target object

	Matrix rotCam = Matrix::CreateFromYawPitchRoll(getYaw(), getPitch(), 0.0f);

	// if in the third person view, we take mouse inputs to rotate the camera
	//if (camera_state == TPS_CAMERA)
	//{
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
	//}

	// Sets the positon of the camera
		m_pos = Vector3(0.01f, 0.01f, 0.01f) + Vector3::Transform(m_dpos, rotCam);

	// Sets what the camera is looking at
		m_target = Vector3(0.01f, 0.01f, 0.01f);

	// Then set up proj and view matrices
	Camera::Tick(_GD);
}

