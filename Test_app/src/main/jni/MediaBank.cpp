#include "MediaBank.h"

MediaBank MEDIA_BANK;


void MediaBank::initAssetPathMaps() {

	image_path_map = std::map<ASSET, ImgDetails>{
		{ ASSET::IMG_RANGE_INDICATOR,		ImgDetails("images/interface/in_game_menu/range_indicator.png")			      },
		{ ASSET::IMG_MAIN_MENU_START,		ImgDetails("images/interface/main_menu/glass-sphere.png")					  },
			{ ASSET::IMG_GRASS,					ImgDetails("images/terrain/sand.png", true)							  },
			{ ASSET::IMG_GRASS_INACTIVE,		ImgDetails("images/terrain/sand-inactive.png", true)				  },
			{ ASSET::IMG_ROAD,					ImgDetails("images/terrain/tile-road-1.png", true)							  },
			{ ASSET::IMG_ROAD_INACTIVE,			ImgDetails("images/terrain/tile-road-1-inactive.png", true)					  },
		{ ASSET::IMG_BLANK_128x128,			ImgDetails("images/blank/blank_128x128.png", true)							  },
		{ ASSET::IMG_ROAD_START,			ImgDetails("images/terrain/tile-22.png")									  },
		{ ASSET::IMG_ROAD_END,				ImgDetails("images/terrain/tile-end-1.png")									  },
		{ ASSET::IMG_CELL_SELECT,			ImgDetails("images/markers/select-cell-marker-1.png", true)					  },
		{ ASSET::IMG_MENU,					ImgDetails("images/interface/in_game_menu/main-menu-1.png")					  },
		{ ASSET::IMG_MENU_P,				ImgDetails("images/interface/in_game_menu/main-menu-2.png")					  },
		{ ASSET::IMG_PAUSE,					ImgDetails("images/interface/in_game_menu/pause-1.png")						  },
		{ ASSET::IMG_PAUSE_P,				ImgDetails("images/interface/in_game_menu/pause-2.png")						  },
		{ ASSET::IMG_RESUME,				ImgDetails("images/interface/in_game_menu/resume-1.png")					  },
		{ ASSET::IMG_RESUME_P,				ImgDetails("images/interface/in_game_menu/resume-2.png")					  },
		{ ASSET::IMG_FAST,					ImgDetails("images/interface/in_game_menu/fast-1.png")						  },
		{ ASSET::IMG_FAST_P,				ImgDetails("images/interface/in_game_menu/fast-2.png")						  },
		{ ASSET::IMG_DEBUG,					ImgDetails("images/interface/in_game_menu/debug-1.png")						  },
		{ ASSET::IMG_DEBUG_P,				ImgDetails("images/interface/in_game_menu/debug-2.png")						  },
		{ ASSET::IMG_MUSIC_ON,				ImgDetails("images/interface/in_game_menu/sound-on-1.png")					  },
		{ ASSET::IMG_MUSIC_ON_P,			ImgDetails("images/interface/in_game_menu/sound-on-2.png")					  },
		{ ASSET::IMG_MUSIC_OFF,				ImgDetails("images/interface/in_game_menu/sound-off-1.png")					  },
		{ ASSET::IMG_MUSIC_OFF_P,			ImgDetails("images/interface/in_game_menu/sound-off-2.png")					  },
		{ ASSET::IMG_NEXT_WAVE,				ImgDetails("images/interface/in_game_menu/next-wave-3.png")					  },
		{ ASSET::IMG_NEXT_WAVE_P,			ImgDetails("images/interface/in_game_menu/next-wave-2.png")					  },
		{ ASSET::IMG_COIN,					ImgDetails("images/interface/in_game_menu/coin_000.png")					  },
		{ ASSET::IMG_HEART,					ImgDetails("images/interface/in_game_menu/heart.png")						  },
		{ ASSET::IMG_CELL_MENU_BTN,			ImgDetails("images/interface/cell_select_menu/cell-menu-button.png")		  },
		{ ASSET::IMG_CELL_MENU_BTN_SEL,		ImgDetails("images/interface/cell_select_menu/cell-menu-button-sel.png")	  },
		{ ASSET::IMG_CELL_MENU_BTN_INC,		ImgDetails("images/interface/cell_select_menu/cell-menu-button-inactive.png") },
		{ ASSET::IMG_CELL_MENU_AIM_ICON,	ImgDetails("images/interface/cell_select_menu/cell-menu-button-aim-icon.png") },
		{ ASSET::IMG_CELL_MENU_SELL_ICON,	ImgDetails("images/interface/cell_select_menu/cell-menu-button-sell-icon.png")},
		{ ASSET::IMG_CELL_MENU_MEGA_ICON,	ImgDetails("images/interface/cell_select_menu/cell-menu-button-mega-icon.png")},
		{ ASSET::IMG_CELL_MENU_INFO_BACK,	ImgDetails("images/interface/cell_select_menu/info-background.png")			  },
		{ ASSET::IMG_TEST_CHAIN_MARKER,		ImgDetails("images/test_chain_bullet_marker.png")							  }

	};

	music_path_map = std::map<ASSET, char*>{
		{ ASSET::MUSIC_BG_01,				"sounds/music/guitar_53.mp3"   },
		{ ASSET::MUSIC_BG_02,				"sounds/music/DeusExRemix.mp3" }
	};

	sound_path_map = std::map<ASSET, char*>{
		{ ASSET::SOUND_SHOOT_TICK,			"sounds/shoot/tick.wav"}	
	};

	font_path_map = std::map<ASSET, char*>{
		{ ASSET::FONT_MAIN,					"fonts/neuropol-x-free/neuropol_x_rg.ttf"},
		{ ASSET::FONT_COUR,					"fonts/droid-sans-mono/DroidSansMono.ttf"}
	};

	shader_path_map = std::map<ASSET, char*>{
		//{ ASSET::FSHADER_CHAIN_WAVE,					"shaders/chain-wave.fs"},
		//{ ASSET::FSHADER_CHAIN_WAVE_FIRE1,				"shaders/chain-wave-fire-01.fs"}
	};


}

