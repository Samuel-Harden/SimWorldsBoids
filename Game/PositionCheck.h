#pragma once



class PositionCheck
{
public:
	friend class Boid;

	PositionCheck();
	~PositionCheck();

protected:
	void checkPos(float& _axis);

private:
	const int zoneSize = 220;
};