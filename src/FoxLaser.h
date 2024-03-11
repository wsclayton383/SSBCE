#pragma once
#include "Projectile.h"

struct FoxLaser : public Projectile
{
	void update()
	{
		duration--;

		xpos += xvel / 7;
		ypos += yvel / 7;

		Hitbox h;
		h.damage = 10;
		h.knockback = 0;
		h.x1 = xpos;
		h.x2 = 15;
		h.y1 = ypos;
		h.y2 = 3;
		h.team = team;
		s.hboxes.push_back(h);
	}

	void loadSprites()
	{
		Animation laser;
		laser.frames.push_back(foxlaser1);
		laser.ticksPerFrame = 99999;

		hboxx = 15;
		hboxy = 3;

		anims.push_back(laser);
	}
};