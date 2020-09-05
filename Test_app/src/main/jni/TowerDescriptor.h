#pragma once
#include "UI/AgkImage.h"
#include "MobDescriptor.h"
#include "AreaSpawnBulletTypes.h"
#include <iostream>
#include <string>
/*
	Tower type defines the tower class
	The tower class defines how a tower aims and chooses target
*/
enum class tower_type {
	aim,				//can aim at one specific target
	aimless,			//only shoot in one direction
	multi_dir,			//shoots in multiple directions (can rotate, doesn't aim)
	aoe,				//affect all mobs in a given radius around the tower
	block,				//STILL UNSURE: has circular range...
	
};

/*
	bullet_type defines how a bullet travels (or spawn) from the tower
	to its target(s)
*/
enum class bullet_type { 
	normal,
	follow,
	area_spawn_long,
	area_spawn_instant,
	chain,
	path,
	lazer
};



/*
	bullet_hit_effect_type defines what happens when a bullet hits a target,
	how it affects the affected mobs (or cells)

	- damage_over_timer : specific dmg taken every frame (not debuff)
	- DOT debuff        : debuff taking damage every dT time interval
*/
enum class bullet_hit_effect_type { 
	damage, 
	debuff, 
	teleport, 
	damage_over_time
	//area_damage        //aoe around hit mob
};

/*
	bullet_effec_viz_type defines what sort of visualization appears
	when a bullet hits a mob (or its target)
*/
enum class bullet_effect_vis_type { 
	none, 
	animation 
};

class BulletAoeStats {
public:
	bool  hasAoe = false;
	float aoeReduction = 0.0f;
	float aoeRange = 0.0f;
	bullet_hit_effect_type aoeEffect;
};

class BulletHitAnimationStats {
public:
	std::string animationID = "";
	glm::vec2 size;
	float fps = 0.0f;
};

class BulletValues {

private:
	float v1 = 0.0f, v2 = 0.0f, v3 = 0.0f;
	float w1 = 0.0f, w2 = 0.0f, w3 = 0.0f;
	float z1 = 0.0f, z2 = 0.0f, z3 = 0.0f;
	int	a1 = 0, a2 = 0, a3 = 0;
	std::string s1 = "", s2 = "", s3 = "";
public:
	const float& damage() { return v1; }
	const float& speedFactor() { return v1; }
	const float& speed() { return v2; }
	const float& duration() { return v3; }

	const float& debuffDuration() { return w1; }
	const float& debuffInterval() { return w2; }

	const float& animFPS() { return z1; }
	const float& rotSpeed() { return z2; }
	const float& expandSpeed() { return z3; }

	const float& chainMaxChildren() { return z1; }
	const float& chainMaxNestedLevel() { return z2; }
	const float& chainExpandTime() { return z3; }

	bool lazerEffectOnChainBullet() { return (bool)a3; }

	bool releaseDebuffOnChainEnd() { return (bool)a2; }

	std::string getShaderKey() { return s1; }
	std::string getAnimationKey() { return s1; }

	mob_effect_type			mobEffectType() { return (mob_effect_type)a1; }
	area_spawn_bullet_type	areaSpawnBulletType() { return (area_spawn_bullet_type)a2; }

public:
	void setV(const std::vector<float>& v) {
		v1 = v[0]; v2 = v[1]; v3 = v[2];
	}
	void setW(const std::vector<float>& v) {
		w1 = v[0]; w2 = v[1]; w3 = v[2];
	}
	void setZ(const std::vector<float>& v) {
		z1 = v[0]; z2 = v[1]; z3 = v[2];
	}
	void setA(const std::vector<int>& a) {
		a1 = a[0]; a2 = a[1]; a3 = a[2];		
	}
	void setS(const std::vector<std::string>& s) {
		s1 = s[0]; s2 = s[1]; s3 = s[2];
	}
	void setV1(float _v1) { v1 = _v1; }
	void setV2(float _v2) { v2 = _v2; }
	void setV3(float _v3) { v3 = _v3; }
	void setW1(float _w1) { w1 = _w1; }
	void setW2(float _w2) { w2 = _w2; }
	void setW3(float _w3) { w3 = _w3; }
	void setZ1(float _z1) { z1 = _z1; }
	void setZ2(float _z2) { z2 = _z2; }
	void setZ3(float _z3) { z3 = _z3; }
	void setA1(int _a1) { a1 = _a1; }
	void setA2(int _a2) { a2 = _a2; }
	void setA3(int _a3) { a3 = _a3; }
	void setS1(const std::string& _s1) { s1 = _s1; }
	void setS2(const std::string&  _s2) { s2 = _s2; }
	void setS3(const std::string&  _s3) { s3 = _s3; }
};

/*
	Stats common for all bullets of a tower
*/
class BulletStats {
public:
	BulletStats() {}
	AgkImage*				bulletImage = nullptr;
	AGKShader*				shader		= nullptr;
	glm::vec2				size;



	bullet_type				type;
	bullet_hit_effect_type	effectType;
	bullet_effect_vis_type	effectVisualization;
		
	BulletAoeStats			aoeStats;
	BulletHitAnimationStats hitAnimationStats;
	BulletValues			data;
};

class TowerImages {
public:
	TowerImages() {}
	std::vector<AgkImage*> fixed;
	std::vector<AgkImage*> rotatable;
	std::vector<AgkImage*> icons;
};

class TowerDescriptor
{
public:
	TowerDescriptor() {}
	int			ID = -1;
	uString		name;
	tower_type	towerType;
	TowerImages images;
	BulletStats bulletStats;

	bool canRotate = false;
	
	int		aoe_dir			= 1;		//For multi directional towers
	float	range			= 0.0f;	
	float	rotate_speed	= 0.0f;
	float	reload_time		= 0.0f;
	int		bullet_buffer_size	= 1;
	
	std::vector<int> upgradable;
	float price = 0.0f;
};
