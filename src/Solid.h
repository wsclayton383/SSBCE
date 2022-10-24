#pragma once
#include <graphx.h>
#include "Animation.h"

struct Solid
{
	float xpos, ypos, hboxx, hboxy;
	bool semiSolid;
	Animation anim;
	int currentFrame = 0;

	void render()
	{
		if (semiSolid)
			gfx_SetColor(17);
		else
			gfx_SetColor(34);
		gfx_FillRectangle(xpos, ypos, hboxx, hboxy);

		/*gfx_TransparentSprite(anim.frames[currentFrame / anim.ticksPerFrame], xpos, ypos);
		currentFrame++;
		if (currentFrame >= (int)anim.frames.size())
			currentFrame = 0;*/
	}

	Solid(float x, float y, float hx, float hy, bool sS = false)
	{
		xpos = x;
		ypos = y;
		hboxx = hx;
		hboxy = hy;
		semiSolid = sS;
	}
};