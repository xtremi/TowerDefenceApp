#pragma once
#include "agk.h"
#include "logger.h"
#include "UI/AgkImage.h"
#include "UI/AgkFont.h"
#include "UI/AgkSound.h"
#include <map>
#include <string>
#include "global.h"

#define GETIMG(imgname) MEDIA_BANK.getImage(imgname)

#define GRASS_IMAGE				"grass"
#define GRASS_IMAGE_PATH		"images/terrain/grass.png"
#define GRASS_IMAGE_b			"grass-border"
#define GRASS_IMAGE_b_PATH		"images/terrain/grass_border.png"
#define GRASS_IMAGE_c					"grass-1"
#define GRASS_IMAGE_c_PATH				"images/terrain/Sprite_FX_Grass_0001_border.png"
#define GRASS_INACTIVE_IMAGE_c			"grass-1-inactive"
#define GRASS_INACTIVE_IMAGE_c_PATH		"images/terrain/Sprite_FX_Grass_0001_border_inactive.png"

#define ROAD_BLOCK_IMAGE				"road-block"
#define ROAD_BLOCK_IMAGE_PATH			"images/terrain/block2.png"
#define ROAD_BLOCK_INACTIVE_IMAGE		"road-block-inactive"
#define ROAD_BLOCK_INACTIVE_IMAGE_PATH	"images/terrain/block2_inactive.png"

#define START_BLOCK_IMAGE		"start_block"
#define START_BLOCK_IMAGE_PATH	"images/terrain/white_tile.png"
#define END_BLOCK_IMAGE			"end_block"
#define END_BLOCK_IMAGE_PATH	"images/terrain/brown_tile.png"

#define EAST_WEST_IMAGE			"EW"
#define EAST_WEST_IMAGE_PATH	"images/terrain/east-west.png"
#define SOUTH_NORTH_IMAGE		"SN"
#define SOUTH_NORTH_IMAGE_PATH	"images/terrain/south-north.png"

#define NORTH_EAST_IMAGE		"NE"
#define NORTH_EAST_IMAGE_PATH	"images/terrain/north-east.png"
#define NORTH_WEST_IMAGE		"NW"
#define NORTH_WEST_IMAGE_PATH	"images/terrain/north-west.png"

#define SOUTH_EAST_IMAGE		"SE"
#define SOUTH_EAST_IMAGE_PATH	"images/terrain/south-east.png"
#define SOUTH_WEST_IMAGE		"SW"
#define SOUTH_WEST_IMAGE_PATH	"images/terrain/south-west.png"

#define TOWER_WOOD_IMAGE		"tower-wood-00"
#define TOWER_WOOD_IMAGE_PATH	"images/towers/tower_wood_02.png"

#define TOWER_BOX_01_IMAGE		"tower-box-00"
#define TOWER_BOX_01_IMAGE_PATH "images/towers/box_tower_01.png"

#define STONE_BULLET_IMAGE		"stone-bullet"
#define STONE_BULLET_IMAGE_PATH "images/bullets/stone_16x16.png"

#define CELL_SELECT_MARK		"cell-select-mark"
#define CELL_SELECT_MARK_PATH	"images/markers/select_cell_marker.png"

#define TOWER_RANGE_INDICATOR_IMAGE			"tower-range-indicator"
#define TOWER_RANGE_INDICATOR_IMAGE_PATH "images/markers/tower_range_indicator.png"

#define TOWER_DIRECTION_BUTTON_IMAGE		"tower-direction-button"
#define TOWER_DIRECTION_BUTTON_IMAGE_PATH	"images/markers/tower_direction_button.png"
#define TOWER_DIRECTION_BUTTON_P_IMAGE		"tower-direction-button-p"
#define TOWER_DIRECTION_BUTTON_P_IMAGE_PATH	"images/markers/tower_direction_button_pressed.png"
#define TOWER_DIRECTION_BUTTON_I_IMAGE		"tower-direction-button-i"
#define TOWER_DIRECTION_BUTTON_I_IMAGE_PATH	"images/markers/tower_direction_button_inactive.png"


