#pragma once

#include <vector>
#include <d3d11.h>
#include "SimpleMath.h"

//using namespace DirectX;
using namespace DirectX::SimpleMath;

class BoidPrey;
class BoidData;

class Cohesion
{
public:
	friend class BoidPrey;

	Cohesion();
	~Cohesion();

protected:
	Vector3 cohesion(BoidPrey* _boid, BoidData* _BD, std::vector<BoidPrey*>& _boids);
	Vector3 seek(Vector3& target, BoidPrey* _boid, BoidData* _BD);

private:

};