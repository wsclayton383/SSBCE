#include "chargfx.h"
#include <fileioc.h>

#define chargfx_HEADER_SIZE 0

unsigned char *chargfx_appvar[39] =
{
    (unsigned char*)0,
    (unsigned char*)350,
    (unsigned char*)700,
    (unsigned char*)1032,
    (unsigned char*)1364,
    (unsigned char*)1696,
    (unsigned char*)2028,
    (unsigned char*)2262,
    (unsigned char*)2496,
    (unsigned char*)2723,
    (unsigned char*)3055,
    (unsigned char*)3387,
    (unsigned char*)3719,
    (unsigned char*)4051,
    (unsigned char*)4278,
    (unsigned char*)4610,
    (unsigned char*)4676,
    (unsigned char*)4742,
    (unsigned char*)5176,
    (unsigned char*)5610,
    (unsigned char*)5836,
    (unsigned char*)6738,
    (unsigned char*)7442,
    (unsigned char*)7774,
    (unsigned char*)7821,
    (unsigned char*)8375,
    (unsigned char*)9277,
    (unsigned char*)10143,
    (unsigned char*)11169,
    (unsigned char*)11792,
    (unsigned char*)12415,
    (unsigned char*)13038,
    (unsigned char*)13607,
    (unsigned char*)14089,
    (unsigned char*)14667,
    (unsigned char*)15245,
    (unsigned char*)15823,
    (unsigned char*)16401,
    (unsigned char*)16979,
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
    for (i = 0; i < 39; i++)
    {
        chargfx_appvar[i] += data;
    }

    ti_Close(appvar);

    return 1;
}

