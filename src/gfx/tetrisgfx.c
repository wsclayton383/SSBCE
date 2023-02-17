#include "tetrisgfx.h"
#include <fileioc.h>

#define tetrisgfx_HEADER_SIZE 0

unsigned char *tetrisgfx_appvar[13] =
{
    (unsigned char*)0,
    (unsigned char*)350,
    (unsigned char*)528,
    (unsigned char*)706,
    (unsigned char*)884,
    (unsigned char*)1062,
    (unsigned char*)1190,
    (unsigned char*)1318,
    (unsigned char*)1441,
    (unsigned char*)1619,
    (unsigned char*)1797,
    (unsigned char*)1975,
    (unsigned char*)2153,
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

