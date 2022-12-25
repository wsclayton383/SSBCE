#include "tetrisgfx.h"
#include <fileioc.h>

#define tetrisgfx_HEADER_SIZE 0

unsigned char *tetrisgfx_appvar[13] =
{
    (unsigned char*)0,
    (unsigned char*)28,
    (unsigned char*)206,
    (unsigned char*)384,
    (unsigned char*)562,
    (unsigned char*)740,
    (unsigned char*)868,
    (unsigned char*)996,
    (unsigned char*)1119,
    (unsigned char*)1297,
    (unsigned char*)1475,
    (unsigned char*)1653,
    (unsigned char*)1831,
};

unsigned char tetrisgfx_init(void)
{
    unsigned int data, i;
    uint8_t appvar;

    appvar = ti_Open("tetrisgfx", "r");
    if (appvar == 0)
    {
        return 0;
    }

    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)tetrisgfx_appvar[0] + tetrisgfx_HEADER_SIZE;
    for (i = 0; i < 13; i++)
    {
        tetrisgfx_appvar[i] += data;
    }

    ti_Close(appvar);

    return 1;
}