#define ARROW_DIRECTION_IMAGE		"arrow-01"
#define ARROW_DIRECTION_IMAGE_PATH	"images/markers/arrow.png"

#define MENU_IMAGE				"main_menu"
#define MENU_IMAGE_PATH			"images/interface/in_game_menu/main_menu.png"
#define PAUSE_IMAGE				"pause"
#define PAUSE_IMAGE_PATH		"images/interface/in_game_menu/pause.png"

#define BUMPER_IMAGE			"bumper_image"
#define BUMPER_IMAGE_PATH		"images/bumper/glass_sphere.png"

#define FONT1					"font1"
#define FONT1_PATH				"fonts/neuropol-x-free/neuropol_x_rg.ttf"


#define MOBT1_IMAGE				"ball"
#define MOBT1_IMAGE_PATH		"images/mobs/alien-5.png"

#define TOWER1_BUTTON_IMAGE			"tower1-button"
#define TOWER1_BUTTON_IMAGE_PATH	"images/interface/tower_buttons/tower_button_1.png"
#define TOWER1_BUTTON_IMAGE_P		"tower1-button-pressed"
#define TOWER1_BUTTON_IMAGE_P_PATH	"images/interface/tower_buttons/tower_button_1_pressed.png"


#define BUY_TOWER_BUTTON_IMAGE				"buy-tower-button"
#define BUY_BUILDING_BUTTON_IMAGE			"buy-building-button"
#define BUY_OTHER_BUTTON_IMAGE				"buy-other-button"
#define SET_BUILDING_DIR_BUTTON_IMAGE		"set-building-dir"
#define UPGRADE_TOWER_BUTTON_IMAGE			"upgrade-tower"

#define BUY_TOWER_BUTTON_P_IMAGE			"buy-tower-button-p"
#define BUY_BUILDING_BUTTON_P_IMAGE			"buy-building-button-p"
#define BUY_OTHER_BUTTON_P_IMAGE			"buy-other-button-p"
#define SET_BUILDING_DIR_BUTTON_P_IMAGE		"set-building-dir-p"
#define UPGRADE_TOWER_BUTTON_P_IMAGE		"upgrade-tower-p"

#define BUY_TOWER_BUTTON_I_IMAGE			"buy-tower-button-i"
#define BUY_BUILDING_BUTTON_I_IMAGE			"buy-building-button-i"
#define BUY_OTHER_BUTTON_I_IMAGE			"buy-other-button-i"
#define SET_BUILDING_DIR_BUTTON_I_IMAGE		"set-building-dir-i"
#define UPGRADE_TOWER_BUTTON_I_IMAGE		"upgrade-tower-i"

#define BUY_TOWER_BUTTON_IMAGE_PATH			"images/interface/cell_select_menu/tower_button.png"
#define BUY_BUILDING_BUTTON_IMAGE_PATH		"images/interface/cell_select_menu/question_mark_button.png"
#define BUY_OTHER_BUTTON_IMAGE_PATH			"images/interface/cell_select_menu/question_mark_button.png"
#define SET_BUILDING_DIR_BUTTON_IMAGE_PATH	"images/interface/cell_select_menu/set_building_dir_button.png"
#define UPGRADE_TOWER_BUTTON_IMAGE_PATH		"images/interface/cell_select_menu/upgrade_tower_button.png"

#define BUY_TOWER_BUTTON_P_IMAGE_PATH		"images/interface/cell_select_menu/tower_button_pressed.png"
#define BUY_BUILDING_BUTTON_P_IMAGE_PATH	"images/interface/cell_select_menu/question_mark_button_pressed.png"
#define BUY_OTHER_BUTTON_P_IMAGE_PATH		"images/interface/cell_select_menu/question_mark_button_pressed.png"
#define SET_BUILDING_DIR_BUTTON_P_IMAGE_PATH "images/interface/cell_select_menu/set_building_dir_button_pressed.png"
#define UPGRADE_TOWER_BUTTON_P_IMAGE_PATH	"images/interface/cell_select_menu/upgrade_tower_button_pressed.png"

