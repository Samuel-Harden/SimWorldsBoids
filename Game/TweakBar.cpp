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
	int myBarSize[2] = { 325, 355 };
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

	//TwAddVarRW(boidVariables, "Red   - Fight/Flight F2 - (Weight): ", TW_TYPE_FLOAT,
	//	&m_boidManager->getFlightFightWeight2(0), "min=-50.0 max=50.0 step=0.1");

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

	//TwAddVarRW(boidVariables, "Green - Fight/Flight F2 - (Weight): ", TW_TYPE_FLOAT,
	//	&m_boidManager->getFlightFightWeight2(1), "min=-50.0 max=50.0 step=0.1");

	TwAddVarRW(boidVariables, "Green - Avoid Player - (Weight): ", TW_TYPE_FLOAT,
		&_boidManager->getRunWeight(1), "min=-50.0 max=50.0 step=0.1");

	TwAddSeparator(boidVariables, "sep2", NULL);
	TwDefine(" barName/sep2 group='sep' ");

	TwAddVarRO(boidVariables, "Total no Boids: ", TW_TYPE_INT32,
		&_boidManager->activeBoids, nullptr);

	// End AntTweakBar Stuff ///////////////////////////////////////////////////
}



TweakBar::~TweakBar()
{
	TwTerminate();
	//delete boidVariables;
	//boidVariables = nullptr;
}