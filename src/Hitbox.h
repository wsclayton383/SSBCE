#pragma once
#include <string.h>

enum { normal, none, flinching, grab };

struct Hitbox
{
	float xpos, ypos;
	float knockback;
	int knockbackType;
	bool setKnockback;
	float xangle, yangle;
	float damage;
	bool isCircle;
	float sizeR, sizeX, sizeY;
	char* name;
};