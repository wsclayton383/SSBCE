#pragma once
#include "Animation.h"
#include <TINYSTL/vector.h>
#include <string.h>

using namespace tinystl;

struct Projectile
{
	int duration;
	float xpos, ypos, xvel, yvel;
	char* name;
	vector<Animation> anims;
};