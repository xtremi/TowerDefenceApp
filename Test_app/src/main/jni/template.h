#pragma once

#ifndef _H_APP
#define _H_APP

#include "GamePlay.h"
#include "MenuUI.h"

#define WANTED_FPS 60


enum GAME_STATE
{
	GAME_STATE_play,
	GAME_STATE_play_level_is_completed,
	GAME_STATE_game_pause,
	GAME_STATE_game_pause_level_completed,	
	GAME_STATE_menu_ui,
	GAME_STATE_unspecified
};

#ifdef WIN_64_BUILD
#define DEVICE_WIDTH VIRTUAL_WIDTH
#define DEVICE_HEIGHT VIRTUAL_HEIGHT
#define DEVICE_POS_X 32
#define DEVICE_POS_Y 32
#define FULLSCREEN false
#define COMPANY_NAME "My Company"
#endif

class app
{
public:
	app() {

    	}
	~app() {}
	void Begin(void);
	int Loop(void);
	void End(void);
	
private:
	GamePlay	gamePlay;
	MenuUI		menuUI;

	MediaBank	mediaBank;

	GAME_STATE	state		= GAME_STATE_unspecified;
	GAME_STATE	prevState	= GAME_STATE_unspecified;

private:
	void initWindow();
	void updateGameState();

	bool gamePlayHasStarted = false;

};

extern app App;

#endif
