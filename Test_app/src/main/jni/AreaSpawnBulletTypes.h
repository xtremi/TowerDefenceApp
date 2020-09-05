#pragma once

/*
	Determines how an area-spawn-long or -instant bullet is "spawned" or animated:
*/
enum class area_spawn_bullet_type {
	standard,
	animation,
	expansion,
	rotation
};