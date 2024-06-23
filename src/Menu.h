#pragma once
#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include "Button.h"
#include "Player.h"
#include "Tetris.h"
#include "Oiram.h"
#include "Fox.h"
#include "Projectile.h"
#include "Hitbox.h"
#include "Animation.h"
#include "Stage.h"
#include "Solid.h"
#include "gfx/gfx.h"
#include "gfx/menu_palette.h"
#include <TINYSTL/vector.h>
#include <string.h>

using namespace tinystl;

enum { start, charSelect, stageSelect, option, fight };
vector<char> options;
void battle();
void optionMenu();

struct Menu
{
	int id;
	char* name;
	int titleScale;
	vector<Button> p1Buttons;
	vector<Button> p2Buttons;
	int p1Selection = 0;
	int p2Selection = 0;

	Menu(char* mName, int mId, vector<Button> btn, int mScale = 1)
	{
		name = mName;
		id = mId;
		titleScale = mScale;
		for (int i = 0; i < btn.size(); i++)
		{
			switch (btn[i].player)
			{
			default:
			case 1:
				p1Buttons.push_back(btn[i]);
				break;
			case 2:
				p2Buttons.push_back(btn[i]);
			}
		}
	}

	void run();
};

vector<Menu> mList;

void Menu::run()
{
	gfx_SetPalette(menu_palette, sizeof_menu_palette, menu_palette_offset);
	while (!(kb_Data[2] & kb_Alpha || kb_Data[6] & kb_Clear))
	{
		gfx_FillScreen(93);
		gfx_SetTextFGColor(5);
		gfx_SetTextBGColor(255);
		gfx_SetTextScale(titleScale, titleScale);
		gfx_PrintStringXY(name, 1, 1);
		//gfx_SetTextXY(1, 1);
		//gfx_PrintInt(id,2);
		for (int i = 0; i < p1Buttons.size(); i++)
		{
			gfx_SetTextScale(p1Buttons[i].scale, p1Buttons[i].scale);
			if (i == p1Selection)
			{
				gfx_SetTextFGColor(6);
				gfx_SetTextBGColor(5);
				gfx_PrintStringXY(p1Buttons[i].text, p1Buttons[i].xpos, p1Buttons[i].ypos);
				//gfx_SetTextXY(p1Buttons[i].xpos, p1Buttons[i].ypos);
				//gfx_PrintInt(p1Buttons[i].menuLink, 2);
				gfx_ScaledTransparentSprite_NoClip(buttonleftselect, p1Buttons[i].xpos - 8 * p1Buttons[i].scale, p1Buttons[i].ypos, p1Buttons[i].scale, p1Buttons[i].scale);
				gfx_ScaledTransparentSprite_NoClip(buttonrightselect, gfx_GetTextX(), p1Buttons[i].ypos, p1Buttons[i].scale, p1Buttons[i].scale);
			}
			else
			{
				gfx_SetTextFGColor(5);
				gfx_SetTextBGColor(6);
				gfx_PrintStringXY(p1Buttons[i].text, p1Buttons[i].xpos, p1Buttons[i].ypos);
				gfx_ScaledTransparentSprite_NoClip(buttonleft, p1Buttons[i].xpos - 8 * p1Buttons[i].scale, p1Buttons[i].ypos, p1Buttons[i].scale, p1Buttons[i].scale);
				gfx_ScaledTransparentSprite_NoClip(buttonright, gfx_GetTextX(), p1Buttons[i].ypos, p1Buttons[i].scale, p1Buttons[i].scale);
			}
		}
		for (int i = 0; i < p2Buttons.size(); i++)
		{
			gfx_SetTextScale(p2Buttons[i].scale, p2Buttons[i].scale);
			if (i == p2Selection)
			{
				gfx_SetTextFGColor(6);
				gfx_SetTextBGColor(5);
				gfx_PrintStringXY(p2Buttons[i].text, p2Buttons[i].xpos, p2Buttons[i].ypos);
				gfx_ScaledTransparentSprite_NoClip(buttonleftselect, p2Buttons[i].xpos - 8 * p2Buttons[i].scale, p2Buttons[i].ypos, p2Buttons[i].scale, p2Buttons[i].scale);
				gfx_ScaledTransparentSprite_NoClip(buttonrightselect, gfx_GetTextX(), p2Buttons[i].ypos, p2Buttons[i].scale, p2Buttons[i].scale);
			}
			else
			{
				gfx_SetTextFGColor(5);
				gfx_SetTextBGColor(6);
				gfx_PrintStringXY(p2Buttons[i].text, p2Buttons[i].xpos, p2Buttons[i].ypos);
				gfx_ScaledTransparentSprite_NoClip(buttonleft, p2Buttons[i].xpos - 8 * p2Buttons[i].scale, p2Buttons[i].ypos, p2Buttons[i].scale, p2Buttons[i].scale);
				gfx_ScaledTransparentSprite_NoClip(buttonright, gfx_GetTextX(), p2Buttons[i].ypos, p2Buttons[i].scale, p2Buttons[i].scale);
			}
		}
		gfx_SwapDraw();

		kb_Scan();
		if (kb_Data[7] & kb_Down && p1Selection < p1Buttons.size() - 1)
		{
			p1Selection++;
			do
			{
				kb_Scan();
			} while (kb_Data[7] & kb_Down);
		}
		if (kb_Data[7] & kb_Up && p1Selection > 0)
		{
			p1Selection--;
			do
			{
				kb_Scan();
			} while (kb_Data[7] & kb_Up);
		}
		if (kb_Data[4] & kb_5 && p2Selection < p2Buttons.size() - 1)
		{
			p2Selection++;
			do
			{
				kb_Scan();
			} while (kb_Data[4] & kb_5);
		}
		if (kb_Data[4] & kb_8 && p2Selection > 0)
		{
			p2Selection--;
			do
			{
				kb_Scan();
			} while (kb_Data[4] & kb_8);
		}
		if (kb_Data[1] & kb_2nd && p1Buttons.size())
		{
			do
			{
				kb_Scan();
			} while (kb_Data[1] & kb_2nd);
			if (p1Buttons[p1Selection].menuLink == option)
				optionMenu();
			else if (p1Buttons[p1Selection].menuLink == fight)
				battle();
			else if (p1Buttons[p1Selection].menuLink != -1)
				mList[p1Buttons[p1Selection].menuLink].run();
			gfx_SetPalette(menu_palette, sizeof_menu_palette, menu_palette_offset);
		}
	}
	do
	{
		kb_Scan();
	} while (kb_Data[1] & kb_2nd || kb_Data[2] & kb_Alpha || kb_Data[6] & kb_Clear);
}

