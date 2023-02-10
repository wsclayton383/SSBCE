#pragma once
#include <graphx.h>
#include "Animation.h"
#include "Projectile.h"
#include "Hitbox.h"
#include "Stage.h"
#include "Solid.h"
#include "gfx/gfx.h"
#include "gfx/menu_palette.h"
#include <TINYSTL/vector.h>
#include <string.h>
#include <math.h>

using namespace tinystl;

enum { idle, shield, shieldbroken, dodge, spotdodge, airdodge, freefall, attackneutral, smashcharge, attacksmash, attackspecial, attackrecovery };

enum { tetris };

struct Player
{
	int cpuLevel; // 0 if human.
	int team;
	float xpos, ypos, xvel, yvel;
	float hboxx, hboxy;
	int stocks = 3;
	int damage = 0;
	float shieldDamage = 0;
	float knockbackMultiplier = 1;
	int moveTimer = 0;
	int lagTimer = 0;
	bool facingLeft = false;
	bool airborne;
	bool onSemi;
	bool intangible = false;
	int jumps, maxJumps = 2;
	int releasedTime[9] = { 120, 120, 120, 120, 120, 120, 120, 120, 120 };
	int heldTime[9] = { 120, 120, 120, 120, 120, 120, 120, 120, 120 };
	int state = idle;
	int currentFrame = 0;
	int currentAnim = 0;
	char* name;
	gfx_sprite_t* blast;
	vector<Animation> anims;
	vector<Projectile> projs;
	Hitbox h;
	Hitbox temp;
	vector<Hitbox> hboxes;

	virtual int loadSprites() {};

	virtual void setPalette() {};

	virtual void update(bool keyA, bool keyB, bool keyG, bool keyJ, bool keyS, bool keyDown, bool keyLeft, bool keyRight, bool keyUp) {};

	void collide()
	{
		for (int i = 0; i < (int)s.hboxes.size(); i++)
		{
			if (!intangible && s.hboxes[i].team != team && gfx_CheckRectangleHotspot(xpos, ypos, hboxx - 1, hboxy - 1, s.hboxes[i].x1, s.hboxes[i].y1, s.hboxes[i].x2, s.hboxes[i].y2))
			{
				if (state == shield)
				{
					shieldDamage += s.hboxes[i].damage;
				}
				else
				{
					damage += s.hboxes[i].damage;
					float x = (2.0 * xpos + hboxx) / 2.0 - (2.0 * s.hboxes[i].x1 + s.hboxes[i].x2) / 2.0;
					float y = (2.0 * ypos + hboxy) / 2.0 - (2.0 * s.hboxes[i].y1 + s.hboxes[i].y2) / 2.0;
					xvel = x / pow(pow(x, 2.0) + pow(y, 2.0), 0.5);
					yvel = y / pow(pow(x, 2.0) + pow(y, 2.0), 0.5);
					xvel *= knockbackMultiplier * s.hboxes[i].knockback;
					yvel *= knockbackMultiplier * s.hboxes[i].knockback;
					moveTimer = 0;
					lagTimer = 10;
					state = idle;
				}
			}
		}

		for (int i = 0; i < (int)s.solids.size(); i++)
		{
			if (gfx_CheckRectangleHotspot(xpos, ypos, hboxx - 1, hboxy - 1, s.solids[i].xpos, s.solids[i].ypos, s.solids[i].hboxx, s.solids[i].hboxy))
			{
				onSemi = false;
				if (s.solids[i].semiSolid)
				{
					if (ypos <= s.solids[i].ypos && ypos + hboxy <= s.solids[i].ypos + s.solids[i].hboxy && yvel > 0)
					{
						ypos -= (yvel - 1) / 7;
						yvel = 0;
						airborne = false;
						onSemi = true;
						jumps = maxJumps;
						if (s.solids[i].moving)
						{
							xpos += (1 + s.solids[i].speed) * ((s.solids[i].xpos < s.solids[i].moveX[s.solids[i].moveIndex]) - (s.solids[i].xpos > s.solids[i].moveX[s.solids[i].moveIndex]));
							ypos += s.solids[i].speed * ((s.solids[i].ypos < s.solids[i].moveY[s.solids[i].moveIndex]) - (s.solids[i].ypos > s.solids[i].moveY[s.solids[i].moveIndex]));
						}
					}
				}
				else
				{
					if (xpos <= s.solids[i].xpos && xvel > 0 && !s.solids[i].moving)
					{
						xpos -= (xvel + 1) / 7;
						xvel = 0;
					}
					if (xpos + hboxx > s.solids[i].xpos + s.solids[i].hboxx && xvel < 0 && !s.solids[i].moving)
					{
						xpos -= (xvel - 1) / 7;
						xvel = 0;
					}
					if (ypos <= s.solids[i].ypos && yvel > 0)
					{
						ypos -= (yvel - 1) / 7;
						//ypos = s.solids[i].ypos - hboxy;
						yvel = 0;
						airborne = false;
						jumps = maxJumps;
					}
					else if (ypos + hboxy > s.solids[i].ypos + s.solids[i].hboxy && yvel < 0)
					{
						ypos -= (yvel - 1) / 7;
						yvel = 0;
					}
					if (s.solids[i].moving)
					{
						xpos += s.solids[i].speed * ((s.solids[i].xpos < s.solids[i].moveX[s.solids[i].moveIndex]) - (s.solids[i].xpos > s.solids[i].moveX[s.solids[i].moveIndex]));
						ypos += s.solids[i].speed * ((s.solids[i].ypos < s.solids[i].moveY[s.solids[i].moveIndex]) - (s.solids[i].ypos > s.solids[i].moveY[s.solids[i].moveIndex]));
					}
				}
			}
		}
	}

	void render()
	{
		setPalette();
		//gfx_TransparentSprite(anims[0].frames[0], 64 * team + 6, 212 - hboxy / 2);
		gfx_SetTextXY(72 * team - 40, 215);
		gfx_SetTextBGColor(team);
		gfx_PrintInt(damage / 10, 1);
		gfx_PrintChar('.');
		gfx_PrintInt((damage % 10), 1);
		gfx_PrintChar('%');
		gfx_SetTextXY(72 * team - 32, 224);
		gfx_PrintInt(stocks, 1);

		currentFrame *= (currentFrame < anims[currentAnim].ticksPerFrame * (int)anims[currentAnim].frames.size());
		gfx_TransparentSprite(anims[currentAnim].frames[currentFrame / anims[currentAnim].ticksPerFrame], xpos + anims[currentAnim].xOffset, ypos + anims[currentAnim].yOffset);
		if (state == shield)
		{
			gfx_SetColor(team);
			gfx_FillCircle_NoClip(xpos + hboxx / 2 - anims[2].xOffset, ypos + hboxy / 2, 5 - shieldDamage / 60 + (hboxx - anims[2].xOffset) / 2);
		}

		for (int i = 0; i < (int)projs.size(); i++)
		{
			//projs[i].render();
		}

		//gfx_SetPalette(menu_palette, sizeof_menu_palette, 0);
	}
};