#pragma once

// Boid System has been created using reference from David Shiffman's example
// found here
// https://processing.org/examples/flocking.html

#include <d3d11_1.h>

//#include <windows.h>

#include <vector>
#include <memory>
#include <string>

class BoidData;
class Boid;
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
	void resetPreyBoids();

	int activeBoids;

	BoidData* getBoidData(int _faction);

	float& getBoidSpeed(int _faction);
	float& getNeighbourDist(int _faction);
	float& getDesiredSeperation(int _faction);
	float& getSepWeight(int _faction);
	float& getAliWeight(int _faction);
	float& getCohWeight(int _faction);
	float& getFlightFightWeight(int _faction);

	// Player related
	float& getRunWeight(int _faction);


protected:

private:

	void createBehaviours();
	void createBoidData();
	void createBoids(ID3D11Device* _pd3dDevice, const int& _maxBoids);

	void updateBoids(GameData* _GD);

	std::vector<Boid*> m_boids;

	std::vector<Behaviour*> m_behaviours;

	std::unique_ptr<FileReader> m_fileReader;

	// Vector to hold containers of faction Variables
	std::vector<BoidData*> m_boidData;

	// Faction Variables
	BoidData* boidDataRed;
	BoidData* boidDataGreen;
	BoidData* boidDataPurple;

	std::string boidRedFileName;
	std::string boidGreenFileName;
	std::string boidPurpleFileName;

	Boid* boid;

	PositionCheck* m_posCheck;

	int updateGroup;
	int currentNoBoids;
};