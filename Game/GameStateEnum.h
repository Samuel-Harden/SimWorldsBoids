#pragma once



//=================================================================
//Possible GameStates
//=================================================================



enum GameStateEnum
{
	GS_NULL = 0,
	GS_MAIN_MENU,
	GS_PLAY_GAME,
	GS_PAUSE,
	GS_EXIT
};



enum CameraState
{
	FREE_CAMERA, // MAIN FREE CAMERA
	TPS_CAMERA,  // CAMERA FOLLOWS PLAYER
};



extern GameStateEnum game_state;
extern CameraState camera_state;