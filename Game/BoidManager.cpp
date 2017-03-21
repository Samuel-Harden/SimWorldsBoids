#include "BoidManager.h"

#include "Boid.h"
#include "GameData.h"
#include "DrawData.h"
#include "BoidData.h"
#include "Waypoint.h"

#include "PositionCheck.h"
#include "FileReader.h"

// Behaviours
#include "Behaviour.h"
#include "Alignment.h"
#include "Avoidance.h"
#include "Cohesion.h"
#include "Separation.h"
#include "PathFinding.h"

#include <jsoncons/json.hpp>


BoidManager::BoidManager(ID3D11Device* _pd3dDevice, const int& _maxBoids)
	:  currentNoBoids(0),
	   updateGroup(0),
	   activeBoids(0),
	   noWayPoints(4),
	   boidRedFileName("..\\Resources\\BoidData\\redBoid.json"),
	   boidGreenFileName("..\\Resources\\BoidData\\greenBoid.json"),
	   boidPurpleFileName("..\\Resources\\BoidData\\purpleBoid.json")
{
	m_boids.reserve(_maxBoids);
	m_wpPos.reserve(noWayPoints);

	m_fileReader = std::make_unique<FileReader>();

	m_posCheck = new PositionCheck(); // NEW needs to be DELETED!

	createWaypoints(_pd3dDevice);

	createBoids(_pd3dDevice, _maxBoids);
	createBehaviours();
	createBoidData();
}



BoidManager::~BoidManager()
{
	// Tidy away stuf here
	for (std::vector<Boid*>::iterator it = m_boids.begin();
	it != m_boids.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}

	m_boids.clear();

	for (std::vector<BoidData*>::iterator it = m_boidData.begin();
	it != m_boidData.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}

	for (std::vector<WayPoint*>::iterator it = m_wayPoints.begin();
	it != m_wayPoints.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}

	m_wayPoints.clear();

	delete m_posCheck;
	m_posCheck = nullptr;
}



void BoidManager::Tick(GameData* _GD)
{
	// Update BoidData's Pred Position to that of GameData's pos (Player pos)
	for (std::vector<BoidData*>::iterator it = m_boidData.begin();
	it != m_boidData.end(); it++)
	{
		(*it)->predatorPos = _GD->predatorPos;
	}

	updateBoids(_GD);

	updateWayPoints(_GD);
}



void BoidManager::Draw(DrawData* _DD)
{
	for (int i = 0; i < currentNoBoids; i++)
	{
		if (m_boids[i]->isActive == true)
		{
			m_boids[i]->Draw(_DD);
		}
	}

	for (int i = 0; i < noWayPoints; i++)
	{
		m_wayPoints[i]->Draw(_DD);
	}
}



void BoidManager::updateBoids(GameData* _GD)
{
	int j = 0; // J refers to each faction, so that each faction will have the
	           // same number of boids updated during any one 'tick'

	for (int i = 0; i < currentNoBoids; i++)
	{
		if (m_boids[i]->isActive == true)
		{
			m_boids[i]->run(m_boids, _GD, updateGroup, m_boidData[j], m_behaviours, m_posCheck, m_wpPos);
			j++;

			if (j > 2)
			{
				j = 0;
			}
		}
	}

	updateGroup += 100;

	if (updateGroup >= m_boids.size())
	{
		updateGroup = 0;
	}
}



void BoidManager::updateWayPoints(GameData* _GD)
{
	for (int i = 0; i < noWayPoints; i++)
	{
		m_wayPoints[i]->Tick(_GD, m_wpPos[i]);
	}
}



void BoidManager::spawnBoid()
{
	// Spawns one of each faction/colour
	int boidsActivated = 0;

	for (int i = 0; i < m_boids.size(); i++)
	{
		if (m_boids[i]->isActive == false)
		{
			m_boids[i]->activateBoid();
			activeBoids++;
			boidsActivated++;

			// once three boids (one of each faction) have spawned, return
			if (boidsActivated == 3)
			{
				return;
			}
		}
	}
}



void BoidManager::resetBoids()
{
	for (int i = 0; i < m_boids.size(); i++)
	{
		if (m_boids[i]->isActive == true)
		{
			m_boids[i]->deactivateBoid();
		}
	}
}


void BoidManager::createBoids(ID3D11Device* _pd3dDevice, const int& _maxBoids)
{
	int id = 0;
	int faction = 0;

	for (int i = 0; i < _maxBoids; i++)
	{
		boid = new Boid(id, faction); // Pass in an int - Sets Boids ID
		boid->init(_pd3dDevice);
		m_boids.push_back(boid);
		currentNoBoids++;
		id++;
		faction++;

		// reset faction
		if (faction > 2)
		{
			faction = 0;
		}
	}
}



void BoidManager::createBehaviours()
{
	// Create behaviours, store alphabetically
	Alignment* m_align = new Alignment();
	m_behaviours.push_back(m_align);

	Avoidance* m_avoid = new Avoidance();
	m_behaviours.push_back(m_avoid);

	Cohesion* m_coh = new Cohesion();
	m_behaviours.push_back(m_coh);

	Separation* m_sep = new Separation();
	m_behaviours.push_back(m_sep);

	PathFinding* m_pf = new PathFinding();
	m_behaviours.push_back(m_pf);
}


void BoidManager::createBoidData()
{
	// Each faction has its own boid data...
	boidDataRed = new BoidData(m_fileReader, boidRedFileName);
	m_boidData.push_back(boidDataRed);

	boidDataGreen = new BoidData(m_fileReader, boidGreenFileName);
	m_boidData.push_back(boidDataGreen);

	boidDataPurple = new BoidData(m_fileReader, boidPurpleFileName);
	m_boidData.push_back(boidDataPurple);
}



void BoidManager::createWaypoints(ID3D11Device* _pd3dDevice)
{
	for (int i = 0; i < noWayPoints; i++)
	{
		wayPoint = new WayPoint();
		wayPoint->init(_pd3dDevice);
		m_wpPos.push_back(wayPoint->getPos());
		m_wayPoints.push_back(wayPoint);
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



float& BoidManager::getPathFindingWeight(int _faction)
{
	return m_boidData[_faction]->pathWeight;
}



float& BoidManager::getFlightFightWeight(int _faction)
{
	return m_boidData[_faction]->ffWeight;
}



float& BoidManager::getRunWeight(int _faction)
{
	return m_boidData[_faction]->runWeight;
}



float& BoidManager::getWayPointPosX(int _wayPoint)
{
	return m_wpPos[_wayPoint].x;
}



float& BoidManager::getWayPointPosY(int _wayPoint)
{
	return m_wpPos[_wayPoint].y;
}



float& BoidManager::getWayPointPosZ(int _wayPoint)
{
	return m_wpPos[_wayPoint].z;
}