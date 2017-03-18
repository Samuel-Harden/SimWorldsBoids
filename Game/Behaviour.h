#pragma once

#include <vector>
#include <d3d11.h>

#include "SimpleMath.h"

class BoidData;
class Boid;

class Behaviour
{
public:
	friend class Boid;
	friend class BoidManager;

	virtual ~Behaviour();

protected:
	Behaviour();
	virtual DirectX::SimpleMath::Vector3 calculateBehaviour(Boid* _boid, BoidData* _BD, std::vector<Boid*>& _boids);

private:

};