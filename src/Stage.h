#pragma once
#include <graphx.h>
#include "Animation.h"
#include "Hitbox.h"
#include "Solid.h"
#include "TempSprite.h"
#include "gfx/gfx.h"
#include "gfx/chargfx.h"

enum { finalDestination, battlefield, move, boo };

struct Stage
{
	int id;
	vector<Solid> solids;
	vector<Hitbox> hboxes;
	vector<vector<TempSprite*>> tSprites;
	int bgColor = 255;

	void render()
	{
		gfx_SetPalette(character_palette, sizeof_character_palette, 0);
		gfx_FillScreen(bgColor);
		for (int i = 0; i < (int)solids.size(); i++)
		{
			//if (solids[i].anim.frames.size())
			if (!solids[i].moving)
				solids[i].render();
		}
	}
	void restore()
	{
		for (int i = tSprites[0].size() - 1; i >= 0; i--)
		{
			tSprites[0][i]->render();
			delete tSprites[0][i];
		}
		tSprites[0] = tSprites[1];
		tSprites[1].clear();
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