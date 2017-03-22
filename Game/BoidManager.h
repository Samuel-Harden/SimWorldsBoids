#pragma once

// Boid System has been created using reference from David Shiffman's example
// found here
// https://processing.org/examples/flocking.html

#include <d3d11_1.h>

//#include <windows.h>
#include "SimpleMath.h"

#include <vector>
#include <memory>
#include <string>

class BoidData;
class Boid;
class WayPoint;
class FileReader;

class PositionCheck;

class Behaviour;

struct GameData;
struct DrawData;

class BoidManager
{
public:

	BoidManager(ID3D11Device* _pd3dDevice, const int& _maxBoids);
	~BoidManager();

	void Tick(GameData* _GD);
	void Draw(DrawData* _DD);

	void spawnBoid();
	void resetBoids();

	int activeBoids;

	BoidData* getBoidData(int _faction);

	float& getBoidSpeed(int _faction);
	float& getNeighbourDist(int _faction);
	float& getDesiredSeperation(int _faction);
	float& getSepWeight(int _faction);
	float& getAliWeight(int _faction);
	float& getCohWeight(int _faction);
	float& getPathFindingWeight(int _faction);
	float& getFlightFightWeight(int _faction);

	// Player related
	float& getRunWeight(int _faction);

	float& getWayPointPosX(int _wayPoint);
	float& getWayPointPosY(int _wayPoint);
	float& getWayPointPosZ(int _wayPoint);


protected:

private:

	void createBehaviours();
	void createBoidData();
	void createWaypoints(ID3D11Device* _pd3dDevice);
	void createBoids(ID3D11Device* _pd3dDevice, const int& _maxBoids);

	void updateBoids();
	void updateWayPoints(GameData* _GD);

	std::vector<Boid*> m_boids;

	std::vector<Behaviour*> m_behaviours;

	std::unique_ptr<FileReader> m_fileReader;

	// Vector to hold containers of faction Variables
	std::vector<BoidData*> m_boidData;

	// Vector of waypoints
	std::vector<WayPoint*> m_wayPoints;

	std::vector<DirectX::SimpleMath::Vector3> m_wpPos;

	// Faction Variables
	BoidData* boidDataRed;
	BoidData* boidDataGreen;
	BoidData* boidDataPurple;

	std::string boidRedFileName;
	std::string boidGreenFileName;
	std::string boidPurpleFileName;

	Boid* boid;

	WayPoint* wayPoint;

	PositionCheck* m_posCheck;

	int updateGroup;
	int currentNoBoids;
	int noWayPoints;
};