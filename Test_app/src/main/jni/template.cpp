#include "template.h"


using namespace AGK;
app App;

/****************************************************/
void app::initWindow()
/****************************************************/
{
	LOGI("app::initWindow()");
	agk::SetClearColor(255, 255, 255);
	agk::SetSyncRate(WANTED_FPS, 0);
	agk::SetScissor(0, 0, 0, 0);
	agk::UseNewDefaultFonts(0);
	//agk::SetTextDefaultMagFilter(1);
	LOGI("app::initWindow() - complete");
}



/****************************************************/
void app::Begin(void)
/****************************************************/
{
	LOGI("app::Begin()");
#ifdef WIN_64_BUILD
	int s = agk::SetFolder("/assets/");
	std::string f = agk::GetFolder();
#endif
	

	initMediaBank();
	initWindow();	
	menuUI.init();
	gamePlay.init();
	menuUI.show();
	
	prevState = GAME_STATE_play;
	state = GAME_STATE_menu_ui;
	LOGI("app::Begin() - complete");
}

/****************************************************/
int app::Loop (void)
/****************************************************/
{
	bool pPressed = agk::GetPointerPressed();
	bool pReleased	= agk::GetPointerReleased();	
	bool pState		= agk::GetPointerState();

	if(state == GAME_STATE_play){
		gamePlay.update(pPressed, pReleased, pState);
	}
	else if (state == GAME_STATE_game_pause) {
		gamePlay.getHud()->update(pPressed, pReleased, pState);
	}
	else if (state == GAME_STATE_menu_ui){
		menuUI.update(pPressed, pReleased, pState);
	}
	else if (state == GAME_STATE_game_pause_level_completed) {
		
	}

	updateGameState();

	agk::Sync();
	return 0;
}

/****************************************************/
void app::updateGameState()
/****************************************************/
{
	if(state == GAME_STATE_play || state == GAME_STATE_game_pause){

		if (gamePlay.levelIsCompleted()) {

			gamePlay.pause(false);
			gamePlay.getHud()->hide();
			state = GAME_STATE_game_pause_level_completed;
		}

		if (gamePlay.getHud()->goToMainMenuInvoked()) {
			menuUI.show();
			gamePlay.pause(false);
			gamePlay.hide();
			prevState = state;
			state = GAME_STATE_menu_ui;
		}
		else if (gamePlay.getHud()->pauseGameInvoked()) {
			
			if(state == GAME_STATE_play){
				gamePlay.pause(true);
				state = GAME_STATE_game_pause;
			}
			else {
				gamePlay.unpause();
				state = GAME_STATE_play;
			}
				
		}

	}
	else if(state == GAME_STATE_menu_ui){

		if (menuUI.playGameInvoked()) {
			menuUI.hide();
			gamePlay.show();
			if(!gamePlayHasStarted){
				gamePlay.reset(1);
				gamePlayHasStarted = true;
			}
			
			
			state = prevState;
			if(state == GAME_STATE_play)
				gamePlay.unpause();
		}

	}
	else if (state == GAME_STATE_game_pause_level_completed) {



	}
}

/****************************************************/
void app::End (void)
/****************************************************/
{
    
}
