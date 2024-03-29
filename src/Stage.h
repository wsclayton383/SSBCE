#pragma once
#include <graphx.h>
#include "Projectile.h"
#include "Animation.h"
#include "Hitbox.h"
#include "Solid.h"
#include "gfx/gfx.h"

enum { finalDestination, battlefield, move, boo };

struct Stage
{
	int id;
	vector<Solid> solids;
	vector<Hitbox> hboxes;
	int bgColor = 255;

	void render()
	{
		gfx_SetPalette(character_palette, sizeof_character_palette, 0);
		gfx_FillScreen(bgColor);
		for (int i = 0; i < (int)solids.size(); i++)
		{
			//if (solids[i].anim.frames.size())
				solids[i].render();
		}
	}
	void update()
	{
		for (int i = 0; i < (int)solids.size(); i++)
		{
			if (solids[i].moving)
				solids[i].move();
		}
	}
};

static Stage s;