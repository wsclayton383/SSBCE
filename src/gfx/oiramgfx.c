#include "oiramgfx.h"
#include <fileioc.h>

#define oiramgfx_HEADER_SIZE 0

unsigned char *oiramgfx_appvar[13] =
{
    (unsigned char*)0,
    (unsigned char*)350,
    (unsigned char*)416,
    (unsigned char*)482,
    (unsigned char*)548,
    (unsigned char*)614,
    (unsigned char*)1048,
    (unsigned char*)1482,
    (unsigned char*)1916,
    (unsigned char*)2350,
    (unsigned char*)2576,
    (unsigned char*)3478,
    (unsigned char*)4047,
};

unsigned char oiramgfx_init(void)
{
    unsigned int data, i;
    uint8_t appvar;

    appvar = ti_Open("oiramgfx", "r");
    if (appvar == 0)
    {
        return 0;
    }

    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)oiramgfx_appvar[0] + oiramgfx_HEADER_SIZE;
    for (i = 0; i < 13; i++)
    {
        oiramgfx_appvar[i] += data;
    }

    ti_Close(appvar);

    return 1;
}

