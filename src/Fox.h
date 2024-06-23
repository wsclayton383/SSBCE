#pragma once
#include "Player.h"
#include "FoxLaser.h"

struct Fox : public Player
{
	FoxLaser laser;
	vector<FoxLaser> projs;

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
						currentFrame = 5 * 99999 * facingLeft;
						moveTimer = 60;
						lagTimer = 6;
						xvel /= 2;
						yvel = 0;
					}
					else
					{
						if ((int)projs.size() < 3)
						{
							FoxLaser p = laser;
							p.duration = 50;
							p.team = team;
							p.xpos = xpos;
							p.ypos = ypos + 10;
							p.xvel = 56 * (2 * !facingLeft - 1);
							p.yvel = 0;
							p.currentAnim = 0;
							projs.push_back(p);
							lagTimer = 5;
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
			if (currentFrame >= 15)
			{
				h.damage = 150;
				h.knockback = 12;
				h.x1 = xpos - 5;
				h.x2 = 23;
				h.y1 = ypos + 3;
				h.y2 = 10;
				h.team = team;
				h.iFrames = moveTimer;
				h.hitStun = moveTimer;
				h.yKnockback = -2;
				s.hboxes.push_back(h);
			}
			break;
		case attackrecovery:
			if (moveTimer <= 0)
			{
				moveTimer = 0;
				state = freefall;
				currentFrame = 0;
			}
			else if (moveTimer >= 25)
			{
				h.damage = 10;
				h.knockback = 2;
				h.x1 = xpos - 5;
				h.x2 = 32;
				h.y1 = ypos;
				h.y2 = 27;
				h.team = team;
				h.iFrames = 7;
				h.hitStun = 10;
				s.hboxes.push_back(h);
				moveTimer--;
				yvel = 0;
				if (moveTimer == 25)
				{
					currentFrame = 5 + keyLeft - keyRight + 3 * keyUp - 3 * keyDown;
					if (currentFrame == 5)
						currentFrame += 2 * facingLeft - 1;
					currentFrame *= 99999;
				}
			}
			else
			{
				moveTimer--;
				int d = currentFrame / 99999 - 5;
				xvel = 14 * (((d + 9) % 3 == 2) - ((d + 9) % 3 == 1));
				yvel = 14 * ((d < -1) - (d > 1));
				h.damage = 80;
				h.knockback = 1;
				h.x1 = xpos - 5;
				h.x2 = 32;
				h.y1 = ypos;
				h.y2 = 27;
				h.team = team;
				h.iFrames = moveTimer;
				h.hitStun = moveTimer;
				h.xKnockback = xvel;
				h.yKnockback = yvel;
				s.hboxes.push_back(h);
			}
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
		if (state == idle && abs(xvel) >= 7 && currentFrame >=45)
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
		Animation idleRight;
		idleRight.frames.push_back(foxright1);
		idleRight.frames.push_back(foxright2);
		idleRight.frames.push_back(foxright3);
		idleRight.xOffset = -4;
		idleRight.ticksPerFrame = 15;

		Animation idleLeft;
		idleLeft.frames.push_back(gfx_FlipSpriteY(foxright1, gfx_MallocSprite(23, 27)));
		idleLeft.frames.push_back(gfx_FlipSpriteY(foxright2, gfx_MallocSprite(23, 27)));
		idleLeft.frames.push_back(gfx_FlipSpriteY(foxright3, gfx_MallocSprite(23, 27)));
		idleLeft.xOffset = -3;
		idleLeft.ticksPerFrame = 15;

		Animation shield;
		shield.frames.push_back(foxshieldright1);
		shield.frames.push_back(gfx_FlipSpriteY(foxshieldright1, gfx_MallocSprite(21, 27)));
		shield.xOffset = -3;

		Animation shieldBroken;
		shieldBroken.ticksPerFrame = 45;

		Animation dodgeLeft;
		dodgeLeft.frames.push_back(foxright2);
		shieldBroken.frames.push_back(foxright2);

		Animation dodgeRight;
		dodgeRight.frames.push_back(idleLeft.frames[1]);
		shieldBroken.frames.push_back(idleLeft.frames[1]);

		Animation neutralRight;
		neutralRight.frames.push_back(foxneutralright1);

		Animation neutralLeft;
		neutralLeft.frames.push_back(gfx_FlipSpriteY(foxneutralright1, gfx_MallocSprite(24, 23)));
		neutralLeft.xOffset = -8;

		Animation smashCharge;
		smashCharge.frames.push_back(foxsmashchargeright1);
		smashCharge.frames.push_back(gfx_FlipSpriteY(foxsmashchargeright1, gfx_MallocSprite(24, 20)));
		smashCharge.xOffset = -5;
		smashCharge.yOffset = 7;
		smashCharge.ticksPerFrame = 99999;

		Animation smash;
		smash.frames.push_back(foxsmashright1);
		smash.frames.push_back(foxsmashright2);
		smash.frames.push_back(foxsmashright3);
		smash.xOffset = -5;
		smash.yOffset = 3;
		smash.ticksPerFrame = 15;

		Animation special;
		special.frames.push_back(foxright1);

		Animation recovery;
		recovery.frames.push_back(foxrecoveryright1);
		recovery.frames.push_back(foxrecoverydownright2);
		recovery.frames.push_back(gfx_RotateSpriteC(foxrecoveryright2, gfx_MallocSprite(32, 32)));
		recovery.frames.push_back(gfx_FlipSpriteY(foxrecoverydownright2, gfx_MallocSprite(30, 30)));
		recovery.frames.push_back(foxrecoveryright2);
		recovery.frames.push_back(gfx_FlipSpriteY(foxrecoveryright1, gfx_MallocSprite(32, 27)));
		recovery.frames.push_back(gfx_FlipSpriteY(foxrecoveryright2, gfx_MallocSprite(32, 32)));
		recovery.frames.push_back(gfx_FlipSpriteX(foxrecoverydownright2, gfx_MallocSprite(30, 30)));
		recovery.frames.push_back(gfx_RotateSpriteCC(foxrecoveryright2, gfx_MallocSprite(32, 32)));
		recovery.frames.push_back(gfx_FlipSpriteY(recovery.frames[7], gfx_MallocSprite(30, 30)));
		recovery.xOffset = -5;
		recovery.ticksPerFrame = 99999;

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

		laser.loadSprites();

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