#pragma once
#include "Player.h"

struct Tetris : public Player
{
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
			h.damage = 80;
			h.knockback = 9;
			h.x1 = xpos + 7 * (2 * !facingLeft - 1);
			h.x2 = 15;
			h.y1 = ypos + 7;
			h.y2 = 8;
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
			h.x1 = xpos - 7;
			h.x2 = 29;
			h.y1 = ypos + 14;
			h.y2 = 8;
			h.team = team;
			h.iFrames = moveTimer;
			h.hitStun = moveTimer;
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
				h.damage = 125;
				h.knockback = 12;
				h.x1 = xpos;
				h.x2 = 15;
				h.y1 = ypos + 7;
				h.y2 = 15;
				h.team = team;
				h.iFrames = 15;
				h.hitStun = 15;
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
			h.damage = 30;
			h.knockback = 1;
			h.x1 = xpos - 4;
			h.x2 = 22;
			h.y1 = ypos + 7;
			h.y2 = 8;
			h.team = team;
			h.iFrames = 7;
			h.hitStun = 20;
			h.setKnockback = true;
			h.xKnockback = 0;
			h.yKnockback = -15;
			s.hboxes.push_back(h);
		}

		xpos += xvel / 7;
		ypos += yvel / 7;
		if (state != attackneutral)
			xvel *= 0.9 - 0.1 * !airborne;
		if (iFrames)
			yvel *= 0.9 - 0.1 * !airborne;
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
	
	int loadSprites()
	{
		Animation idleRight;
		idleRight.frames.push_back(tetrisright1);
		
		Animation idleLeft;
		idleLeft.frames.push_back(tetrisleft1);
		
		Animation shield;
		shield.frames.push_back(tetrisshield1);
		shield.yOffset = 5;

		Animation shieldBroken;
		shieldBroken.ticksPerFrame = 45;
		
		Animation dodgeLeft;
		dodgeLeft.frames.push_back(tetrisdodgeleft1);
		shieldBroken.frames.push_back(tetrisdodgeleft1);
		
		Animation dodgeRight;
		dodgeRight.frames.push_back(tetrisdodgeright1);
		shieldBroken.frames.push_back(tetrisdodgeright1);
		
		Animation neutralLeft;
		neutralLeft.frames.push_back(tetrisattackneutralleft1);
		neutralLeft.xOffset = -7;
		neutralLeft.yOffset = 7;
		
		Animation neutralRight;
		neutralRight.frames.push_back(tetrisattackneutralright1);
		neutralRight.yOffset = 7;
		
		Animation smashCharge;
		smashCharge.frames.push_back(tetrisattacksmashcharge1);
		smashCharge.xOffset = 3;
		smashCharge.yOffset = -7;
		
		Animation smash;
		smash.frames.push_back(tetrisattacksmash1);
		smash.xOffset = -7;
		smash.yOffset = 14;
		
		Animation special;
		special.frames.push_back(tetrisattackspecial1);
		special.yOffset = 7;
		
		Animation recovery;
		recovery.frames.push_back(tetrisattackrecovery1);
		recovery.frames.push_back(tetrisattackrecovery2);
		recovery.xOffset = -4;
		recovery.yOffset = 7;
		recovery.ticksPerFrame = 15;

		hboxx = 15;
		hboxy = 22;
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

		return 0;
	}

	void setPalette()
	{
		gfx_SetPalette(character_palette, sizeof_character_palette, 0);
	}

	void renderProjs() {};
};