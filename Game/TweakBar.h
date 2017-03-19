#pragma once

#include <AntTweakBar.h>
#include <memory>

#include "CommonStates.h"

class BoidManager;

class TweakBar
{
public:
	TweakBar(ID3D11Device* _pd3dDevice,
		std::unique_ptr<BoidManager>& _boidManager,
		float& _screenWidth, float& _screenHeight);

	~TweakBar();

protected:

private:
	TwBar* boidVariables;
};