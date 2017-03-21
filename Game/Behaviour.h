#pragma once

#include <vector>
#include <d3d11.h>

#include "SimpleMath.h"

class BoidData;
class GameData;
class Boid;

class Behaviour
{
public:
	friend class Boid;
	friend class BoidManager;

	virtual ~Behaviour();

protected:
	Behaviour();
	virtual DirectX::SimpleMath::Vector3 calculateBehaviour1(Boid* _boid, BoidData* _BD, std::vector<Boid*>& _boids);

	virtual DirectX::SimpleMath::Vector3 calculateBehaviour2(Boid* _boid, BoidData* _BD);

	virtual DirectX::SimpleMath::Vector3 calculateBehaviour3(Boid* _boid, BoidData* _BD, std::vector<DirectX::SimpleMath::Vector3>& _wpPos);

private:

};