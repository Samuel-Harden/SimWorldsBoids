#pragma once

#include "Behaviour.h"

class PathFinding : public Behaviour
{
public:

	friend class BoidManager;
	friend class Boid;

	~PathFinding();

protected:
	PathFinding();

	DirectX::SimpleMath::Vector3 calculateBehaviour3(Boid* _boid, BoidData* _BD, std::vector<DirectX::SimpleMath::Vector3>& _wpPos) override;

private:

};
