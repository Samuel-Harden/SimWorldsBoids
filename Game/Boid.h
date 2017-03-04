#pragma once
#include "CMOGO.h"

class Boid : public CMOGO
{
public:
	Boid(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);
	~Boid();


	virtual void Tick(GameData* _GD) override;
	void run(std::vector<Boid*>& boids, GameData* _GD);

	Vector3 getVelocity();

	bool isActive;
	void activateBoid();

protected:

private:
	void flock(std::vector<Boid*>& _boids);
	void applyForce(Vector3& force);
	float randomFloat(float _min, float _max);

	void checkPosition();

	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;

	float maxForce;
	float maxSpeed;

	int zoneSize;

	float neighbourDistance;
	float desiredSeperation;

	// Behaviours
	Vector3 seek(Vector3& target);
	Vector3 seperate(std::vector<Boid*>& _boids);
	Vector3 align(std::vector<Boid*>& _boids);
	Vector3 cohesion(std::vector<Boid*>& _boids);

	Matrix m_viewMat;
	Vector3 m_dir;
	Vector3 direction;

	Vector3 ClampVector(Vector3& _vector);
};