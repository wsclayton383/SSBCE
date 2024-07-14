#pragma once
#include "Projectile.h"

struct AlexisDagger : public Projectile
{
	void update()
	{
		duration--;

		xpos += xvel / 7;

		Hitbox h;
		h.damage = 50;
		h.knockback = 9;
		h.x1 = xpos;
		h.x2 = 14;
		h.y1 = ypos;
		h.y2 = 8;
		h.team = team;
		h.iFrames = 30;
		h.hitStun = 20;
		s.hboxes.push_back(h);
	}

	void loadSprites()
	{
		Animation right(14, 8);
		right.frames.push_back(alexisdaggerright1);

		Animation left(14, 8);
		left.frames.push_back(gfx_FlipSpriteY(alexisdaggerright1, gfx_MallocSprite(14, 8)));

		hboxx = 14;
		hboxy = 8;

		anims.push_back(right);
		anims.push_back(left);
	}
};