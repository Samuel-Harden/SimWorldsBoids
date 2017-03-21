#include "TweakBar.h"

#include "BoidManager.h"



TweakBar::TweakBar(ID3D11Device* _pd3dDevice,
	std::unique_ptr<BoidManager>& _boidManager,
	float& _screenWidth, float& _screenHeight)
{
	// AntTweakBar Setup
	TwInit(TW_DIRECT3D11, _pd3dDevice);
	TwWindowSize(_screenWidth, _screenHeight);

	boidVariables = TwNewBar("Boid Parameters:");
	int myBarSize[2] = { 325, 840 };
	TwSetParam(boidVariables, NULL, "size", TW_PARAM_INT32, 2, myBarSize);

	// AntTweakBar Buttons /////////////////////////////////////////////////////

	TwAddButton(boidVariables, "comment1", NULL, NULL, " label='Red Faction: ' ");

	// RED FACTION VARIABLES
	TwAddVarRW(boidVariables, "Red   - BoidSpeed: ", TW_TYPE_FLOAT,
		&_boidManager->getBoidSpeed(0), "min=0.1 max=1.0 step=0.1");

	TwAddVarRW(boidVariables, "Red   - View Distance: ", TW_TYPE_FLOAT,
		&_boidManager->getNeighbourDist(0), "min=0.0 max=500.0 step=1.0");

	TwAddVarRW(boidVariables, "Red   - Desired Seperation: ", TW_TYPE_FLOAT,
		&_boidManager->getDesiredSeperation(0), "min=0.0 max=10.0 step=0.1");

	TwAddVarRW(boidVariables, "Red   - Separation - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getSepWeight(0), "min=-50.0 max=50.0 step=0.1");

	TwAddVarRW(boidVariables, "Red   - Alignment - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getAliWeight(0), "min=-50.0 max=50.0 step=0.1");

	TwAddVarRW(boidVariables, "Red   - Cohesion - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getCohWeight(0), "min=-50.0 max=50.0 step=0.1");

	TwAddVarRW(boidVariables, "Red   - Fight/Flight - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getFlightFightWeight(0), "min=-50.0 max=50.0 step=0.1");

	TwAddVarRW(boidVariables, "Red   - Path Finding - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getPathFindingWeight(0), "min=-50.0 max=0.0 step=0.5");

	TwAddVarRW(boidVariables, "Red   - Avoid Player - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getRunWeight(0), "min=-50.0 max=50.0 step=0.1");

	TwAddSeparator(boidVariables, "sep1", NULL);
	TwDefine(" barName/sep1 group='Parameters1' ");

	TwAddButton(boidVariables, "comment2", NULL, NULL, " label='Green Faction: ' ");

	//GREEN FACTION VARIABLES
	TwAddVarRW(boidVariables, "Green - BoidSpeed: ", TW_TYPE_FLOAT,
		&_boidManager->getBoidSpeed(1), "min=0.1 max=1.0 step=0.1");

	TwAddVarRW(boidVariables, "Green - View Distance: ", TW_TYPE_FLOAT,
		&_boidManager->getNeighbourDist(1), "min=0.0 max=500.0 step=1.0");

	TwAddVarRW(boidVariables, "Green - Desired Seperation: ", TW_TYPE_FLOAT,
		&_boidManager->getDesiredSeperation(1), "min=0.0 max=10.0 step=0.1");

	TwAddVarRW(boidVariables, "Green - Separation - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getSepWeight(1), "min=-50.0 max=50.0 step=0.1");

	TwAddVarRW(boidVariables, "Green - Alignment - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getAliWeight(1), "min=-50.0 max=50.0 step=0.1");

	TwAddVarRW(boidVariables, "Green - Cohesion - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getCohWeight(1), "min=-50.0 max=50.0 step=0.1");

	TwAddVarRW(boidVariables, "Green - Fight/Flight - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getFlightFightWeight(1), "min=-50.0 max=50.0 step=0.1");

	TwAddVarRW(boidVariables, "Green   - Path Finding - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getPathFindingWeight(1), "min=-50.0 max=0.0 step=0.5");

	TwAddVarRW(boidVariables, "Green   - Avoid Player - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getRunWeight(1), "min=-50.0 max=50.0 step=0.1");

	TwAddSeparator(boidVariables, "sep2", NULL);
	TwDefine(" barName/sep2 group='Parameters2' ");

	TwAddButton(boidVariables, "comment3", NULL, NULL, " label='Purple Faction: ' ");

	//Purple FACTION VARIABLES
	TwAddVarRW(boidVariables, "Purple   - BoidSpeed: ", TW_TYPE_FLOAT,
		&_boidManager->getBoidSpeed(2), "min=0.1 max=1.0 step=0.1");

	TwAddVarRW(boidVariables, "Purple   - View Distance: ", TW_TYPE_FLOAT,
		&_boidManager->getNeighbourDist(2), "min=0.0 max=500.0 step=1.0");

	TwAddVarRW(boidVariables, "Purple   - Desired Seperation: ", TW_TYPE_FLOAT,
		&_boidManager->getDesiredSeperation(2), "min=0.0 max=10.0 step=0.1");

	TwAddVarRW(boidVariables, "Purple   - Separation - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getSepWeight(2), "min=-50.0 max=50.0 step=0.1");

	TwAddVarRW(boidVariables, "Purple   - Alignment - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getAliWeight(2), "min=-50.0 max=50.0 step=0.1");

	TwAddVarRW(boidVariables, "Purple   - Cohesion - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getCohWeight(2), "min=-50.0 max=50.0 step=0.1");

	TwAddVarRW(boidVariables, "Purple   - Fight/Flight - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getFlightFightWeight(2), "min=-50.0 max=50.0 step=0.1");

	TwAddVarRW(boidVariables, "Purple   - Path Finding - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getPathFindingWeight(2), "min=-50.0 max=0.0 step=0.5");

	TwAddVarRW(boidVariables, "Purple   - Avoid Player - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getRunWeight(2), "min=-50.0 max=50.0 step=0.1");

	TwAddSeparator(boidVariables, "sep3", NULL);
	TwDefine(" barName/sep3 group='Parameters3' ");

	TwAddVarRO(boidVariables, "Total no Boids: ", TW_TYPE_INT32,
		&_boidManager->activeBoids, nullptr);

	TwAddSeparator(boidVariables, "sep4", NULL);
		TwDefine(" barName/sep4 group='Parameters4' ");

	TwAddButton(boidVariables, "comment4", NULL, NULL, " label='WayPoint Positions: ' ");

	TwAddSeparator(boidVariables, "sep4", NULL);
	TwDefine(" barName/sep4 group='Parameters4' ");

	TwAddVarRW(boidVariables, "WayPoint1 PosX: ", TW_TYPE_FLOAT,
		&_boidManager->getWayPointPosX(0), "min=-180.0 max=180.0 step=5.0");

	TwAddVarRW(boidVariables, "WayPoint1 PosY: ", TW_TYPE_FLOAT,
		&_boidManager->getWayPointPosY(0), "min=-180.0 max=180.0 step=5.0");

	TwAddVarRW(boidVariables, "WayPoint1 PosZ: ", TW_TYPE_FLOAT,
		&_boidManager->getWayPointPosZ(0), "min=-180.0 max=180.0 step=5.0");

	TwAddSeparator(boidVariables, "sep5", NULL);
	TwDefine(" barName/sep5 group='Parameters5' ");

	TwAddVarRW(boidVariables, "WayPoint2 PosX: ", TW_TYPE_FLOAT,
		&_boidManager->getWayPointPosX(1), "min=-180.0 max=180.0 step=5.0");

	TwAddVarRW(boidVariables, "WayPoint2 PosY: ", TW_TYPE_FLOAT,
		&_boidManager->getWayPointPosY(1), "min=-180.0 max=180.0 step=5.0");

	TwAddVarRW(boidVariables, "WayPoint2 PosZ: ", TW_TYPE_FLOAT,
		&_boidManager->getWayPointPosZ(1), "min=-180.0 max=180.0 step=5.0");

	TwAddSeparator(boidVariables, "sep6", NULL);
	TwDefine(" barName/sep6 group='Parameters6' ");

	TwAddVarRW(boidVariables, "WayPoint3 PosX: ", TW_TYPE_FLOAT,
		&_boidManager->getWayPointPosX(2), "min=-180.0 max=180.0 step=5.0");

	TwAddVarRW(boidVariables, "WayPoint3 PosY: ", TW_TYPE_FLOAT,
		&_boidManager->getWayPointPosY(2), "min=-180.0 max=180.0 step=5.0");

	TwAddVarRW(boidVariables, "WayPoint3 PosZ: ", TW_TYPE_FLOAT,
		&_boidManager->getWayPointPosZ(2), "min=-180.0 max=180.0 step=5.0");

	TwAddSeparator(boidVariables, "sep7", NULL);
	TwDefine(" barName/sep7 group='Parameters7' ");

	TwAddVarRW(boidVariables, "WayPoint4 PosX: ", TW_TYPE_FLOAT,
		&_boidManager->getWayPointPosX(3), "min=-180.0 max=180.0 step=5.0");

	TwAddVarRW(boidVariables, "WayPoint4 PosY: ", TW_TYPE_FLOAT,
		&_boidManager->getWayPointPosY(3), "min=-180.0 max=180.0 step=5.0");

	TwAddVarRW(boidVariables, "WayPoint4 PosZ: ", TW_TYPE_FLOAT,
		&_boidManager->getWayPointPosZ(3), "min=-180.0 max=180.0 step=5.0");


	// End AntTweakBar Stuff ///////////////////////////////////////////////////
}



TweakBar::~TweakBar()
{
	TwTerminate();
	//delete boidVariables;
	//boidVariables = nullptr;
}