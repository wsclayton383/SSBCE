#pragma once
#include <graphx.h>
#include "Animation.h"

struct Solid
{
	float xpos, ypos, hboxx, hboxy;
	bool semiSolid, moving;
	int moveIndex = 0;
	vector<int> moveX;
	vector<int> moveY;
	int speed = 1;
	int color;
	Animation anim;
	int currentFrame = 0;

	void render()
	{
		gfx_SetColor(color);
		gfx_FillRectangle(xpos, ypos, hboxx, hboxy);

		/*gfx_TransparentSprite(anim.frames[currentFrame / anim.ticksPerFrame], xpos, ypos);
		currentFrame++;
		if (currentFrame >= (int)anim.frames.size())
			currentFrame = 0;*/
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

	Solid(float x, float y, float hx, float hy, int c, bool sS = false, bool m = false)
	{
		xpos = x;
		ypos = y;
		hboxx = hx;
		hboxy = hy;
		color = c;
		semiSolid = sS;
		moving = m;
	}
};