#pragma once
#include <graphx.h>
#include "gfx/gfx.h"

struct TempSprite
{
	int xpos, ypos, width, height;
	gfx_sprite_t* sprite;

	TempSprite(int x, int y, int w, int h)
	{
		xpos = x;
		ypos = y;
		sprite = gfx_MallocSprite(w, h);
		gfx_GetSprite(sprite, x, y);
	}

	void render()
	{
		gfx_SetPalette(character_palette, sizeof_character_palette, 0);
		gfx_Sprite(sprite, xpos, ypos);
		free(sprite);
	}
};