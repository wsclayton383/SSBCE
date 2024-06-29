#pragma once
#include "Projectile.h"

struct FoxLaser : public Projectile
{
	void update()
	{
		duration--;

		xpos += xvel / 7;

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

	void render()
	{
		gfx_SetPalette(character_palette, sizeof_character_palette, 0);
		TempSprite* ts = new TempSprite;
		ts->init(xpos, ypos, 15, 3);
		s.tSprites[1].push_back(ts);
		gfx_SetColor(33);
		gfx_FillRectangle(xpos, ypos, 15, 3);
	}

	void loadSprites()
	{
		Animation laser(15, 3);
		laser.ticksPerFrame = 99999;

		hboxx = 15;
		hboxy = 3;

		anims.push_back(laser);
	}
};