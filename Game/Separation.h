#pragma once

#include <vector>
#include <d3d11.h>
#include "SimpleMath.h"

//using namespace DirectX;
using namespace DirectX::SimpleMath;

class BoidPrey;
class BoidData;

class Separation
{
public:
	friend class BoidPrey;

	Separation();
	~Separation();

protected:
	Vector3 separate(BoidPrey* _boid, BoidData* _BD, std::vector<BoidPrey*>& _boids);

private:

};