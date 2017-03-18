#pragma once

#include "Behaviour.h"

class Cohesion : public Behaviour
{
public:

	friend class BoidManager;
	friend class Boid;

	~Cohesion();

protected:
	Cohesion();

	DirectX::SimpleMath::Vector3 calculateBehaviour(Boid* _boid, BoidData* _BD, std::vector<Boid*>& _boids) override;
	DirectX::SimpleMath::Vector3 seek(DirectX::SimpleMath::Vector3& target, Boid* _boid, BoidData* _BD);

private:

};