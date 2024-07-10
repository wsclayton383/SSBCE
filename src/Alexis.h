#pragma once
#include "Player.h"
#include "Stage.h"
#include "Solid.h"

struct Alexis : public Player
{
	vector<Projectile> projs;
	Solid block = Solid(true, 16, 16, 180, true);
	int blockTimer = 0;

	void update(bool keyA, bool keyB, bool keyG, bool keyJ, bool keyS, bool keyDown, bool keyLeft, bool keyRight, bool keyUp)
	{
		h = tempH;
		intangible *= !(keyA || keyB || keyG || keyJ || keyS || keyDown || keyLeft || keyRight || keyUp);
		switch (state)
		{
		default:
		case idle:
			currentAnim = facingLeft + 4 * intangible;
			if (lagTimer > 0)
				lagTimer--;
			else
			{
				xvel += (2 + !airborne) * (keyRight - keyLeft);
				if (keyDown && onSemi && (yvel >= 10 || (releasedTime[5] <= 20 && !heldTime[5])))
				{
					ypos += 3;
					if (yvel < 10)
						yvel = 10;
				}
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
						currentFrame = 99999 * facingLeft;
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
					if (keyUp && !blockTimer)
					{
						state = attackrecovery;
						currentAnim = 11;
						currentFrame = 99999 * facingLeft;
						moveTimer = 30;
						lagTimer = 6;
						xvel = 0;
						yvel = 0;
						block.xpos = xpos;
						block.ypos = ypos + 30;
						s.solids.push_back(block);
						blockTimer = 180;
					}
					else
					{
						state = attackspecial;
						currentAnim = 10;
						lagTimer = 10;
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
			currentAnim = 2;
			currentFrame = !facingLeft;
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
			xvel = 28 * facingLeft - 14;
		case spotdodge:
			currentAnim = 4 + facingLeft;
			moveTimer--;
			if (moveTimer <= 0)
			{
				moveTimer = 0;
				state = idle;
				intangible = false;
			}
			break;
		case airdodge:
			currentAnim = 4 + facingLeft;
			xvel = 14 * (keyRight - keyLeft);
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
			currentAnim = 4 + facingLeft;
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
			h.damage = 80;
			h.knockback = 9;
			h.x1 = xpos + 13 * !facingLeft - 8 * facingLeft;
			h.x2 = 11;
			h.y1 = ypos + 14;
			h.y2 = 4;
			h.team = team;
			h.iFrames = moveTimer;
			h.hitStun = moveTimer;
			h.xKnockback = !facingLeft * 4 - 2;
			h.yKnockback = -1;
			s.hboxes.push_back(h);
			break;
		case smashcharge:
			moveTimer--;
			if (moveTimer <= 0 || !keyA)
			{
				moveTimer = 30;
				lagTimer = 10;
				state = attacksmash;
				currentAnim = 9;
				currentFrame = 99999 * facingLeft;

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
		case attackrecovery:
			moveTimer--;
			if (moveTimer <= 0)
			{
				moveTimer = 0;
				state = idle;
				currentAnim = facingLeft;
				currentFrame = 0;
			}
		}

		blockTimer -= (blockTimer > 0);

		xpos += xvel / 7;
		ypos += yvel / 7;
		if (state != attackneutral)
			xvel *= 0.9 - 0.1 * !airborne;
		yvel++;
		airborne = true;

		if (!gfx_CheckRectangleHotspot(xpos, ypos, hboxx - 1, hboxy - 1, 0, 0, 319, 239) || damage > 9999)
		{
			stocks--;
			xpos = 107 + (team == 2) * 95;
			ypos = 75;
			xvel = 0;
			yvel = 0;
			jumps = maxJumps - 1;
			damage = 0;
			shieldDamage = 0;
			currentFrame = 0;
			state = idle;
			facingLeft = (team == 2);
			intangible = true;
		}
		damage *= damage > 0;

		knockbackMultiplier = 1 + damage / 200.0;

		if (state == idle && abs(xvel) < 7)
			currentFrame = 0;
		currentFrame++;
		if (state == idle && abs(xvel) >= 7 && currentFrame >= 45)
			currentFrame = 15;

		bool keys[9] = { keyA, keyB, keyG, keyJ, keyS, keyDown, keyLeft, keyRight, keyUp };
		for (int i = 0; i < 9; i++)
		{
			releasedTime[i]++;
			releasedTime[i] *= !keys[i];
			heldTime[i]++;
			heldTime[i] *= keys[i];
		}

		for (int i = 0; i < (int)projs.size(); i++)
		{
			projs[i].update();
			if (projs[i].duration <= 0)
			{
				for (int j = i; j < (int)projs.size() - 1; j++)
					projs[j] = projs[j + 1];
				projs.pop_back();
			}
		}
	}

	int loadSprites()
	{
		Animation idleRight(16, 30);
		idleRight.frames.push_back(alexisright1);
		idleRight.frames.push_back(alexisright2);
		idleRight.ticksPerFrame = 15;

		Animation idleLeft(16, 30);
		idleLeft.frames.push_back(gfx_FlipSpriteY(alexisright1, gfx_MallocSprite(16, 30)));
		idleLeft.frames.push_back(gfx_FlipSpriteY(alexisright2, gfx_MallocSprite(16, 30)));
		idleLeft.ticksPerFrame = 15;

		Animation shield(16, 30);
		shield.frames.push_back(alexisneutralright1);
		shield.frames.push_back(gfx_FlipSpriteY(alexisneutralright1, gfx_MallocSprite(16, 30)));

		Animation shieldBroken(16, 30);
		shieldBroken.ticksPerFrame = 45;

		Animation dodgeLeft(16, 30);
		dodgeLeft.frames.push_back(alexisneutralright1);
		shieldBroken.frames.push_back(alexisneutralright1);

		Animation dodgeRight(16, 30);
		dodgeRight.frames.push_back(shield.frames[1]);
		shieldBroken.frames.push_back(shield.frames[1]);

		Animation neutralRight(16, 30);
		neutralRight.frames.push_back(alexisneutralright1);

		Animation neutralLeft(16, 30);
		neutralLeft.frames.push_back(shield.frames[1]);

		Animation smashCharge(16, 30);
		smashCharge.frames.push_back(alexisneutralright1);
		smashCharge.frames.push_back(shield.frames[1]);
		smashCharge.ticksPerFrame = 99999;

		Animation smash(16, 30);
		smash.frames.push_back(alexisright2);
		smash.frames.push_back(idleLeft.frames[1]);
		smash.ticksPerFrame = 99999;

		Animation special(16, 30);
		special.frames.push_back(alexisright2);
		special.frames.push_back(idleLeft.frames[1]);

		Animation recovery(28, 30, -6);
		recovery.frames.push_back(alexisrecoveryright1);
		recovery.frames.push_back(gfx_FlipSpriteY(alexisrecoveryright1, gfx_MallocSprite(28, 30)));
		recovery.ticksPerFrame = 99999;

		hboxx = 16;
		hboxy = 30;

		anims.push_back(idleRight);
		anims.push_back(idleLeft);
		anims.push_back(shield);
		anims.push_back(shieldBroken);
		anims.push_back(dodgeLeft);
		anims.push_back(dodgeRight);
		anims.push_back(neutralLeft);
		anims.push_back(neutralRight);
		anims.push_back(smashCharge);
		anims.push_back(smash);
		anims.push_back(special);
		anims.push_back(recovery);

		block.anim.frames.push_back(alexisblock1);

		return 0;
	}

	void setPalette()
	{
		gfx_SetPalette(character_palette, sizeof_character_palette, 0);
	}

	void renderProjs()
	{
		for (int i = 0; i < (int)projs.size(); i++)
			projs[i].render();
	}
};