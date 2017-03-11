#include "BoidManager.h"

#include "BoidPrey.h"
#include "GameData.h"
#include "DrawData.h"
#include "BoidData.h"


BoidManager::BoidManager(ID3D11Device* _pd3dDevice, const int& _maxBoids)
	:  currentNoBoids(0),
	      updateGroup(0),
	      activeBoids(0)
{
	preyboids.reserve(_maxBoids);

	int id = 0;
	int faction = 0;

	for (int i = 0; i < _maxBoids; i++)
	{
		boid = new BoidPrey(id, faction); // Pass in an int - Sets Boids ID
		boid->init(_pd3dDevice);
		preyboids.push_back(boid);
		currentNoBoids++;
		id++;
		faction++;

		// reset faction
		if (faction == 3)
		{
			faction = 0;
		}
	}

	boidDataRed = new BoidData();
	m_boidData.push_back(boidDataRed);

	boidDataGreen = new BoidData();
	m_boidData.push_back(boidDataGreen);

	boidDataPurple = new BoidData();
	m_boidData.push_back(boidDataPurple);
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

	for (std::vector<BoidData*>::iterator it = m_boidData.begin();
	it != m_boidData.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}

	m_boidData.clear();
}



void BoidManager::Tick(GameData* _GD)
{
	updateBoids(_GD);
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



void BoidManager::updateBoids(GameData* _GD)
{
	int j = 0;

	for (int i = 0; i < currentNoBoids; i++)
	{
		if (preyboids[i]->isActive == true)
		{
			preyboids[i]->run(preyboids, _GD, updateGroup, m_boidData[j]);
			j++;

			if (j > 2)
			{
				j = 0;
			}
		}
	}

		updateGroup += 99;

	if (updateGroup >= preyboids.size())
	{
		updateGroup = 0;
	}
}



void BoidManager::spawnBoid()
{
	// Spawns one of each faction/colour
	int boidsActivated = 0;

	for (int i = 0; i < preyboids.size(); i++)
	{
		if (preyboids[i]->isActive == false)
		{
			preyboids[i]->activateBoid();
			activeBoids++;
			boidsActivated++;

			// once three boids (one of each faction) has spawned, return
			if (boidsActivated == 3)
			{
				return;
			}
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



// These are Getters for boidData, needed for AntTweakBar, to access 
// from game must go through boidManager...

BoidData* BoidManager::getBoidData(int _faction)
{
	return m_boidData[_faction];
}



float& BoidManager::getBoidSpeed(int _faction)
{
	return m_boidData[_faction]->boidMaxSpeed;
}



float& BoidManager::getNeighbourDist(int _faction)
{
	return m_boidData[_faction]->neighbourDist;
}



float& BoidManager::getDesiredSeperation(int _faction)
{
	return m_boidData[_faction]->desiredSep;
}



float& BoidManager::getSepWeight(int _faction)
{
	return m_boidData[_faction]->sepWeight;
}



float& BoidManager::getAliWeight(int _faction)
{
	return m_boidData[_faction]->aliWeight;
}



float& BoidManager::getCohWeight(int _faction)
{
	return m_boidData[_faction]->cohWeight;
}



float& BoidManager::getFlightFightWeight(int _faction)
{
	return m_boidData[_faction]->ffWeight;
}



float& BoidManager::getRunWeight(int _faction)
{
	return m_boidData[_faction]->runWeight;
}