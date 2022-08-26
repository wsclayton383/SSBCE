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
#include "Projectile.h"
#include "Hitbox.h"
#include "Animation.h"
#include "Stage.h"
#include "Solid.h"
#include "gfx/gfx.h"

using namespace tinystl;

void loadPlayerSprites(Player& p, int character);

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
    Player p1;
    loadPlayerSprites(p1, tetris);
    
    free(finalDes);

    p1.xpos = 107;
    p1.ypos = 100;
    p1.playerNumber = 1;

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
            p1.update(kb_Data[1] & kb_2nd, kb_Data[2] & kb_Alpha, kb_Data[2] & kb_Math, kb_Data[1] & kb_Mode, kb_Data[3] & kb_GraphVar, kb_Data[7] & kb_Down, kb_Data[7] & kb_Left, kb_Data[7] & kb_Right, kb_Data[7] & kb_Up);
            frame++;
            frameSkip += ((fps >= targetFPS) && (frameSkip < 30)) - ((fps < targetFPS - 5) && (frameSkip > 1));
            if (frameSkip < 1)
                frameSkip = 1;
            if (frame <= frameSkip)
            {
                s.render();
                if (debug)
                {
                    gfx_SetTextXY(10, 10);
                    gfx_SetTextFGColor(32);
                    gfx_SetTextBGColor(255);
                    gfx_PrintInt(fps, 1);
                    gfx_SetTextXY(10, 20);
                    gfx_PrintInt(frameSkip, 1);
                }
                p1.render();
                gfx_SwapDraw();
            }
            else
                frame = 0;
        }
    } while (!(kb_Data[6] & kb_Clear));
    gfx_End();
    
    return 0;
}

void loadPlayerSprites(Player& p, int character)
{
    vector<gfx_sprite_t*> temp;
    switch (character)
    {
    default:
    case tetris:
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
        
        p.hboxx = tetrisidleright1_width;
        p.hboxy = tetrisidleright1_height;
        p.anims.push_back(idleRight);
        p.anims.push_back(idleLeft);
        p.anims.push_back(shield);
        p.anims.push_back(shieldBroken);
        p.anims.push_back(dodgeLeft);
        p.anims.push_back(dodgeRight);
        p.anims.push_back(neutralLeft);
        p.anims.push_back(neutralRight);
        p.anims.push_back(smashCharge);
        p.anims.push_back(smash);
        p.anims.push_back(special);
        p.anims.push_back(recovery);
        break;
    }

    for (int i = 0; i < (int)temp.size(); i++)
    {
        free(temp[i]);
    }
}