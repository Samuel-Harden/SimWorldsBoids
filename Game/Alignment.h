#pragma once

#include <vector>
#include <d3d11.h>
#include "SimpleMath.h"

//using namespace DirectX;
using namespace DirectX::SimpleMath;

class BoidPrey;
class BoidData;

class Alignment
{
public:
	friend class BoidPrey;

	Alignment();
	~Alignment();

protected:
	Vector3 align(BoidPrey* _boid, BoidData* _BD, std::vector<BoidPrey*>& _boids);

private:

};