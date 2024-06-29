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
	int width, height;
	char* name;

	Animation(int w, int h, int x = 0, int y = 0)
	{
		width = w;
		height = h;
		xOffset = x;
		yOffset = y;
	}
};