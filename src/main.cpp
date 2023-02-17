/*
 *--------------------------------------
 * Program Name: Super Smash Bros. CE
 * Author: William Clayton
 * License: idk man, half this code was stolen anyway
 * Description: Super Smash Bros. for the TI-84+ CE
 *--------------------------------------
*/

#include <tice.h>
#include <stdlib.h>
#include <fileioc.h>
#include <fontlibc.h>
#include <graphx.h>
#include <keypadc.h>
#include <compression.h>
#include <string.h>
#include <TINYSTL/vector.h>
#include "Player.h"
#include "Tetris.h"
#include "Oiram.h"
#include "Projectile.h"
#include "Hitbox.h"
#include "Animation.h"
#include "Stage.h"
#include "Solid.h"

using namespace tinystl;

void battle();
vector<char> options;
int p1Char;
int p2Char;

int main()
{
    gfx_Begin();
    gfx_SetDrawBuffer();

    uint8_t optionsFile = ti_Open("SSBCEopt", "r");
    
    if (optionsFile)
    {
        int i = ti_GetC(optionsFile);
        while (i != EOF)
        {
            options.push_back(i);
            i = ti_GetC(optionsFile);
        }
    }
    while (options.size() < 2)
        options.push_back(0);
    if (options[0] == 0)
        options[0] = 3;
    ti_Close(optionsFile);

    int selection;
    int selection2;
    vector<char*> menuList;
    menuList.push_back("Local");
    menuList.push_back("Link");
    menuList.push_back("Options");
    vector<char*> optionsList;
    optionsList.push_back("Stocks: ");
    optionsList.push_back("Debug Mode: ");
    vector<char*> characterList;
    characterList.push_back("Tetronimo");
    characterList.push_back("Oiram");
    vector<char*> stageList;
    stageList.push_back("Final Destination");
    stageList.push_back("Battlefield");
    stageList.push_back("Move");
    stageList.push_back("Boo");

    while (!(kb_Data[6] & kb_Clear))
    {
        gfx_SetPalette(menu_palette, sizeof_menu_palette, menu_palette_offset);
        gfx_FillScreen(93);
        gfx_TransparentSprite(title, 70, 50);
        gfx_SetTextFGColor(5);
        gfx_SetTextBGColor(255);
        gfx_SetTextScale(1, 1);
        gfx_PrintStringXY("Press 2nd", 127, 187);
        gfx_SwapDraw();
        //Start
        kb_Scan();
        if (kb_Data[1] & kb_2nd)
        {
            do
            {
                kb_Scan();
            } while (kb_Data[1] & kb_2nd);

            selection = 0;
            while (!(kb_Data[6] & kb_Clear || kb_Data[2] & kb_Alpha))
            {
                gfx_SetPalette(menu_palette, sizeof_menu_palette, menu_palette_offset);
                gfx_FillScreen(93);
                gfx_SetTextFGColor(5);
                gfx_SetTextBGColor(255);
                gfx_SetTextScale(4, 4);
                gfx_PrintStringXY("Menu ", 1, 1);
                gfx_SetTextScale(2, 2);
                
                for (int i = 0; i < (int)menuList.size(); i++)
                {
                    if (i == selection)
                    {
                        gfx_SetTextFGColor(6);
                        gfx_SetTextBGColor(5);
                        gfx_PrintStringXY(menuList[i], 17, 20 * i + 36);
                        gfx_ScaledTransparentSprite_NoClip(buttonleftselect, 1, 20 * i + 36, 2, 2);
                        gfx_ScaledTransparentSprite_NoClip(buttonrightselect, gfx_GetTextX(), 20 * i + 36, 2, 2);
                    }
                    else
                    {
                        gfx_SetTextFGColor(5);
                        gfx_SetTextBGColor(6);
                        gfx_PrintStringXY(menuList[i], 17, 20 * i + 36);
                        gfx_ScaledTransparentSprite_NoClip(buttonleft, 1, 20 * i + 36, 2, 2);
                        gfx_ScaledTransparentSprite_NoClip(buttonright, gfx_GetTextX(), 20 * i + 36, 2, 2);
                    }
                }
                gfx_SwapDraw();

                //Menu
                kb_Scan();

                if (kb_Data[7] & kb_Down && selection < (int)menuList.size() - 1)
                {
                    selection++;
                    do
                    {
                        kb_Scan();
                    } while (kb_Data[7] & kb_Down);
                }
                if (kb_Data[7] & kb_Up && selection > 0)
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

                    switch (selection)
                    {
                    default:
                    case 0:
                        selection = 0;
                        selection2 = 0;
                        while (!(kb_Data[6] & kb_Clear || kb_Data[2] & kb_Alpha))
                        {
                            gfx_SetPalette(menu_palette, sizeof_menu_palette, menu_palette_offset);
                            gfx_FillScreen(93);
                            gfx_SetTextFGColor(5);
                            gfx_SetTextBGColor(255);
                            gfx_SetTextScale(1, 1);
                            gfx_PrintStringXY("Character Select", 1, 1);
                            gfx_SetTextBGColor(5);
                            for (int i = 0; i < (int)characterList.size(); i++)
                            {
                                if (i == selection)
                                {
                                    gfx_SetTextFGColor(6);
                                    gfx_SetTextBGColor(5);
                                    gfx_PrintStringXY(characterList[i], 9, 10 * i + 11);
                                    gfx_TransparentSprite(buttonleftselect, 1, 10 * i + 11);
                                    gfx_TransparentSprite(buttonrightselect, gfx_GetTextX(), 10 * i + 11);
                                }
                                else
                                {
                                    gfx_SetTextFGColor(5);
                                    gfx_SetTextBGColor(6);
                                    gfx_PrintStringXY(characterList[i], 9, 10 * i + 11);
                                    gfx_TransparentSprite(buttonleft, 1, 10 * i + 11);
                                    gfx_TransparentSprite(buttonright, gfx_GetTextX(), 10 * i + 11);
                                }
                            }
                            for (int i = 0; i < (int)characterList.size(); i++)
                            {
                                if (i == selection2)
                                {
                                    gfx_SetTextFGColor(6);
                                    gfx_SetTextBGColor(5);
                                    gfx_PrintStringXY(characterList[i], 115, 10 * i + 11);
                                    gfx_TransparentSprite(buttonleftselect, 107, 10 * i + 11);
                                    gfx_TransparentSprite(buttonrightselect, gfx_GetTextX(), 10 * i + 11);
                                }
                                else
                                {
                                    gfx_SetTextFGColor(5);
                                    gfx_SetTextBGColor(6);
                                    gfx_PrintStringXY(characterList[i], 115, 10 * i + 11);
                                    gfx_TransparentSprite(buttonleft, 107, 10 * i + 11);
                                    gfx_TransparentSprite(buttonright, gfx_GetTextX(), 10 * i + 11);
                                }
                            }
                            //gfx_PrintStringXY(">", 1, 8 * selection + 9);
                            //gfx_PrintStringXY(">", 9, 8 * selection2 + 9);
                            gfx_SwapDraw();

                            //Character Select
                            kb_Scan();
                            if (kb_Data[7] & kb_Down && selection < (int)characterList.size() - 1)
                            {
                                selection++;
                                do
                                {
                                    kb_Scan();
                                } while (kb_Data[7] & kb_Down);
                            }
                            if (kb_Data[7] & kb_Up && selection > 0)
                            {
                                selection--;
                                do
                                {
                                    kb_Scan();
                                } while (kb_Data[7] & kb_Up);
                            }
                            if (kb_Data[4] & kb_5 && selection2 < (int)characterList.size() - 1)
                            {
                                selection2++;
                                do
                                {
                                    kb_Scan();
                                } while (kb_Data[4] & kb_5);
                            }
                            if (kb_Data[4] & kb_8 && selection2 > 0)
                            {
                                selection2--;
                                do
                                {
                                    kb_Scan();
                                } while (kb_Data[4] & kb_8);
                            }
                            if (kb_Data[1] & kb_2nd)
                            {
                                do
                                {
                                    kb_Scan();
                                } while (kb_Data[1] & kb_2nd);

                                switch (selection)
                                {
                                default:
                                case 0:
                                    p1Char = tetris;
                                    break;
                                case 1:
                                    p1Char = oiram;
                                    break;
                                }
                                switch (selection2)
                                {
                                default:
                                case 0:
                                    p2Char = tetris;
                                    break;
                                case 1:
                                    p2Char = oiram;
                                    break;
                                }

                                selection = 0;
                                while (!(kb_Data[6] & kb_Clear || kb_Data[2] & kb_Alpha))
                                {
                                    gfx_SetPalette(menu_palette, sizeof_menu_palette, menu_palette_offset);
                                    gfx_FillScreen(93);
                                    gfx_SetTextFGColor(5);
                                    gfx_SetTextBGColor(255);
                                    gfx_PrintStringXY("Stage Select ", 1, 1);
                                    gfx_SetTextBGColor(5);
                                    for (int i = 0; i < (int)stageList.size(); i++)
                                    {
                                        if (i == selection)
                                        {
                                            gfx_SetTextFGColor(6);
                                            gfx_SetTextBGColor(5);
                                            gfx_PrintStringXY(stageList[i], 9, 10 * i + 11);
                                            gfx_TransparentSprite(buttonleftselect, 1, 10 * i + 11);
                                            gfx_TransparentSprite(buttonrightselect, gfx_GetTextX(), 10 * i + 11);
                                        }
                                        else
                                        {
                                            gfx_SetTextFGColor(5);
                                            gfx_SetTextBGColor(6);
                                            gfx_PrintStringXY(stageList[i], 9, 10 * i + 11);
                                            gfx_TransparentSprite(buttonleft, 1, 10 * i + 11);
                                            gfx_TransparentSprite(buttonright, gfx_GetTextX(), 10 * i + 11);
                                        }
                                    }
                                    gfx_SwapDraw();

                                    //Stage Select
                                    kb_Scan();
                                    if (kb_Data[7] & kb_Down && selection < (int)stageList.size() - 1)
                                    {
                                        selection++;
                                        do
                                        {
                                            kb_Scan();
                                        } while (kb_Data[7] & kb_Down);
                                    }
                                    if (kb_Data[7] & kb_Up && selection > 0)
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

                                        s.solids.clear();

                                        switch (selection)
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
                                        battle();
                                    }
                                }
                                do
                                {
                                    kb_Scan();
                                } while (kb_Data[6] & kb_Clear || kb_Data[2] & kb_Alpha);
                                selection = 0;
                            }
                        }
                        do
                        {
                            kb_Scan();
                        } while (kb_Data[6] & kb_Clear || kb_Data[2] & kb_Alpha);
                        selection = 0;
                        break;
                    case 1:
                        //Online
                        break;
                    case 2:
                        selection = 0;
                        while (!(kb_Data[6] & kb_Clear || kb_Data[2] & kb_Alpha))
                        {
                            gfx_SetPalette(menu_palette, sizeof_menu_palette, menu_palette_offset);
                            gfx_FillScreen(93);
                            gfx_SetTextFGColor(32);
                            gfx_SetTextBGColor(255);
                            gfx_PrintStringXY("Options", 1, 1);
                            for (int i = 0; i < (int)optionsList.size(); i++)
                            {
                                gfx_PrintStringXY(optionsList[i], 9, 8 * i + 9);
                                gfx_PrintInt(options[i], 1);
                            }
                            gfx_PrintStringXY(">", 1, 8 * selection + 9);
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
                            if (kb_Data[7] & kb_Up && selection > 0)
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
                                    gfx_SetTextFGColor(32);
                                    gfx_SetTextBGColor(255);
                                    gfx_SetTextScale(1, 1);
                                    gfx_PrintStringXY("Options ", 1, 1);
                                    for (int i = 0; i < (int)optionsList.size(); i++)
                                    {
                                        gfx_SetTextFGColor(32);
                                        gfx_PrintStringXY(optionsList[i], 9, 8 * i + 9);
                                        gfx_SetTextFGColor(32 + 128 * (i == selection));
                                        gfx_PrintInt(options[i], 1);
                                    }
                                    gfx_SetTextFGColor(32);
                                    gfx_PrintStringXY(">", 1, 8 * selection + 9);
                                    gfx_SwapDraw();

                                    kb_Scan();

                                    switch (selection)
                                    {
                                    case 0:
                                        if (kb_Data[7] & kb_Up && options[0] < 99)
                                        {
                                            options[0]++;
                                            do
                                            {
                                                kb_Scan();
                                            } while (kb_Data[7] & kb_Up);
                                        }
                                        if (kb_Data[7] & kb_Down && options[0] > 1)
                                        {
                                            options[0]--;
                                            do
                                            {
                                                kb_Scan();
                                            } while (kb_Data[7] & kb_Down);
                                        }
                                        break;
                                    default:
                                        if (kb_Data[7] & kb_Up)
                                            options[selection] = 1;
                                        if (kb_Data[7] & kb_Down)
                                            options[selection] = 0;
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

                        optionsFile = ti_Open("SSBCEopt", "w");
                        for (int i = 0; i < (int)options.size(); i++)
                            ti_PutC(options[i], optionsFile);
                        ti_SetArchiveStatus(true, optionsFile);
                        ti_Close(optionsFile);
                        break;
                    }
                }
            }
            do
            {
                kb_Scan();
            } while (kb_Data[6] & kb_Clear);
        }
    }

    gfx_End();
    
    return 0;
}

