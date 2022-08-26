#pragma once
#include <graphx.h>
#include "Animation.h"
#include "Projectile.h"
#include "Hitbox.h"
#include "Stage.h"
#include "Solid.h"
#include <TINYSTL/vector.h>
#include <string.h>

using namespace tinystl;

enum { idle, shield, shieldbroken, dodge, spotdodge, airdodge, freefall, attackneutral, smashcharge, attacksmash, attackspecial, attackrecovery };

enum { tetris };

struct Player
{
	int cpuLevel; // 0 if human.
	int playerNumber;
	float xpos, ypos, xvel, yvel;
	float hboxx, hboxy;
	int damage = 0;
	float shieldDamage = 0;
	int moveTimer = 0;
	int lagTimer = 0;
	bool facingLeft = false;
	bool airborne;
	bool intangible = false;
	int jumps, maxJumps = 2;
	int releasedTime[9] = { 120, 120, 120, 120, 120, 120, 120, 120, 120 };
	int heldTime[9] = { 120, 120, 120, 120, 120, 120, 120, 120, 120 };
	int state = idle;
	int currentFrame = 0;
	int currentAnim = 0;
	char* name;
	vector<Animation> anims;
	vector<Projectile> projs;
	vector<Hitbox> hboxes;
	vector<float> characterVars;

	void update(bool keyA, bool keyB, bool keyG, bool keyJ, bool keyS, bool keyDown, bool keyLeft, bool keyRight, bool keyUp)
	{
		if (intangible)
			intangible = !(keyA || keyB || keyG || keyJ || keyS || keyDown || keyLeft || keyRight || keyUp);
		switch (state)
		{
		default:
		case idle:
			if (lagTimer > 0)
				lagTimer--;
			else
			{
				xvel += (2 + !airborne) * (keyRight - keyLeft);
				if (keyJ && releasedTime[3] && (!airborne || jumps > 0))
				{
					yvel = -20;
					jumps--;
					airborne = true;
				}
				shieldDamage -= (shieldDamage > 0);
				if (keyS && shieldDamage < 600)
				{
					if (airborne)
					{
						state = airdodge;
						currentFrame = 0;
						moveTimer = 10;
						lagTimer = 6;
						intangible = true;
					}
					else
					{
						state = shield;
						currentFrame = 0;
						xvel = 0;
						yvel = 0;
					}
				}
				else if (keyA && releasedTime[0])
				{
					if (!airborne && keyDown && heldTime[5] <= 30)
					{
						state = smashcharge;
						currentAnim = 8;
						currentFrame = 0;
						xvel = 0;
						moveTimer = 180;
					}
					else
					{
						state = attackneutral;
						currentAnim = 7 - facingLeft;
						currentFrame = 0;
						xvel *= airborne;
						moveTimer = 15;
						lagTimer = 10;
					}
				}
				else if (keyB && releasedTime[1])
				{
					if (keyUp)
					{
						state = attackrecovery;
						currentAnim = 11;
						currentFrame = 0;
						moveTimer = 45;
						lagTimer = 6;
						yvel = 0;
					}
					else
					{
						state = attackspecial;
						currentAnim = 10;
						currentFrame = 0;
						moveTimer = 15;
						lagTimer = 10;
						yvel = -20;
					}
				}
				else if (keyG)
				{

				}

				if (!airborne && (xvel > 7 || xvel < -7))
					facingLeft = xvel < 0;
			}
			break;
		case shield:
			if (keyS)
			{
				if (shieldDamage < 600)
					shieldDamage++;
				else
				{
					state = shieldbroken;
					currentAnim = 3;
					currentFrame = 0;
					yvel = -10;
				}
				if (keyLeft || keyRight || keyDown)
				{
					if (keyDown)
					{
						state = spotdodge;
						moveTimer = 15;
						lagTimer = 10;
						intangible = true;
					}
					else
					{
						state = dodge;
						moveTimer = 15;
						lagTimer = 6;
						facingLeft = keyRight;
						intangible = true;
					}
					currentFrame = 0;
				}
			}
			else
			{
				state = idle;
				currentFrame = 0;
			}
			break;
		case shieldbroken:
			if (shieldDamage)
				shieldDamage--;
			else
			{
				state = idle;
				currentFrame = 0;
			}
			break;
		case dodge:
			xvel = 14 * facingLeft - 7;
		case spotdodge:
			moveTimer--;
			if (moveTimer <= 0)
			{
				moveTimer = 0;
				state = idle;
				intangible = false;
			}
			break;
		case airdodge:
			xvel = 6 * (keyRight - keyLeft);
			yvel = 14 * (keyDown - keyUp);
			moveTimer--;
			if (moveTimer <= 0)
			{
				moveTimer = 0;
				state = freefall;
				currentFrame = 0;
				intangible = false;
			}
			if (!airborne)
			{
				lagTimer = 0;
				state = shield;
				intangible = false;
			}
			break;
		case freefall:
			xvel += 2 * (keyRight - keyLeft);
			if (!airborne)
			{
				state = idle;
				currentFrame = 0;
			}
			break;
		case attackneutral:
			moveTimer--;
			if (moveTimer <= 0)
			{
				moveTimer = 0;
				state = idle;
			}
			break;
		case smashcharge:
			moveTimer--;
			if (moveTimer <= 0 || !keyA)
			{
				moveTimer = 30;
				lagTimer = 10;
				state = attacksmash;
				currentAnim = 9;
				currentFrame = 0;
			}
			break;
		case attacksmash:
			moveTimer--;
			if (moveTimer <= 0)
			{
				moveTimer = 0;
				state = idle;
			}
			break;
		case attackspecial:
			if (airborne)
				yvel++;
			else
				moveTimer--;
			if (moveTimer <= 0 || (keyB && releasedTime[1]))
			{
				moveTimer = 0;
				state = idle;
			}
			break;
		case attackrecovery:
			moveTimer--;
			xvel += keyRight - keyLeft;
			yvel -= 1 + (yvel > -14);
			if (moveTimer <= 0)
			{
				moveTimer = 0;
				state = freefall;
				currentFrame = 0;
			}
		}

		xpos += xvel / 3;
		ypos += yvel / 7;
		if (state != attackneutral)
			xvel *= 0.75;
		yvel++;
		airborne = true;

		if (!gfx_CheckRectangleHotspot(xpos, ypos, hboxx - 1, hboxy - 1, 0, 0, 320, 240))
		{
			xpos = 107;
			ypos = 75;
			xvel = 0;
			yvel = 0;
			jumps = maxJumps - 1;
			damage = 0;
			shieldDamage = 0;
			currentFrame = 0;
			state = idle;
			facingLeft = false;
			intangible = true;
		}
		if (damage > 9999)
			damage = 9999;
		if (damage < 0)
			damage = 0;

		collide(keyDown && releasedTime[5] <= 40 && releasedTime[5] > 0);
		bool keys[9] = { keyA, keyB, keyG, keyJ, keyS, keyDown, keyLeft, keyRight, keyUp };
		for (int i = 0; i < 9; i++)
		{
			releasedTime[i]++;
			releasedTime[i] *= !keys[i];
			heldTime[i]++;
			heldTime[i] *= keys[i];
		}
	}

