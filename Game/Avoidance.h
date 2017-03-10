#pragma once

#include <vector>
#include <d3d11.h>
#include "SimpleMath.h"

//using namespace DirectX;
using namespace DirectX::SimpleMath;

class BoidPrey;
class BoidData;
class GameData;

class Avoidance
{
public:
	friend class BoidPrey;

	Avoidance();
	~Avoidance();

protected:
	Vector3 avoid(BoidPrey* _boid, BoidData* _BD, GameData* _GD);

private:

};
