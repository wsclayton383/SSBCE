#ifndef oiramgfx_appvar_include_file
#define oiramgfx_appvar_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define sizeof_global_palette 350
#define global_palette (oiramgfx_appvar[0])
#define oiram_palette_offset 0
#define fireleft1_width 8
#define fireleft1_height 8
#define oiramgfx_oiram_fireleft1_index 1
#define fireleft1 ((gfx_sprite_t*)oiramgfx_appvar[1])
#define fireleft2_width 8
#define fireleft2_height 8
#define oiramgfx_oiram_fireleft2_index 2
#define fireleft2 ((gfx_sprite_t*)oiramgfx_appvar[2])
#define fireright1_width 8
#define fireright1_height 8
#define oiramgfx_oiram_fireright1_index 3
#define fireright1 ((gfx_sprite_t*)oiramgfx_appvar[3])
#define fireright2_width 8
#define fireright2_height 8
#define oiramgfx_oiram_fireright2_index 4
#define fireright2 ((gfx_sprite_t*)oiramgfx_appvar[4])
#define oiramleft1_width 16
#define oiramleft1_height 27
#define oiramgfx_oiram_oiramleft1_index 5
#define oiramleft1 ((gfx_sprite_t*)oiramgfx_appvar[5])
#define oiramleft2_width 16
#define oiramleft2_height 27
#define oiramgfx_oiram_oiramleft2_index 6
#define oiramleft2 ((gfx_sprite_t*)oiramgfx_appvar[6])
#define oiramright1_width 16
#define oiramright1_height 27
#define oiramgfx_oiram_oiramright1_index 7
#define oiramright1 ((gfx_sprite_t*)oiramgfx_appvar[7])
#define oiramright2_width 16
#define oiramright2_height 27
#define oiramgfx_oiram_oiramright2_index 8
#define oiramright2 ((gfx_sprite_t*)oiramgfx_appvar[8])
#define oiramshield1_width 14
#define oiramshield1_height 16
#define oiramgfx_oiram_oiramshield1_index 9
#define oiramshield1 ((gfx_sprite_t*)oiramgfx_appvar[9])
#define oiramsmashcharge1_width 30
#define oiramsmashcharge1_height 30
#define oiramgfx_oiram_oiramsmashcharge1_index 10
#define oiramsmashcharge1 ((gfx_sprite_t*)oiramgfx_appvar[10])
#define oiramspin1_width 21
#define oiramspin1_height 27
#define oiramgfx_oiram_oiramspin1_index 11
#define oiramspin1 ((gfx_sprite_t*)oiramgfx_appvar[11])
#define oiramspin2_width 26
#define oiramspin2_height 27
#define oiramgfx_oiram_oiramspin2_index 12
#define oiramspin2 ((gfx_sprite_t*)oiramgfx_appvar[12])
#define oiramgfx_entries_num 13
extern unsigned char *oiramgfx_appvar[13];
unsigned char oiramgfx_init(void);

#ifdef __cplusplus
}
#endif

#endif