void battle()
{
	gfx_SetPalette(character_palette, sizeof_character_palette, 0);

	s.solids.clear();

	switch (mList[stageSelect].p1Selection)
	{
	default:
	case 0:
		s.bgColor = 93;
		s.solids.push_back(Solid(80, 135, 160, 26, 16));
		s.solids.push_back(Solid(100, 161, 120, 26, 16));
		break;
	case 1:
		s.bgColor = 93;
		s.solids.push_back(Solid(80, 135, 160, 26, 34));
		s.solids.push_back(Solid(100, 161, 120, 26, 34));
		s.solids.push_back(Solid(100, 104, 40, 5, 17, true));
		s.solids.push_back(Solid(140, 73, 40, 5, 17, true));
		s.solids.push_back(Solid(180, 104, 40, 5, 17, true));
		break;
	case 2:
		s.bgColor = 174;
		s.solids.push_back(Solid(80, 140, 160, 13, 33));
		s.solids.push_back(Solid(40, 110, 40, 5, 33, true, true));
		s.solids[1].moveX.push_back(40);
		s.solids[1].moveX.push_back(120);
		s.solids[1].moveY.push_back(110);
		s.solids[1].moveY.push_back(110);
		s.solids.push_back(Solid(240, 110, 40, 5, 33, true, true));
		s.solids[2].moveX.push_back(240);
		s.solids[2].moveX.push_back(160);
		s.solids[2].moveY.push_back(110);
		s.solids[2].moveY.push_back(110);
		break;
	case 3:
		s.bgColor = 174;
		s.solids.push_back(Solid(64, 110, 192, 5, 33, true));
		s.solids.push_back(Solid(64, 190, 75, 13, 33));
		s.solids.push_back(Solid(181, 190, 75, 13, 33));
		s.solids.push_back(Solid(22, 150, 42, 5, 33, true, true));
		s.solids[3].moveX.push_back(22);
		s.solids[3].moveX.push_back(97);
		s.solids[3].moveY.push_back(150);
		s.solids[3].moveY.push_back(150);
		s.solids.push_back(Solid(256, 150, 42, 5, 33, true, true));
		s.solids[4].moveX.push_back(256);
		s.solids[4].moveX.push_back(181);
		s.solids[4].moveY.push_back(150);
		s.solids[4].moveY.push_back(150);
		s.solids.push_back(Solid(139, 130, 42, 5, 33, false, true));
		s.solids[5].moveX.push_back(139);
		s.solids[5].moveX.push_back(139);
		s.solids[5].moveY.push_back(130);
		s.solids[5].moveY.push_back(235);
		s.solids.push_back(Solid(90, 55, 40, 5, 33, false, true));
		s.solids[6].moveX.push_back(90);
		s.solids[6].moveX.push_back(140);
		s.solids[6].moveX.push_back(190);
		s.solids[6].moveX.push_back(140);
		s.solids[6].moveY.push_back(55);
		s.solids[6].moveY.push_back(105);
		s.solids[6].moveY.push_back(55);
		s.solids[6].moveY.push_back(5);
		break;
	}

	vector<Player*> players;
	switch (mList[charSelect].p1Selection)
	{
	default:
	case tetris:
	{
		Tetris* p1 = new Tetris;
		players.push_back(p1);
		break;
	}
	case oiram:
	{
		Oiram* p1 = new Oiram;
		players.push_back(p1);
		break;
	}
	case fox:
	{
		Fox* p1 = new Fox;
		players.push_back(p1);
		break;
	}
	}
	switch (mList[charSelect].p2Selection)
	{
	default:
	case tetris:
	{
		Tetris* p2 = new Tetris;
		players.push_back(p2);
		break;
	}
	case oiram:
	{
		Oiram* p2 = new Oiram;
		players.push_back(p2);
		break;
	}
	case fox:
	{
		Fox* p2 = new Fox;
		players.push_back(p2);
		break;
	}
	}

	players[0]->team = 1;
	players[0]->xpos = 107 + (players[0]->team == 2) * 95;
	players[0]->ypos = 75;
	players[0]->xvel = 0;
	players[0]->yvel = 0;
	players[0]->stocks = options[1];
	players[0]->loadSprites();

	players[1]->team = 2;
	players[1]->xpos = 107 + (players[1]->team == 2) * 95;
	players[1]->ypos = 75;
	players[1]->xvel = 0;
	players[1]->yvel = 0;
	players[1]->facingLeft = true;
	players[1]->stocks = options[1];
	players[1]->loadSprites();

	//free(finalDes);

	gfx_SetTextFGColor(5);
	gfx_SetTextBGColor(255);
	for (int i = 3; i > 0; i--)
	{
		s.render();
		gfx_SetTextXY(156, 116);
		gfx_PrintInt(i, 1);
		gfx_SwapDraw();
		delay(1000);
	}
	s.render();
	gfx_PrintStringXY("Fight!", 138, 116);
	gfx_SwapDraw();
	delay(1000);

	s.render();
	vector<TempSprite> ts;
	s.tSprites.push_back(ts);
	gfx_SwapDraw();
	s.render();
	s.tSprites.push_back(ts);
	gfx_SwapDraw();

	int frame = 0;
	int frameSkip = 3;
	int tps = 0;
	int targetTPS = 60;
	bool debug = false;
	int t = 0;
	timer_Enable(1, TIMER_32K, TIMER_NOINT, TIMER_UP);
	timer_Enable(2, TIMER_32K, TIMER_NOINT, TIMER_UP);
	timer_Enable(3, TIMER_32K, TIMER_NOINT, TIMER_UP);
	gfx_SetTextXY(10, 10);
	timer_Set(1, 0);
	do
	{
		if ((int)timer_Get(1) >= 32768 / targetTPS)
		{
			tps = 32768 / timer_Get(1);
			timer_Set(1, 0);
			kb_Scan();
			if (kb_Data[1] & kb_Yequ && options[1])
				debug = true;
			if (kb_Data[1] & kb_Window)
				debug = false;
			players[0]->damage += (kb_Data[6] & kb_Add) - (kb_Data[6] & kb_Sub);
			s.hboxes.clear();
			players[0]->update(kb_Data[1] & kb_2nd, kb_Data[2] & kb_Alpha, kb_Data[2] & kb_Math, kb_Data[1] & kb_Mode, kb_Data[3] & kb_GraphVar, kb_Data[7] & kb_Down, kb_Data[7] & kb_Left, kb_Data[7] & kb_Right, kb_Data[7] & kb_Up);
			players[1]->update(kb_Data[6] & kb_Power, kb_Data[6] & kb_Div, kb_Data[6] & kb_Mul, kb_Data[5] & kb_Tan, kb_Data[5] & kb_RParen, kb_Data[4] & kb_5, kb_Data[3] & kb_4, kb_Data[5] & kb_6, kb_Data[4] & kb_8);
			s.update();
			players[0]->collide();
			players[1]->collide();
			frame++;
			frameSkip += ((tps >= targetTPS) && (frameSkip < 30)) - ((tps < (targetTPS * 5.0 / 6.0)) && (frameSkip > 1));
			if (frameSkip < 1)
				frameSkip = 1;
			if (frame <= frameSkip || frameSkip == 30)
			{
				timer_Set(2, 0);
				//s.render();
				s.restore();
				players[0]->render();
				players[1]->render();
				if (debug)
				{
					gfx_SetTextXY(10, 10);
					gfx_SetTextFGColor(5);
					gfx_SetTextBGColor(255);
					gfx_PrintInt(tps, 1);
					gfx_SetTextXY(30, 10);
					gfx_PrintInt((1.0 - 1.0 / frameSkip)* tps + 2 * (frameSkip == 30), 1);
					gfx_SetTextXY(50, 10);
					gfx_PrintInt(frameSkip, 1);
					gfx_SetTextXY(70, 10);
					gfx_PrintInt(t, 1);
					if (s.hboxes.size())
					{
						gfx_SetTextXY(10, 20);
						gfx_PrintInt(s.hboxes[0].knockback, 1);
						gfx_SetTextXY(10, 30);
						gfx_PrintInt(s.hboxes[0].xKnockback, 1);
						gfx_SetTextXY(10, 40);
						gfx_PrintInt(s.hboxes[0].yKnockback, 1);
					}
					for (int i = 0; i < (int)players.size(); i++)
					{
						gfx_SetColor(players[i]->team);
						gfx_Rectangle(players[i]->xpos, players[i]->ypos, players[i]->hboxx, players[i]->hboxy);
					}
					gfx_SetColor(5);
					for (int i = 0; i < (int)s.hboxes.size(); i++)
						gfx_Rectangle(s.hboxes[i].x1, s.hboxes[i].y1, s.hboxes[i].x2, s.hboxes[i].y2);
				}
				gfx_SwapDraw();
				if (timer_Get(2) > t)
					t = timer_Get(2);
			}
			else
				frame = 0;
		}
	} while (!(kb_Data[6] & kb_Clear) && players[0]->stocks && players[1]->stocks);

	do
	{
		kb_Scan();
	} while (kb_Data[6] & kb_Clear);

	if (!players[0]->stocks || !players[1]->stocks)
	{
		gfx_SetTextFGColor(5);
		gfx_SetTextBGColor(255);
		s.render();
		gfx_PrintStringXY("Game!", 142, 116);
		gfx_SwapDraw();
		delay(1000);
	}
	for (int i = 0; i < (int)players.size(); i++)
		delete players[i];
}

