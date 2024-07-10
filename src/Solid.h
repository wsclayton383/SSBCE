#pragma once
#include <graphx.h>
#include "Animation.h"
#include "TempSprite.h"
#include "gfx/gfx.h"
#include "gfx/chargfx.h"

struct Solid
{
	float xpos, ypos, hboxx, hboxy;
	bool semiSolid, moving;
	bool temp = false;
	int duration;
	int moveIndex = 0;
	vector<int> moveX;
	vector<int> moveY;
	int speed = 1;
	int color = 0;
	Animation anim;
	int currentFrame = 0;

	TempSprite* render()
	{
		gfx_SetPalette(character_palette, sizeof_character_palette, 0);
		if (anim.frames.size())
		{
			if (moving || temp)
			{
				TempSprite* ts = new TempSprite;
				ts->init(xpos, ypos, hboxx, hboxy);
				gfx_TransparentSprite(anim.frames[currentFrame / anim.ticksPerFrame], xpos, ypos);
				currentFrame++;
				if (currentFrame >= (int)anim.frames.size())
					currentFrame = 0;
				return ts;
			}

			gfx_TransparentSprite(anim.frames[currentFrame / anim.ticksPerFrame], xpos, ypos);
			currentFrame++;
			if (currentFrame >= (int)anim.frames.size())
				currentFrame = 0;
			return 0;
		}

		if (moving || temp)
		{
			TempSprite* ts = new TempSprite;
			ts->init(xpos, ypos, hboxx, hboxy);
			gfx_SetColor(color);
			gfx_FillRectangle(xpos, ypos, hboxx, hboxy);
			return ts;
		}

		gfx_SetColor(color);
		gfx_FillRectangle(xpos, ypos, hboxx, hboxy);
		return 0;
	}

	void move()
	{
		xpos += speed * ((xpos < moveX[moveIndex]) - (xpos > moveX[moveIndex]));
		ypos += speed * ((ypos < moveY[moveIndex]) - (ypos > moveY[moveIndex]));
		if (xpos == moveX[moveIndex] && ypos == moveY[moveIndex])
		{
			moveIndex++;
			moveIndex *= (moveIndex < (int)moveX.size());
		}
	}

	Solid(float x, float y, float hx, float hy, int c, bool sS, bool m = false)
	{
		xpos = x;
		ypos = y;
		hboxx = hx;
		hboxy = hy;
		color = c;
		semiSolid = sS;
		moving = m;
	}

	Solid(bool t, float hx, float hy, int d, bool sS = false)
	{
		temp = t;
		hboxx = hx;
		hboxy = hy;
		duration = d;
		semiSolid = sS;
	}
};