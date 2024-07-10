#include "chargfx.h"
#include <fileioc.h>

#define chargfx_HEADER_SIZE 0

unsigned char *chargfx_appvar[44] =
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
    (unsigned char*)4344,
    (unsigned char*)4410,
    (unsigned char*)4844,
    (unsigned char*)5278,
    (unsigned char*)5504,
    (unsigned char*)6406,
    (unsigned char*)7110,
    (unsigned char*)7664,
    (unsigned char*)8566,
    (unsigned char*)9432,
    (unsigned char*)10458,
    (unsigned char*)11081,
    (unsigned char*)11704,
    (unsigned char*)12327,
    (unsigned char*)12896,
    (unsigned char*)13378,
    (unsigned char*)13956,
    (unsigned char*)14534,
    (unsigned char*)15112,
    (unsigned char*)15690,
    (unsigned char*)16268,
    (unsigned char*)16526,
    (unsigned char*)16640,
    (unsigned char*)16742,
    (unsigned char*)17224,
    (unsigned char*)18066,
    (unsigned char*)18548,
    (unsigned char*)19030,
    (unsigned char*)19312,
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
    for (i = 0; i < 44; i++)
    {
        chargfx_appvar[i] += data;
    }

    ti_Close(appvar);

    return 1;
}

