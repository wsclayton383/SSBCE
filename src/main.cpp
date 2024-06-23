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
#include "Menu.h"
#include "Player.h"
#include "Tetris.h"
#include "Oiram.h"
#include "Fox.h"
#include "Projectile.h"
#include "Hitbox.h"
#include "Animation.h"
#include "Stage.h"
#include "Solid.h"

using namespace tinystl;

void battle();
int p1Char;
int p2Char;

int main()
{
    gfx_Begin();
    gfx_SetDrawBuffer();

    if (chargfx_init() == 0)
    {
        gfx_SetPalette(menu_palette, sizeof_menu_palette, menu_palette_offset);
        gfx_FillScreen(93);
        gfx_SetTextFGColor(5);
        gfx_SetTextBGColor(255);
        gfx_SetTextScale(1, 1);
        gfx_PrintStringXY("Missing Appvar", 1, 1);
        return 1;
    }

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
    for (int i = options.size(); i < 3; i++)
        options.push_back(0);
    options[0] = 1;
    if ((int)options[1] <= 0)
        options[1] = 3;
    ti_Close(optionsFile);
    optionsFile = ti_Open("SSBCEopt", "w");
    for (int i = 0; i < (int)options.size(); i++)
        ti_PutC(options[i], optionsFile);
    ti_SetArchiveStatus(true, optionsFile);
    ti_Close(optionsFile);

    vector<Button> bTemp;
    bTemp.push_back(Button(17, 36, "Local", 1, charSelect, 2));
    bTemp.push_back(Button(17, 56, "Link", 1, -1, 2));
    bTemp.push_back(Button(17, 76, "Options", 1, option, 2));
    mList.push_back(Menu("Menu", start, bTemp, 4));
    bTemp.clear();
    bTemp.push_back(Button(9, 10, "Tetronimo", 1, stageSelect));
    bTemp.push_back(Button(9, 21, "Oiram", 1, stageSelect));
    bTemp.push_back(Button(9, 32, "Fox", 1, stageSelect));
    bTemp.push_back(Button(115, 10, "Tetronimo", 2, -1));
    bTemp.push_back(Button(115, 21, "Oiram", 2, -1));
    bTemp.push_back(Button(115, 32, "Fox", 2, -1));
    mList.push_back(Menu("Character Select", charSelect, bTemp));
    bTemp.clear();
    bTemp.push_back(Button(9, 10, "Final Destination", 1, fight));
    bTemp.push_back(Button(9, 21, "Battlefield", 1, fight));
    bTemp.push_back(Button(9, 32, "Move", 1, fight));
    bTemp.push_back(Button(9, 43, "Boo", 1, fight));
    mList.push_back(Menu("Stage Select", stageSelect, bTemp));
    bTemp.clear();

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
            mList[start].run();
        }
    }
    gfx_End();

    return 0;
}