void optionMenu()
{
	vector<char*> optionsList;
	optionsList.push_back("Option Format Version");
	optionsList.push_back("Stocks: ");
	optionsList.push_back("Debug Mode: ");
	int selection = 1;
	while (!(kb_Data[6] & kb_Clear || kb_Data[2] & kb_Alpha))
	{
		gfx_SetPalette(menu_palette, sizeof_menu_palette, menu_palette_offset);
		gfx_FillScreen(93);
		gfx_SetTextFGColor(5);
		gfx_SetTextBGColor(255);
		gfx_SetTextScale(1, 1);
		gfx_PrintStringXY("Options", 1, 1);
		for (int i = 1; i < (int)optionsList.size(); i++)
		{
			gfx_PrintStringXY(optionsList[i], 9, 8 * i + 1);
			gfx_PrintInt(options[i], 1);
		}
		gfx_PrintStringXY(">", 1, 8 * selection + 1);
		gfx_SwapDraw();

		//Options
		kb_Scan();

		if (kb_Data[7] & kb_Down && selection < (int)optionsList.size() - 1)
		{
			selection++;
			do
			{
				kb_Scan();
			} while (kb_Data[7] & kb_Down);
		}
		if (kb_Data[7] & kb_Up && selection > 1)
		{
			selection--;
			do
			{
				kb_Scan();
			} while (kb_Data[7] & kb_Up);
		}

		if (kb_Data[1] & kb_2nd)
		{
			do
			{
				kb_Scan();
			} while (kb_Data[1] & kb_2nd);

			while (!(kb_Data[1] & kb_2nd || kb_Data[2] & kb_Alpha || kb_Data[6] & kb_Clear))
			{
				gfx_FillScreen(93);
				gfx_SetTextFGColor(5);
				gfx_SetTextBGColor(255);
				gfx_SetTextScale(1, 1);
				gfx_PrintStringXY("Options ", 1, 1);
				for (int i = 1; i < (int)optionsList.size(); i++)
				{
					gfx_SetTextFGColor(5);
					gfx_PrintStringXY(optionsList[i], 9, 8 * i + 1);
					gfx_SetTextFGColor(5 + (i == selection));
					gfx_PrintInt(options[i], 1);
				}
				gfx_SetTextFGColor(5);
				gfx_PrintStringXY(">", 1, 8 * selection + 1);
				gfx_SwapDraw();

				kb_Scan();

				switch (selection)
				{
				case 1:
					if (kb_Data[7] & kb_Up && options[1] < 99)
					{
						options[1]++;
						do
						{
							kb_Scan();
						} while (kb_Data[7] & kb_Up);
					}
					if (kb_Data[7] & kb_Down && options[1] > 1)
					{
						options[1]--;
						do
						{
							kb_Scan();
						} while (kb_Data[7] & kb_Down);
					}
					break;
				case 2:
					if (kb_Data[7] & kb_Up)
						options[selection] = 1;
					if (kb_Data[7] & kb_Down)
						options[selection] = 0;
					break;
				default:
					break;
				}
			}
			do
			{
				kb_Scan();
			} while (kb_Data[1] & kb_2nd || kb_Data[2] & kb_Alpha || kb_Data[6] & kb_Clear);
		}
	}
	do
	{
		kb_Scan();
	} while (kb_Data[6] & kb_Clear || kb_Data[2] & kb_Alpha);
	selection = 0;

	uint8_t optionsFile = ti_Open("SSBCEopt", "w");
	for (int i = 0; i < (int)options.size(); i++)
		ti_PutC(options[i], optionsFile);
	ti_SetArchiveStatus(true, optionsFile);
	ti_Close(optionsFile);
}