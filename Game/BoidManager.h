#pragma once

#include <d3d11_1.h>

#include <vector>
#include <string>
#include "Model.h"

class Boid;
class GameData;
class DrawData;

class BoidManager
{
public:

	BoidManager(std::string _fileName, ID3D11Device* _pd3dDevice, DirectX::IEffectFactory* _EF, int& _maxBoids);
	~BoidManager();

	void Tick(GameData* _GD);
	void Draw(DrawData* _DD);

	void spawnBoid(int& _maxBoids);

protected:

private:

	void setBoids(GameData* _GD);

	std::vector<Boid*> boids;

	Boid* boid;

	int currentNoBoids;
};