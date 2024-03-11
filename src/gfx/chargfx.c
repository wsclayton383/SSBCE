#include "chargfx.h"
#include <fileioc.h>

#define chargfx_HEADER_SIZE 0

unsigned char *chargfx_appvar[35] =
{
    (unsigned char*)0,
    (unsigned char*)350,
    (unsigned char*)682,
    (unsigned char*)1014,
    (unsigned char*)1346,
    (unsigned char*)1678,
    (unsigned char*)1912,
    (unsigned char*)2146,
    (unsigned char*)2373,
    (unsigned char*)2705,
    (unsigned char*)3037,
    (unsigned char*)3369,
    (unsigned char*)3701,
    (unsigned char*)3928,
    (unsigned char*)3994,
    (unsigned char*)4060,
    (unsigned char*)4494,
    (unsigned char*)4928,
    (unsigned char*)5154,
    (unsigned char*)6056,
    (unsigned char*)6760,
    (unsigned char*)6807,
    (unsigned char*)7361,
    (unsigned char*)8263,
    (unsigned char*)9129,
    (unsigned char*)10155,
    (unsigned char*)10778,
    (unsigned char*)11401,
    (unsigned char*)12024,
    (unsigned char*)12593,
    (unsigned char*)13075,
    (unsigned char*)13653,
    (unsigned char*)14231,
    (unsigned char*)14809,
    (unsigned char*)15387,
};

unsigned char chargfx_init(void)
{
    unsigned int data, i;
    uint8_t appvar;

    appvar = ti_Open("chargfx", "r");
    if (appvar == 0)
    {
        return 0;
    }

    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)chargfx_appvar[0] + chargfx_HEADER_SIZE;
    for (i = 0; i < 35; i++)
    {
        chargfx_appvar[i] += data;
    }

    ti_Close(appvar);

    return 1;
}

