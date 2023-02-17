#pragma once
#include "Projectile.h"
#include "gfx/oiramgfx.h"

struct OiramFire : public Projectile
{
	void update()
	{
		duration--;
		currentFrame++;

		xpos += xvel / 7;
		ypos += yvel / 7;
		yvel += 2;

		for (int i = 0; i < (int)s.solids.size(); i++)
		{
			if (gfx_CheckRectangleHotspot(xpos, ypos, 7, 7, s.solids[i].xpos, s.solids[i].ypos, s.solids[i].hboxx, s.solids[i].hboxy))
				yvel *= -1;
		}

		Hitbox h;
		h.damage = 16;
		h.knockback = 9;
		h.x1 = xpos;
		h.x2 = 8;
		h.y1 = ypos;
		h.y2 = 8;
		h.team = team;
		s.hboxes.push_back(h);
	}

	void loadSprites()
	{
		Animation right;
		right.frames.push_back(fireright1);
		right.frames.push_back(fireright2);
		right.ticksPerFrame = 10;

		Animation left;
		left.frames.push_back(fireleft1);
		left.frames.push_back(fireleft2);
		left.ticksPerFrame = 10;

		hboxx = 8;
		hboxy = 8;

		anims.push_back(right);
		anims.push_back(left);
	}
};