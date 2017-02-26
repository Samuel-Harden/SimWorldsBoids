#include "BoidManager.h"
#include "VBBoidPrey.h"
#include "GameData.h"
#include "DrawData.h"



BoidManager::BoidManager(ID3D11Device* _pd3dDevice, int& _maxBoids)
	: currentNoBoids (0)
{
	boids.reserve(_maxBoids);

	for (int i = 0; i < _maxBoids; i++)
	{
		boid = new VBBoidPrey();
		boid->init(11, _pd3dDevice);
		boid->SetScale(0.5f);
		boids.push_back(boid);
		currentNoBoids++;
	}
}



BoidManager::~BoidManager()
{
	// Tidy away stuf here
	for (std::vector<VBBoidPrey*>::iterator it = boids.begin(); it != boids.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
	boids.clear();
}



void BoidManager::Tick(GameData* _GD)
{
	setBoids(_GD);
}



void BoidManager::Draw(DrawData* _DD)
{
	for (int i = 0; i < currentNoBoids; i++)
	{
		if (boids[i]->isActive == true)
		{
			boids[i]->Draw(_DD);
		}
	}
}



void BoidManager::setBoids(GameData* _GD)
{
	for (int i = 0; i < currentNoBoids; i++)
	{
		if (boids[i]->isActive == true)
		{
			boids[i]->run(boids, _GD);
		}
	}
}



void BoidManager::spawnBoid()
{
	for (int i = 0; i < boids.size(); i++)
	{
		if (boids[i]->isActive == false)
		{
			boids[i]->activateBoid();
			return;
		}
	}
}