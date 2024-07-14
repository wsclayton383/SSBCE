#pragma once
#include "Player.h"
#include "OiramFire.h"

struct Oiram : public Player
{
	OiramFire fireball;
	vector<OiramFire> projs;

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
						currentFrame = 15 * facingLeft;
						moveTimer = 66;
						lagTimer = 6;
						yvel = 0;
					}
					else
					{
						if ((int)projs.size() < 2)
						{
							OiramFire p = fireball;
							p.duration = 120;
							p.team = team;
							p.xpos = xpos + 4;
							p.ypos = ypos + 14;
							p.xvel = 12 * (2 * !facingLeft - 1);
							p.yvel = 9;
							p.currentAnim = facingLeft;
							projs.push_back(p);
							lagTimer = 10;
						}
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
			h.knockback = 6;
			h.x1 = xpos + 10 * !facingLeft;
			h.x2 = 6;
			h.y1 = ypos + 21;
			h.y2 = 6;
			h.team = team;
			h.iFrames = moveTimer;
			h.hitStun = moveTimer;
			h.xKnockback = !facingLeft * 3.0 - 1.5;
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
			h.damage = 150;
			h.knockback = 12;
			h.x1 = xpos - 5 + 19 * (currentFrame >= 15);
			h.x2 = 8;
			h.y1 = ypos + 17;
			h.y2 = 9;
			h.team = team;
			h.iFrames = moveTimer;
			h.hitStun = moveTimer;
			s.hboxes.push_back(h);
			break;
		case attackrecovery:
			moveTimer--;
			xvel += keyRight - keyLeft;
			if (currentFrame % 15 == 1)
				yvel = -14;
			if (moveTimer <= 0)
			{
				moveTimer = 0;
				state = freefall;
				currentFrame = 0;
			}
			h.damage = 50;
			h.knockback = 8;
			h.x1 = xpos - 5 + 19 * (currentFrame >= 15);
			h.x2 = 8;
			h.y1 = ypos + 17;
			h.y2 = 9;
			h.team = team;
			h.iFrames = 10;
			h.hitStun = 20;
			h.setKnockback = true;
			h.xKnockback = 30 * (currentFrame < 15) - 15;
			h.yKnockback = -23;
			s.hboxes.push_back(h);
		}

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
			state = idle;
			facingLeft = (team == 2);
			currentAnim = 5 + facingLeft;
			currentFrame = 0;
			d.active = false;
			intangible = true;
		}
		damage *= damage > 0;

		knockbackMultiplier = 1 + damage / 200.0;

		if (state == idle && abs(xvel) < 7)
			currentFrame = 0;
		currentFrame++;

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
		Animation idleRight(16, 27);
		idleRight.frames.push_back(oiramright1);
		idleRight.frames.push_back(oiramright2);
		idleRight.ticksPerFrame = 10;

		Animation idleLeft(16, 27);
		idleLeft.frames.push_back(gfx_FlipSpriteY(oiramright1, gfx_MallocSprite(16, 27)));
		idleLeft.frames.push_back(gfx_FlipSpriteY(oiramright2, gfx_MallocSprite(16, 27)));
		idleLeft.ticksPerFrame = 10;

		Animation shield(14, 16, 2, 11);
		shield.frames.push_back(oiramshield1);

		Animation shieldBroken(16, 27);
		shieldBroken.ticksPerFrame = 45;

		Animation dodgeLeft(16, 27);
		dodgeLeft.frames.push_back(oiramright2);
		shieldBroken.frames.push_back(oiramright2);

		Animation dodgeRight(16, 27);
		dodgeRight.frames.push_back(idleLeft.frames[1]);
		shieldBroken.frames.push_back(idleLeft.frames[1]);

		Animation neutralLeft(16, 27);
		neutralLeft.frames.push_back(idleLeft.frames[1]);

		Animation neutralRight(16, 27);
		neutralRight.frames.push_back(oiramright2);

		Animation smashCharge(30, 30, -5);
		smashCharge.frames.push_back(oiramsmashcharge1);

		Animation smash(26, 27, -5);
		smash.frames.push_back(gfx_FlipSpriteY(oiramspin1, gfx_MallocSprite(26, 27)));
		smash.frames.push_back(oiramspin1);
		smash.ticksPerFrame = 10;

		Animation special(16, 27);
		special.frames.push_back(oiramright2);

		Animation recovery(26, 27, -5);
		recovery.frames.push_back(smash.frames[0]);
		recovery.frames.push_back(oiramspin1);
		recovery.ticksPerFrame = 15;

		hboxx = 16;
		hboxy = 27;

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

		fireball.loadSprites();

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