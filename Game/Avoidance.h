#pragma once

#include "Behaviour.h"

class Avoidance : public Behaviour
{
public:

	friend class BoidManager;
	friend class Boid;

	~Avoidance();

protected:
	Avoidance();

	DirectX::SimpleMath::Vector3 calculateBehaviour1(Boid* _boid, BoidData* _BD, std::vector<Boid*>& _boids) override;

	DirectX::SimpleMath::Vector3 calculateBehaviour2(Boid* _boid, BoidData* _BD) override;

private:

};
