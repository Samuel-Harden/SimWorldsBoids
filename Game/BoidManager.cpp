#include "BoidManager.h"

#include "BoidPrey.h"
#include "GameData.h"
#include "DrawData.h"
#include "BoidData.h"


BoidManager::BoidManager(ID3D11Device* _pd3dDevice, const int& _maxBoids)
	      : currentNoBoids (0),
	      updateGroup(0)
{
	preyboids.reserve(_maxBoids);

	activeBoids = 0;
	int j = 0;

	for (int i = 0; i < _maxBoids; i++)
	{
		boid = new BoidPrey(j); // Pass in an int - Sets Boids ID
		boid->init(_pd3dDevice);
		preyboids.push_back(boid);
		currentNoBoids++;
		j++;
	}

	m_boidData = new BoidData();
}



BoidManager::~BoidManager()
{
	// Tidy away stuf here
	for (std::vector<BoidPrey*>::iterator it = preyboids.begin();
	it != preyboids.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
	preyboids.clear();

	delete m_boidData;
	m_boidData = nullptr;
}



void BoidManager::Tick(GameData* _GD)
{
	updatePreyBoids(_GD);
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



void BoidManager::updatePreyBoids(GameData* _GD)
{
	for (int i = 0; i < currentNoBoids; i++)
	{
		if (preyboids[i]->isActive == true)
		{
			preyboids[i]->run(preyboids, _GD, updateGroup, m_boidData);
		}
	}

	updateGroup += 100;

	if (updateGroup == preyboids.size())
	{
		updateGroup = 0;
	}
}



void BoidManager::spawnBoid()
{
	for (int i = 0; i < preyboids.size(); i++)
	{
		if (preyboids[i]->isActive == false)
		{
			preyboids[i]->activateBoid();
			activeBoids++;
			return;
		}
	}
}



void BoidManager::resetPreyBoids()
{
	for (int i = 0; i < preyboids.size(); i++)
	{
		if (preyboids[i]->isActive == true)
		{
			preyboids[i]->deactivateBoid();
		}
	}
}



BoidData* BoidManager::getBoidData()
{
	return m_boidData;
}



float& BoidManager::getBoidSpeed()
{
	return m_boidData->boidSpeed;
}



float& BoidManager::getNeighbourDist() 
{
	return m_boidData->neighbourDist;
}



float& BoidManager::getDesiredSeperation()
{
	return m_boidData->desiredSep;
}



float& BoidManager::getSepWeight()
{
	return m_boidData->sepWeight;
}



float& BoidManager::getAliWeight()
{
	return m_boidData->aliWeight;
}



float& BoidManager::getCohWeight()
{
	return m_boidData->cohWeight;
}



float& BoidManager::getRunWeight()
{
	return m_boidData->runWeight;
}