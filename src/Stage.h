#pragma once
#include <graphx.h>
#include "Projectile.h"
#include "Animation.h"
#include "Hitbox.h"
#include "Solid.h"

enum { finalDestination };

struct Stage
{
	int id;
	vector<Solid> solids;
	vector<Hitbox> hboxes;
	int bgColor;

	void render()
	{
		gfx_FillScreen(bgColor);
		for (int i = 0; i < (int)solids.size(); i++)
		{
			//if (solids[i].anim.frames.size())
				solids[i].render();
		}
	}
};

static Stage s;