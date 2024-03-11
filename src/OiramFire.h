#pragma once
#include "Projectile.h"

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
		h.damage = 50;
		h.knockback = 9;
		h.x1 = xpos;
		h.x2 = 8;
		h.y1 = ypos;
		h.y2 = 8;
		h.team = team;
		h.iFrames = 30;
		h.hitStun = 20;
		s.hboxes.push_back(h);
	}

	void loadSprites()
	{
		Animation right;
		right.frames.push_back(oiramfireright1);
		right.frames.push_back(oiramfireright2);
		right.ticksPerFrame = 10;

		Animation left;
		left.frames.push_back(gfx_FlipSpriteY(oiramfireright1, gfx_MallocSprite(8, 8)));
		left.frames.push_back(gfx_FlipSpriteY(oiramfireright2, gfx_MallocSprite(8, 8)));
		left.ticksPerFrame = 10;

		hboxx = 8;
		hboxy = 8;

		anims.push_back(right);
		anims.push_back(left);
	}
};