#pragma once

#ifndef _H_APP
#define _H_APP

#include "GamePlay.h"
#include "MenuUI.h"

#define WANTED_FPS 40

enum class GAME_STATE
{
	play,
	play_level_is_completed,
	game_pause,
	game_pause_level_completed,	
	menu_ui,
	unspecified
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

	GAME_STATE	state		= GAME_STATE::unspecified;
	GAME_STATE	prevState	= GAME_STATE::unspecified;

private:
	void initWindow();
	void updateGameState();

	bool gamePlayHasStarted = false;

	std::vector<Sprite> loadingSprites;
	std::vector<AgkImage> loadingImages;
	void beginLoadingScreen();
	void endLoadingScreen();

};

extern app App;

#endif
