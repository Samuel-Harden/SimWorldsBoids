#pragma once



//=================================================================
//Possible GameStates
//=================================================================



enum GameState 
{
	GS_NULL = 0,
	GS_MAIN_MENU,
	GS_PLAY_GAME,
	GS_PAUSE,
};



enum CameraState
{
	FREE_CAMERA, // MAIN FREE CAMERA
	TPS_CAMERA,  // CAMERA FOLLOWS PLAYER
};



extern GameState game_state;
extern CameraState camera_state;