void battle()
{
    /*gfx_SetTransparentColor(0);
    gfx_SetColor(100);
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);

    s.bgColor = 93;
    s.solids.push_back(Solid(80, 135, 160, 26));
    s.solids.push_back(Solid(100, 161, 120, 26));
    s.solids.push_back(Solid(100, 104, 40, 5, true));
    s.solids.push_back(Solid(140, 73, 40, 5, true));
    s.solids.push_back(Solid(180, 104, 40, 5, true));

    gfx_sprite_t* finalDes;
    finalDes = gfx_MallocSprite(200, 100);
    zx0_Decompress(finalDes, finalDes_compressed);
    s.solids[0].anim.frames.push_back(finalDes);*/

    gfx_SetPalette(global_palette, sizeof_global_palette, 0);

    vector<Player*> players;
    switch (p1Char)
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
    }
    switch (p2Char)
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
    }
    for (int i = 0; i < (int)players.size(); i++)
    {
        if (players[i]->loadSprites())
        {
            gfx_SetTextXY(10, 10);
            gfx_SetTextFGColor(5);
            gfx_SetTextBGColor(255);
            gfx_FillScreen(93);
            gfx_PrintString("Missing AppVar");
            gfx_SwapDraw();
            delay(2000);
            return;
        }
    }

    players[0]->team = 1;
    players[0]->xpos = 107 + (players[0]->team == 2) * 95;
    players[0]->ypos = 75;
    players[0]->xvel = 0;
    players[0]->yvel = 0;
    players[0]->stocks = options[0];

    players[1]->team = 2;
    players[1]->xpos = 107 + (players[1]->team == 2) * 95;
    players[1]->ypos = 75;
    players[1]->xvel = 0;
    players[1]->yvel = 0;
    players[1]->facingLeft = true;
    players[1]->stocks = options[0];

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

    int frame = 0;
    int frameSkip = 3;
    int fps = 0;
    int targetFPS = 60;
    bool debug = false;
    timer_Enable(1, TIMER_32K, TIMER_NOINT, TIMER_UP);
    gfx_SetTextXY(10, 10);
    timer_Set(1, 0);
    do
    {
        if ((int)timer_Get(1) >= 32768 / targetFPS)
        {
            fps = 32768 / timer_Get(1);
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
            frameSkip += ((fps >= targetFPS) && (frameSkip < 30)) - ((fps < targetFPS - 8) && (frameSkip > 1));
            if (frameSkip < 1)
                frameSkip = 1;
            if (frame <= frameSkip)
            {
                s.render();
                players[0]->render();
                players[1]->render();
                if (debug)
                {
                    gfx_SetTextXY(10, 10);
                    gfx_SetTextFGColor(5);
                    gfx_SetTextBGColor(255);
                    gfx_PrintInt(fps, 1);
                    gfx_SetTextXY(10, 20);
                    gfx_PrintInt(frameSkip, 1);
                    gfx_SetTextXY(10, 30);
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