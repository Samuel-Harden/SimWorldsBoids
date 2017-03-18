#include "Behaviour.h"

#include"Boid.h"
#include "BoidData.h"



Behaviour::Behaviour()
{

}



Behaviour::~Behaviour()
{

}


DirectX::SimpleMath::Vector3 Behaviour::calculateBehaviour(Boid* _boid, BoidData* _BD, std::vector<Boid*>& _boids)
{
	// Default Value...
	return DirectX::SimpleMath::Vector3::Zero;
}