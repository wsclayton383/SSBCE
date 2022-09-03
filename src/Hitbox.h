#pragma once
#include <string.h>

enum { normal, none, flinching, grab };

struct Hitbox
{
	float x1, y1, x2, y2;
	float knockback;
	int knockbackType = normal;
	bool setKnockback;
	float xangle, yangle;
	float damage;
	int team;
	char* name;
};