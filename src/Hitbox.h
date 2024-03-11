#pragma once
#include <string.h>

struct Hitbox
{
	float x1, y1, x2, y2;
	float knockback;
	int iFrames = 10;
	int hitStun = 10;
	bool setKnockback = false;
	float xKnockback = 0;
	float yKnockback = 0;
	float damage;
	int team;
	char* name;
};