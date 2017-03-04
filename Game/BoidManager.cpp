#include "BoidManager.h"
#include "BoidPrey.h"
#include "Boid.h"
#include "GameData.h"
#include "DrawData.h"



BoidManager::BoidManager(ID3D11Device* _pd3dDevice, int& _maxBoids)
	: currentNoBoids (0)
{
	preyboids.reserve(_maxBoids);

	for (int i = 0; i < _maxBoids; i++)
	{
		boid = new BoidPrey();
		boid->init(_pd3dDevice);
		boid->SetScale(0.5f);
		preyboids.push_back(boid);
		currentNoBoids++;
	}
}



BoidManager::~BoidManager()
{
	// Tidy away stuf here
	for (std::vector<BoidPrey*>::iterator it = preyboids.begin(); it != preyboids.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
	preyboids.clear();
}



void BoidManager::Tick(GameData* _GD)
{
	setPreyBoids(_GD);
}



void BoidManager::Draw(DrawData* _DD)
{
	for (int i = 0; i < currentNoBoids; i++)
	{
		if (preyboids[i]->isActive == true)
		{
			preyboids[i]->Draw(_DD);
		}
	}
}



void BoidManager::setPreyBoids(GameData* _GD)
{
	for (int i = 0; i < currentNoBoids; i++)
	{
		if (preyboids[i]->isActive == true)
		{
			preyboids[i]->run(preyboids, _GD);
		}
	}
}



void BoidManager::spawnBoid()
{
	for (int i = 0; i < preyboids.size(); i++)
	{
		if (preyboids[i]->isActive == false)
		{
			preyboids[i]->activateBoid();
			return;
		}
	}
}