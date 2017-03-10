#pragma once

// Boid System has been created using reference from David Shiffman's example
// found here
// https://processing.org/examples/flocking.html

#include <d3d11_1.h>

//#include <windows.h>

#include <vector>
#include <memory>
//#include "Model.h"

class BoidData;
class BoidPrey;
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

	BoidData* getBoidData();

	float& getBoidSpeed();
	float& getNeighbourDist();
	float& getDesiredSeperation();
	float& getSepWeight();
	float& getAliWeight();
	float& getCohWeight();
	float& getRunWeight();


protected:

private:

	void updatePreyBoids(GameData* _GD);

	std::vector<BoidPrey*> preyboids;

	BoidData* m_boidData;

	BoidPrey* boid;

	int updateGroup;
	int currentNoBoids;
};