#pragma once

#include "VBGO.h"
#include "vertex.h"
#include <vector>



class WayPoint : public VBGO
{
public:
	WayPoint();
	virtual ~WayPoint();

	//initialise the Veretx and Index buffers for the waypoints
	void init(ID3D11Device* _GD);

	void Tick(GameData* _GD, DirectX::SimpleMath::Vector3 _wayPoint);

protected:

	int m_size;
	myVertex* m_vertices;

private:


	void setRandPos();

	// Helper Function
	float randomFloat(float _min, float _max);

	int waypointID;

	float red = 1.0f;	  // Red colour Value for Vertices
	float blue = 1.0f;	  // Blue colour Value for Vertices
	float green = 1.0f;  // Green colour Value for Vertices

					  // Constants
	const int zoneSize = 180;
};