	void collide(bool drop)
	{
		for (int i = 0; i < (int)s.solids.size(); i++)
		{
			if (gfx_CheckRectangleHotspot(xpos, ypos, hboxx - 1, hboxy - 1, s.solids[i].xpos, s.solids[i].ypos, s.solids[i].hboxx, s.solids[i].hboxy))
			{
				if (s.solids[i].semiSolid)
				{
					if (ypos <= s.solids[i].ypos && ypos + hboxy <= s.solids[i].ypos + s.solids[i].hboxy && yvel > 0)
					{
						if (drop)
							ypos += 7;
						else
						{
							ypos -= (yvel - 1) / 7;
							yvel--;
							airborne = false;
							jumps = maxJumps;
						}
					}
				}
				else
				{
					if (xpos <= s.solids[i].xpos && xvel > 0)
					{
						xpos -= 2 + xvel / 7;
						xvel = 0;
					}
					if (xpos + hboxx > s.solids[i].xpos + s.solids[i].hboxx && xvel < 0)
					{
						xpos -= -2 + xvel / 7;
						xvel = 0;
					}
					if (ypos <= s.solids[i].ypos && yvel > 0)
					{
						ypos -= (yvel - 1) / 7;
						yvel = 0;
						airborne = false;
						jumps = maxJumps;
					}
					else if (ypos + hboxy > s.solids[i].ypos + s.solids[i].hboxy && yvel < 0)
					{
						ypos -= (yvel - 1) / 7;
						yvel = 0;
					}
				}
			}
		}
	}

	void render()
	{
		gfx_TransparentSprite(anims[0].frames[0], 64 * playerNumber + 6, 212 - hboxy / 2);
		gfx_SetTextXY(64 * playerNumber, 215);
		gfx_SetTextBGColor(playerNumber);
		gfx_PrintInt(damage / 10, 1);
		gfx_PrintChar('.');
		gfx_PrintInt((damage % 10), 1);
		gfx_PrintChar('%');

		switch (state)
		{
		case idle:
			currentAnim = facingLeft + 4 * intangible;
			break;
		case shield:
			currentAnim = 2;
			gfx_TransparentSprite(anims[2].frames[currentFrame / anims[2].ticksPerFrame], xpos + anims[2].xOffset, ypos + anims[2].yOffset);
			gfx_SetColor(playerNumber);
			gfx_FillCircle(xpos + hboxx / 2 - anims[2].xOffset, ypos + hboxy / 2, 5 - shieldDamage / 60 + (hboxx - anims[2].xOffset) / 2);
			break;
		case dodge:
			currentAnim = 4 + facingLeft;
			break;
		case spotdodge:
		case airdodge:
		case freefall:
			currentAnim = 4 + facingLeft;
			break;
		}
		if (state != shield)
			gfx_TransparentSprite(anims[currentAnim].frames[currentFrame / anims[currentAnim].ticksPerFrame], xpos + anims[currentAnim].xOffset, ypos + anims[currentAnim].yOffset);
		currentFrame++;
		currentFrame *= (currentFrame < anims[currentAnim].ticksPerFrame* (int)anims[currentAnim].frames.size());
	}
};