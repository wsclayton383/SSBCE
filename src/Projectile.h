#pragma once
#include "Animation.h"
#include <TINYSTL/vector.h>
#include <string.h>

using namespace tinystl;

struct Projectile
{
	int duration;
	float xpos, ypos;
	float xvel = 0;
	float yvel = 0;
	char* name;
	int currentAnim = 0;
	int currentFrame = 0;
	vector<Animation> anims;

	void render()
	{
		currentFrame *= (currentFrame < anims[currentAnim].ticksPerFrame* (int)anims[currentAnim].frames.size());
		gfx_TransparentSprite(anims[currentAnim].frames[currentFrame / anims[currentAnim].ticksPerFrame], xpos, ypos);
	}
};