/***********************************************/
void MediaBank::init()
/***********************************************/
{
	initAssetPathMaps();
	
	for (std::map<ASSET, ImgDetails>::iterator it = image_path_map.begin(); it != image_path_map.end(); it++){
		addImage(it->second.path, it->first, it->second.magFilter);	
		if (sc++ == sc_sync) { agk::Sync(); sc = 0; }
	}
	for (std::map<ASSET, char*>::iterator it = music_path_map.begin(); it != music_path_map.end(); it++){
		addMusic(it->second, it->first);
		if (sc++ == sc_sync) { agk::Sync(); sc = 0; }
	}
	for (std::map<ASSET, char*>::iterator it = sound_path_map.begin(); it != sound_path_map.end(); it++){
		addSound(it->second, it->first);
		if (sc++ == sc_sync) { agk::Sync(); sc = 0; }
	}
	for (std::map<ASSET, char*>::iterator it = font_path_map.begin(); it != font_path_map.end(); it++){
		addFont(it->second, it->first);
		if (sc++ == sc_sync) { agk::Sync(); sc = 0; }
	}
	for (std::map<ASSET, char*>::iterator it = shader_path_map.begin(); it != shader_path_map.end(); it++){
		addShader(it->second, it->first);
		if (sc++ == sc_sync) { agk::Sync(); sc = 0; }
	}

	image_path_map.clear();
	music_path_map.clear();
	sound_path_map.clear();
	font_path_map.clear();
	shader_path_map.clear();

	loadExternalMedia();
}

int MediaBank::getShader(const std::string& key) {	
	if (customShaders.find(key) != customShaders.end())
		return customShaders[key];
	LOGW("Shader with key \"%s\" not found.", &key[0]);
	return NULL;
}

AgkImage* MediaBank::getImage(const uString& key) {
	std::string keystr = std::string(key.GetStr());
	if (customImages.find(keystr) != customImages.end())
		return customImages[keystr];
	LOGW("Image with key \"%s\" not found.", key.GetStr());
	return NULL;
}

AgkImage* MediaBank::getImage(ASSET key) {
	if (images.find(key) != images.end())
		return images[key];
	LOGW("Image with key \"%d\" not found.", (int)key);
	return NULL;
}

AgkFont* MediaBank::getFont(ASSET key) {
	if (fonts.find(key) != fonts.end())
		return fonts[key];
	LOGW("Font with key \"%d\" not found.", (int)key);
	return NULL;
}

