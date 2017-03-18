#pragma once

#include "Behaviour.h"

class Alignment : public Behaviour
{
public:

	friend class BoidManager;
	friend class Boid;

	~Alignment();

protected:
	Alignment();

	DirectX::SimpleMath::Vector3 calculateBehaviour(Boid* _boid, BoidData* _BD, std::vector<Boid*>& _boids) override;

private:

};