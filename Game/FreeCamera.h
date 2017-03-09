#pragma once

#include "camera.h"

//=================================================================
//TPS style camera which will follow a given GameObject around _target
//=================================================================

class FreeCamera : public Camera
{
public:
	FreeCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, Vector3 _up, Vector3 _dpos);
	virtual ~FreeCamera();

	virtual void Tick(GameData* _GD) override;

	void allowRotation(GameData* _GD);

	void increaseZoom();
	void decreaseZoom();

protected:
	GameObject*	m_targetObject; //I'm following this object
	Vector3	m_dpos; //I'll lurk this far behind and away from it
};