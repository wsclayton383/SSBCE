#pragma once
#include "Player.h"

struct Tetris : public Player
{
	void update(bool keyA, bool keyB, bool keyG, bool keyJ, bool keyS, bool keyDown, bool keyLeft, bool keyRight, bool keyUp)
	{
		h = temp;
		hboxes.clear();
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
				if (keyDown && onSemi && (yvel >= 10 || (releasedTime[5] <= 30 && !heldTime[5])))
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
			h.damage = 16;
			h.knockback = 9;
			h.x1 = xpos + 5 * (2 * !facingLeft - 1);
			h.x2 = 11;
			h.y1 = ypos + 5;
			h.y2 = 6;
			h.team = team;
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
			h.damage = 30;
			h.knockback = 12;
			h.x1 = xpos - 5;
			h.x2 = 21;
			h.y1 = ypos + 10;
			h.y2 = 6;
			h.team = team;
			s.hboxes.push_back(h);
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
			if (yvel > 0)
			{
				h.damage = 25;
				h.knockback = 12;
				h.x1 = xpos;
				h.x2 = 11;
				h.y1 = ypos + 5;
				h.y2 = 11;
				h.team = team;
				s.hboxes.push_back(h);
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
			h.damage = 10;
			h.knockback = 8;
			h.x1 = xpos - 3;
			h.x2 = 16;
			h.y1 = ypos + 5;
			h.y2 = 6;
			h.team = team;
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
		damage *= damage > 0;

		knockbackMultiplier = 1 + damage / 200.0;

		currentFrame++;

		bool keys[9] = {keyA, keyB, keyG, keyJ, keyS, keyDown, keyLeft, keyRight, keyUp};
		for (int i = 0; i < 9; i++)
		{
			releasedTime[i]++;
			releasedTime[i] *= !keys[i];
			heldTime[i]++;
			heldTime[i] *= keys[i];
		}
	}

	void loadSprites()
	{
		vector<gfx_sprite_t*> temp;

		temp.push_back(gfx_MallocSprite(50, 50));
		zx0_Decompress(temp[0], tetrisidleright1_compressed);
		Animation idleRight;
		idleRight.frames.push_back(temp[0]);

		temp.push_back(gfx_MallocSprite(50, 50));
		zx0_Decompress(temp[1], tetrisidleleft1_compressed);
		Animation idleLeft;
		idleLeft.frames.push_back(temp[1]);

		temp.push_back(gfx_MallocSprite(50, 50));
		zx0_Decompress(temp[2], tetrisshield1_compressed);
		Animation shield;
		shield.frames.push_back(temp[2]);
		shield.yOffset = 5;

		Animation shieldBroken;
		shieldBroken.ticksPerFrame = 45;

		temp.push_back(gfx_MallocSprite(50, 50));
		zx0_Decompress(temp[3], tetrisdodgeleft1_compressed);
		Animation dodgeLeft;
		dodgeLeft.frames.push_back(temp[3]);
		shieldBroken.frames.push_back(temp[3]);

		temp.push_back(gfx_MallocSprite(50, 50));
		zx0_Decompress(temp[4], tetrisdodgeright1_compressed);
		Animation dodgeRight;
		dodgeRight.frames.push_back(temp[4]);
		shieldBroken.frames.push_back(temp[4]);

		temp.push_back(gfx_MallocSprite(50, 50));
		zx0_Decompress(temp[5], tetrisattackneutralleft1_compressed);
		Animation neutralLeft;
		neutralLeft.frames.push_back(temp[5]);
		neutralLeft.xOffset = -5;
		neutralLeft.yOffset = 5;

		temp.push_back(gfx_MallocSprite(50, 50));
		zx0_Decompress(temp[6], tetrisattackneutralright1_compressed);
		Animation neutralRight;
		neutralRight.frames.push_back(temp[6]);
		neutralRight.yOffset = 5;

		temp.push_back(gfx_MallocSprite(50, 50));
		zx0_Decompress(temp[7], tetrisattacksmashcharge1_compressed);
		Animation smashCharge;
		smashCharge.frames.push_back(temp[7]);
		smashCharge.xOffset = 2;
		smashCharge.yOffset = -5;

		temp.push_back(gfx_MallocSprite(50, 50));
		zx0_Decompress(temp[8], tetrisattacksmash1_compressed);
		Animation smash;
		smash.frames.push_back(temp[8]);
		smash.xOffset = -5;
		smash.yOffset = 10;

		temp.push_back(gfx_MallocSprite(50, 50));
		zx0_Decompress(temp[9], tetrisattackspecial1_compressed);
		Animation special;
		special.frames.push_back(temp[9]);
		special.yOffset = 5;

		temp.push_back(gfx_MallocSprite(50, 50));
		zx0_Decompress(temp[10], tetrisattackrecovery1_compressed);
		temp.push_back(gfx_MallocSprite(50, 50));
		zx0_Decompress(temp[11], tetrisattackrecovery2_compressed);
		Animation recovery;
		recovery.frames.push_back(temp[10]);
		recovery.frames.push_back(temp[11]);
		recovery.xOffset = -3;
		recovery.yOffset = 5;
		recovery.ticksPerFrame = 15;

		hboxx = tetrisidleright1_width;
		hboxy = tetrisidleright1_height;
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

		for (int i = 0; i < (int)temp.size(); i++)
		{
			free(temp[i]);
		}
	}
};