AgkSound* MediaBank::getSound(ASSET key) {
	if (sounds.find(key) != sounds.end())
		return sounds[key];
	LOGW("Sound with key \"%d\" not found.", (int)key);
	return NULL;
}

int MediaBank::getShader(ASSET key){
	if (shaders.find(key) != shaders.end())
		return shaders[key];
	LOGW("Shader with key \"%d\" not found.", (int)key);
	return 0;
}

AgkMusic* MediaBank::getMusic(ASSET key) {
	if (musics.find(key) != musics.end())
		return musics[key];
	LOGW("Music with key \"%d\" not found.", (int)key);
	return NULL;
}

AgkImage* MediaBank::addImage(char* path, ASSET key, bool magFilterNearest) {
	if (images.find(key) == images.end()) {
		AgkImage* img = new AgkImage(path);
		if (magFilterNearest)
			agk::SetImageMagFilter(img->getID(), 0);
		images[key] = img;
		return img;
	}
	else {
		LOGW("MediaBank::addImage() - tried to add image with already used key.");
		return NULL;
	}
}

AgkImage* MediaBank::addImage(const uString& path, const uString& key, bool magFilterNearest) {
	std::string keystr = std::string(key.GetStr());
	if (customImages.find(keystr) == customImages.end()) {
		AgkImage* img = new AgkImage((char*)path.GetStr());
		if (magFilterNearest)
			agk::SetImageMagFilter(img->getID(), 0);
		customImages[keystr] = img;
		return img;
	}
	else {
		LOGW("MediaBank::addImage() - tried to add image with already used key.");
		return NULL;
	}
}

void MediaBank::addShader(char* path, ASSET key) {
	if (shaders.find(key) == shaders.end()) {
		int shaderID = agk::LoadSpriteShader(path);
		shaders[key] = shaderID;
	}
	else
		LOGW("MediaBank::addShader() - tried to add shader with already used key.");
}

void MediaBank::addFont(char* path, ASSET key) {
	if (fonts.find(key) == fonts.end()) {
		AgkFont* font = new AgkFont(path);
		fonts[key] = font;
	}
	else
		LOGW("MediaBank::addFont() - tried to add font with already used key.");
}

void MediaBank::addSound(char* path, ASSET key, bool isOGG) {
	if (sounds.find(key) == sounds.end()) {
		AgkSound* sound = new AgkSound(path, isOGG);
		sounds[key] = sound;
	}
	else
		LOGW("MediaBank::addSound() - tried to add sound with already used key.");
}

void MediaBank::addMusic(char* path, ASSET key, bool isOGG) {
	if (musics.find(key) == musics.end()) {
		AgkMusic* music = new AgkMusic(path, isOGG);
		musics[key] = music;
	}
	else
		LOGW("MediaBank::addMusic() - tried to add sound with already used key.");
}


void MediaBank::loadExternalMedia() {
	loadAnimations();
	loadShaders();
}

void MediaBank::loadAnimations() {
	//TODO: move animation reading and shader reading in one common JSON reader: "MediaJSONreader"
	AnimationJSONReader animationReader;
	std::string errmsg = "";
	if (!animationReader.readAnimations(&animSprites, &animImages, ANIMATIONS_JSON_FILE, errmsg)) {
		//handle error:
	}
}

void MediaBank::loadShaders() {
	//TODO: move animation reading and shader reading in one common JSON reader: "MediaJSONreader"
	MediaJSONReader mediaReader;
	std::string errmsg = "";
	if (!mediaReader.readMedia(&customShaders, SHADERS_JSON_FILE, errmsg)) {
		//handle error:
	}
}

void MediaBank::playAnimation(const std::string& id, const glm::vec2& pos, int fps) {
	Sprite* s = animSprites[id];
	s->setPosition(pos);
	s->playSprite((float)fps, 0);
	s->show();
}
Sprite* MediaBank::getAnimationSprite(const std::string& id) {
	return animSprites[id];
}

Sprite* MediaBank::createAnimationSpriteCopy(const std::string& id, const glm::vec2& size) {
	Sprite* s = new Sprite(getAnimationSprite(id));
	s->setSize(size);
	s->setAlignment(AgkHAlign::center, AgkVAlign::center); //This is just a quick fix... When the sprite is copied, it is no longer aligned apperently.
	return s;
}