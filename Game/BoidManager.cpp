#include "BoidManager.h"
#include "Boid.h"
#include "GameData.h"
#include "DrawData.h"



BoidManager::BoidManager(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, int& _maxBoids)
	: currentNoBoids (0)
{
	boids.reserve(_maxBoids);

	for (int i = 0; i < _maxBoids; i++)
	{
		boid = new Boid(_fileName, _pd3dDevice, _EF);
		boids.push_back(boid);
		currentNoBoids++;
	}
}



BoidManager::~BoidManager()
{
	// Tidy away stuf here
	for (std::vector<Boid *>::iterator it = boids.begin(); it != boids.end(); it++)
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



void BoidManager::spawnBoid(int& _maxBoids)
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