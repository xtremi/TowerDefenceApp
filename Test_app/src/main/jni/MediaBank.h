#pragma once
#include "agk.h"
#include "logger.h"
#include "UI/Sprite.h"
#include "UI/AgkFont.h"
#include "UI/AgkSound.h"
#include "UI/AgkMusic.h"
#include <map>
#include <string>
#include "global.h"
#include "AnimationJSONReader.h"
#include "MediaJSONReader.h"

#define ANIMATIONS_JSON_FILE "json/animation_defs.json"
#define SHADERS_JSON_FILE "json/shader_defs.json"

enum class ASSET {

	IMG_RANGE_INDICATOR,		// images/interface/in_game_menu/range_indicator.png
	IMG_MAIN_MENU_START,		// images\interface\main_menu\glass-sphere.png
	IMG_GRASS,					// images\terrain\tile-grass-1.png
	IMG_GRASS_INACTIVE,			// images\terrain\tile-grass-1-inactive.png
	IMG_ROAD,					// images\terrain\tile-road-1.png
	IMG_ROAD_INACTIVE,			// images\terrain\tile-road-1-inactive.png
	IMG_ROAD_START,				// images\terrain\tile-start-1.png
	IMG_ROAD_END,				// images\terrain\tile-end-1-inactive.png

	IMG_CELL_SELECT,			// images\markers\select-cell-marker.png

	IMG_TOWER_TREE,				// images/interface/in_game_menu/tower-1.png
	IMG_TOWER_TREE_P,			// images/interface/in_game_menu/tower-2.png
    IMG_MENU,					// images/interface/in_game_menu/main-menu-1.png
    IMG_MENU_P,					// images/interface/in_game_menu/main-menu-2.png
    IMG_PAUSE,					// images/interface/in_game_menu/pause-1.png
    IMG_PAUSE_P,				// images/interface/in_game_menu/pause-2.png
    IMG_RESUME,					// images/interface/in_game_menu/resume-1.png
    IMG_RESUME_P,				// images/interface/in_game_menu/resume-2.png
    IMG_FAST,					// images/interface/in_game_menu/fast-1.png
    IMG_FAST_P,					// images/interface/in_game_menu/fast-2.png
    IMG_DEBUG,					// images/interface/in_game_menu/debug-1.png
    IMG_DEBUG_P,				// images/interface/in_game_menu/debug-2.png
	IMG_RETURN,					
	IMG_RETURN_P,				
    IMG_MUSIC_ON,				// images/interface/in_game_menu/sound-on-1.png
    IMG_MUSIC_ON_P,				// images/interface/in_game_menu/sound-on-2.png
    IMG_MUSIC_OFF,				// images/interface/in_game_menu/sound-off-1.png
    IMG_MUSIC_OFF_P,			// images/interface/in_game_menu/sound-off-2.png
    IMG_NEXT_WAVE,				// images/interface/in_game_menu/next-wave-3.png
    IMG_NEXT_WAVE_P,			// images/interface/in_game_menu/next-wave-2.png"

	IMG_COIN,					// images/interface/in_game_menu/coin_000.png
	IMG_HEART,					// images/interface/in_game_menu/heart.png

	IMG_CELL_MENU_BTN,			// images\interface\cell_select_menu\cell-menu-button.png
	IMG_CELL_MENU_BTN_SEL,		// images\interface\cell_select_menu\cell-menu-button-sel.png
	IMG_CELL_MENU_BTN_INC,		// images\interface\cell_select_menu\cell-menu-button-inactive.png
	IMG_CELL_MENU_AIM_ICON,		// images\interface\cell_select_menu\cell-menu-button-aim-icon.png				  
	IMG_CELL_MENU_SELL_ICON,	// images\interface\cell_select_menu\cell-menu-button-sell-icon.png
	IMG_CELL_MENU_MEGA_ICON,	// images\interface\cell_select_menu\cell-menu-button-mega-icon.png

	IMG_CELL_MENU_INFO_BACK,	// images\interface\cell_select_menu\info-background.png

	IMG_TEST_CHAIN_MARKER,		// images\test_chain_bullet_marker.png
	IMG_BLANK_128x128,			//images\blank\blank_128x128.png

	MUSIC_BG_01,				// sounds/music/guitar_53.mp3
	MUSIC_BG_02,				// sounds/music/DeusExRemix.mp3
	SOUND_SHOOT_TICK,			// sounds/shoot/tick.wav

	FONT_MAIN,					// fonts/neuropol-x-free/neuropol_x_rg.ttf
	FONT_COUR,					// fonts/Courier-Prime-Code/Courier-Prime-Code.tff

	FSHADER_CHAIN_WAVE,			// shaders/chain-wave.fs
	FSHADER_CHAIN_WAVE_FIRE1	// shaders/chain-wave-fire-01.fs
};

#define GETIMG(imgname) MEDIA_BANK.getImage(imgname)



/***********************************************/
class MediaBank
/***********************************************/
{
public:
	MediaBank(){}
	void init();

	struct ImgDetails {
		ImgDetails(char* _path, bool _magFilter = false) {
			path = _path;
			magFilter = _magFilter;
		}
		char* path;
		bool  magFilter;
	};

public:
	AgkImage* getImage(const uString& key);
	AgkImage* getImage(ASSET key);
	AgkFont* getFont(ASSET key);
	AgkSound* getSound(ASSET key);
	AgkMusic* getMusic(ASSET key);
	int getShader(ASSET key);
	int getShader(const std::string& key);
	
	AgkImage* addImage(const uString& path, const uString& key, bool magFilterNearest = false);
	AgkImage* addImage(char* path, ASSET key, bool magFilterNearest = false);
	void addFont(char* path, ASSET key);
	void addSound(char* path, ASSET key, bool isOGG = false);
	void addMusic(char* path, ASSET key, bool isOGG = false);
	void addShader(char* path, ASSET key);

	void playAnimation(const std::string& id, const glm::vec2& pos, int fps);
	Sprite* getAnimationSprite(const std::string& id);
	Sprite* createAnimationSpriteCopy(const std::string& id, const glm::vec2& size);

private:
	int sc = 0; int sc_sync = 128; //counter for Agk::Sync()

	void initAssetPathMaps();
	void loadExternalMedia();
	void loadAnimations();
	void loadShaders();
	std::map<ASSET, ImgDetails>	image_path_map;
	std::map<ASSET, char*>		font_path_map;
	std::map<ASSET, char*>		sound_path_map;
	std::map<ASSET, char*>		music_path_map;
	std::map<ASSET, char*>		shader_path_map;

	std::map<ASSET, AgkImage*>	images;
	std::map<ASSET, AgkFont*>	fonts;
	std::map<ASSET, AgkSound*>	sounds;
	std::map<ASSET, AgkMusic*>	musics;
	std::map<ASSET, int>		shaders;

	std::map<std::string, AgkImage*>				customImages;
	std::map<std::string, int>						customShaders;

	std::map<std::string, Sprite*>					animSprites;
	std::map<std::string, std::vector<AgkImage*>>	animImages;

	
};


extern MediaBank MEDIA_BANK;


