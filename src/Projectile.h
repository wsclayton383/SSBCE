#pragma once
#include "Animation.h"
#include "Hitbox.h"
#include <TINYSTL/vector.h>
#include <string.h>

using namespace tinystl;

struct Projectile
{
	int duration;
	int team;
	float xpos, ypos, xvel, yvel;
	int hboxx, hboxy;
	int currentAnim = 0;
	int currentFrame = 0;
	vector<Animation> anims;

	virtual void loadSprites() {};

	virtual void update() {};

	void render()
	{
		currentFrame *= (currentFrame < anims[currentAnim].ticksPerFrame* (int)anims[currentAnim].frames.size());
		gfx_TransparentSprite(anims[currentAnim].frames[currentFrame / anims[currentAnim].ticksPerFrame], xpos, ypos);
	}
};