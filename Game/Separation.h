#pragma once

#include "Behaviour.h"

class Separation: public Behaviour
{
public:

	friend class BoidManager;
	friend class Boid;

	~Separation();

protected:
	Separation();

	DirectX::SimpleMath::Vector3 calculateBehaviour(Boid* _boid, BoidData* _BD, std::vector<Boid*>& _boids) override;

private:

};