#define BUY_TOWER_BUTTON_I_IMAGE_PATH		"images/interface/cell_select_menu/tower_button_inactive.png"
#define BUY_BUILDING_BUTTON_I_IMAGE_PATH	"images/interface/cell_select_menu/question_mark_button_inactive.png"
#define BUY_OTHER_BUTTON_I_IMAGE_PATH		"images/interface/cell_select_menu/question_mark_button_inactive.png"
#define SET_BUILDING_DIR_BUTTON_I_IMAGE_PATH "images/interface/cell_select_menu/set_building_dir_button_inactive.png"
#define UPGRADE_TOWER_BUTTON_I_IMAGE_PATH	"images/interface/cell_select_menu/upgrade_tower_button_inactive.png"

#define COIN_IMAGE_PATH						"images/interface/in_game_menu/coin_000.png"
#define COIN_IMAGE							"coin"

//sounds
#define BG_MUSIC_01_PATH		"sounds/music/guitar_53.ogg"
#define BG_MUSIC_01				"guitar53"
#define BG_MUSIC_02_PATH		"sounds/music/DeusExRemix.wav"
#define BG_MUSIC_02				"deusExRemix"
#define SHOOT_TICK_PATH			"sounds/shoot/tick.wav"
#define SHOOT_TICK				"tick"

/***********************************************/
class MediaBank
/***********************************************/
{
public:
	MediaBank(){}

	/*AgkImage* getImage(const uString& key) {
		std::string stdkey = "";
		
		for (int i = 0; i < key.GetNumChars(); i++)
			stdkey.push_back(key[i]);
		return getImage(key);
	}*/

	AgkImage* getImage(const std::string& key) {
		if (images.find(key) != images.end())
			return images[key];
		LOGW("Image with key \"%s\" not found.", &key[0]);
		return NULL;
	}

	AgkFont* getFont(const std::string& key) {
		if (fonts.find(key) != fonts.end())
			return fonts[key];
		LOGW("Font with key \"%s\" not found.", &key[0]);
		return NULL;
	}

	AgkSound* getSound(const std::string& key) {
		if (sounds.find(key) != sounds.end())
			return sounds[key];
		LOGW("Sound with key \"%s\" not found.", &key[0]);
		return NULL;
	}


	AgkImage* addImage(const uString& path, const uString& key, bool magFilterNearest = false) {
		return addImage(&toStdStr(path)[0], toStdStr(key), magFilterNearest);
	}

	AgkImage* addImage(char* path, const std::string& key, bool magFilterNearest = false) {		
		if (images.find(key) == images.end()) {
			AgkImage* img = new AgkImage(path);
			if(magFilterNearest)
				agk::SetImageMagFilter(img->getID(), 0);
			images[key] = img;
			return img;
		}
		else{
			LOGW("MediaBank::addImage() - tried to add image with already used key.");
			return NULL;
		}
	}

	void addFont(char* path, const std::string& key) {
		if (fonts.find(key) == fonts.end()) {
			AgkFont* font = new AgkFont(path);
			fonts[key] = font;
		}
		else
			LOGW("MediaBank::addFont() - tried to add font with already used key.");
	}

	void addSound(char* path, const std::string& key, bool isOGG = false) {
		if (sounds.find(key) == sounds.end()) {
			AgkSound* sound = new AgkSound(path, isOGG);
			sounds[key] = sound;
		}
		else
			LOGW("MediaBank::addSound() - tried to add sound with already used key.");
	}

private:
	std::map<std::string, AgkImage*>	images;
	std::map<std::string, AgkFont*>		fonts;
	std::map<std::string, AgkSound*>	sounds;
};


extern MediaBank MEDIA_BANK;

void initMediaBank();
