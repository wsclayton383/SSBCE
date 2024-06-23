#pragma once
#include <graphx.h>
#include "Projectile.h"
#include "Animation.h"
#include "Hitbox.h"
#include "Solid.h"
#include "TempSprite.h"
#include "gfx/gfx.h"

enum { finalDestination, battlefield, move, boo };

struct Stage
{
	int id;
	vector<Solid> solids;
	vector<Hitbox> hboxes;
	vector<vector<TempSprite>> tSprites;
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
	void restore()
	{
		gfx_SetPalette(character_palette, sizeof_character_palette, 0);
		for (int i = tSprites[0].size() - 1; i >= 0; i++)
			tSprites[0][i].render();
		tSprites.erase(tSprites.begin());
		vector<TempSprite> ts;
		tSprites.push_back(ts);
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