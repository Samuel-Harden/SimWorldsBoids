#pragma once

// Boid System has been created using reference from David Shiffman's example
// found here
// https://processing.org/examples/flocking.html

#include <d3d11_1.h>

#include <windows.h>
#include <list>
#include <dinput.h>
#include "Effects.h"
#include "CommonStates.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "Audio.h"

#include <vector>
#include <string>
#include "Model.h"

class BoidPrey;
class Player;
struct GameData;
struct DrawData;

class BoidManager
{
public:

	BoidManager(ID3D11Device* _pd3dDevice, int& _maxBoids);
	~BoidManager();

	void Tick(GameData* _GD);
	void Draw(DrawData* _DD);

	void spawnBoid();

protected:

private:

	void setPreyBoids(GameData* _GD);

	std::vector<BoidPrey*> preyboids;

	BoidPrey* boid;

	int currentNoBoids;
};