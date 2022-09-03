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
#include "Projectile.h"
#include "Hitbox.h"
#include "Animation.h"
#include "Stage.h"
#include "Solid.h"

using namespace tinystl;

int main()
{
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetTransparentColor(0);
    gfx_SetColor(100);

    s.bgColor = 51;
    s.solids.push_back(Solid(80, 135, 160, 26));
    s.solids.push_back(Solid(100, 162, 140, 26));
    gfx_sprite_t* finalDes;
    finalDes = gfx_MallocSprite(200, 100);
    zx0_Decompress(finalDes, finalDes_compressed);
    s.solids[0].anim.frames.push_back(finalDes);
    Tetris p1;
    p1.loadSprites();
    Tetris p2 = p1;
    
    free(finalDes);

    p1.xpos = 107;
    p1.ypos = 100;
    p1.team = 1;

    p2.xpos = 213;
    p2.ypos = 100;
    p2.team = 2;

    int frame = 0;
    int frameSkip = 3;
    int fps = 0;
    int targetFPS = 60;
    bool debug = false;
    timer_Enable(1, TIMER_32K, TIMER_NOINT, TIMER_UP);
    gfx_SetTextXY(10, 10);
    gfx_SetTextFGColor(32);

    gfx_SetPalette(global_palette, sizeof_global_palette, 0);

    timer_Set(1, 0);
    do
    {
        if ((int)timer_Get(1) >= 32768 / 60)
        {
            fps = 32768 / timer_Get(1);
            timer_Set(1, 0);
            kb_Scan();
            if (kb_Data[1] & kb_Yequ)
                debug = true;
            if (kb_Data[1] & kb_Window)
                debug = false;
            p1.damage += (kb_Data[6] & kb_Add) - (kb_Data[6] & kb_Sub);
            s.hboxes.clear();
            p1.update(kb_Data[1] & kb_2nd, kb_Data[2] & kb_Alpha, kb_Data[2] & kb_Math, kb_Data[1] & kb_Mode, kb_Data[3] & kb_GraphVar, kb_Data[7] & kb_Down, kb_Data[7] & kb_Left, kb_Data[7] & kb_Right, kb_Data[7] & kb_Up);
            p2.update(kb_Data[6] & kb_Power, kb_Data[6] & kb_Div, kb_Data[6] & kb_Mul, kb_Data[5] & kb_Tan, kb_Data[5] & kb_RParen, kb_Data[4] & kb_5, kb_Data[3] & kb_4, kb_Data[5] & kb_6, kb_Data[4] & kb_8);
            p1.collide();
            p2.collide();
            frame++;
            frameSkip += ((fps >= targetFPS) && (frameSkip < 30)) - ((fps < targetFPS - 8) && (frameSkip > 1));
            if (frameSkip < 1)
                frameSkip = 1;
            if (frame <= frameSkip)
            {
                s.render();
                p1.render();
                p2.render();
                if (debug)
                {
                    gfx_SetTextXY(10, 10);
                    gfx_SetTextFGColor(32);
                    gfx_SetTextBGColor(255);
                    gfx_PrintInt(fps, 1);
                    gfx_SetTextXY(10, 20);
                    gfx_PrintInt(frameSkip, 1);
                    gfx_SetColor(32);
                    for (int i = 0; i < (int)s.hboxes.size(); i++)
                    {
                        gfx_FillRectangle(s.hboxes[i].x1, s.hboxes[i].y1, s.hboxes[i].x2, s.hboxes[i].y2);
                    }
                }
                gfx_SwapDraw();
            }
            else
                frame = 0;
        }
    } while (!(kb_Data[6] & kb_Clear));
    gfx_End();
    
    return 0;
}