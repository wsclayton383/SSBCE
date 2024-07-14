#pragma once
#include "Animation.h"
#include "Hitbox.h"
#include "Stage.h"
#include "TempSprite.h"
#include "gfx/gfx.h"
#include "gfx/chargfx.h"
#include <TINYSTL/vector.h>
#include <string.h>
#include <graphx.h>

using namespace tinystl;

struct Disjoint
{
	int currentAnim = 0;
	int currentFrame = 0;
	bool active = false;
	vector<Animation> anims;
};