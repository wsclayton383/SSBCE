#pragma once
#include <graphx.h>
#include <TINYSTL/vector.h>
#include <string.h>

using namespace tinystl;

struct Animation
{
	vector<gfx_sprite_t*> frames;
	int ticksPerFrame = 1;
	int xOffset = 0;
	int yOffset = 0;
	char* name;
};