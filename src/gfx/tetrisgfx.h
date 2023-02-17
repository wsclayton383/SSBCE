#ifndef tetrisgfx_appvar_include_file
#define tetrisgfx_appvar_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define sizeof_global_palette 350
#define global_palette (tetrisgfx_appvar[0])
#define tetris_palette_offset 0
#define tetrisattackneutralleft1_width 16
#define tetrisattackneutralleft1_height 11
#define tetrisgfx_tetris_tetrisattackneutralleft1_index 1
#define tetrisattackneutralleft1 ((gfx_sprite_t*)tetrisgfx_appvar[1])
#define tetrisattackneutralright1_width 16
#define tetrisattackneutralright1_height 11
#define tetrisgfx_tetris_tetrisattackneutralright1_index 2
#define tetrisattackneutralright1 ((gfx_sprite_t*)tetrisgfx_appvar[2])
#define tetrisattackrecovery1_width 16
#define tetrisattackrecovery1_height 11
#define tetrisgfx_tetris_tetrisattackrecovery1_index 3
#define tetrisattackrecovery1 ((gfx_sprite_t*)tetrisgfx_appvar[3])
#define tetrisattackrecovery2_width 16
#define tetrisattackrecovery2_height 11
#define tetrisgfx_tetris_tetrisattackrecovery2_index 4
#define tetrisattackrecovery2 ((gfx_sprite_t*)tetrisgfx_appvar[4])
#define tetrisattacksmash1_width 21
#define tetrisattacksmash1_height 6
#define tetrisgfx_tetris_tetrisattacksmash1_index 5
#define tetrisattacksmash1 ((gfx_sprite_t*)tetrisgfx_appvar[5])
#define tetrisattacksmashcharge1_width 6
#define tetrisattacksmashcharge1_height 21
#define tetrisgfx_tetris_tetrisattacksmashcharge1_index 6
#define tetrisattacksmashcharge1 ((gfx_sprite_t*)tetrisgfx_appvar[6])
#define tetrisattackspecial1_width 11
#define tetrisattackspecial1_height 11
#define tetrisgfx_tetris_tetrisattackspecial1_index 7
#define tetrisattackspecial1 ((gfx_sprite_t*)tetrisgfx_appvar[7])
#define tetrisdodgeleft1_width 11
#define tetrisdodgeleft1_height 16
#define tetrisgfx_tetris_tetrisdodgeleft1_index 8
#define tetrisdodgeleft1 ((gfx_sprite_t*)tetrisgfx_appvar[8])
#define tetrisdodgeright1_width 11
#define tetrisdodgeright1_height 16
#define tetrisgfx_tetris_tetrisdodgeright1_index 9
#define tetrisdodgeright1 ((gfx_sprite_t*)tetrisgfx_appvar[9])
#define tetrisidleleft1_width 11
#define tetrisidleleft1_height 16
#define tetrisgfx_tetris_tetrisidleleft1_index 10
#define tetrisidleleft1 ((gfx_sprite_t*)tetrisgfx_appvar[10])
#define tetrisidleright1_width 11
#define tetrisidleright1_height 16
#define tetrisgfx_tetris_tetrisidleright1_index 11
#define tetrisidleright1 ((gfx_sprite_t*)tetrisgfx_appvar[11])
#define tetrisshield1_width 11
#define tetrisshield1_height 11
#define tetrisgfx_tetris_tetrisshield1_index 12
#define tetrisshield1 ((gfx_sprite_t*)tetrisgfx_appvar[12])
#define tetrisgfx_entries_num 13
extern unsigned char *tetrisgfx_appvar[13];
unsigned char tetrisgfx_init(void);

#ifdef __cplusplus
}
#endif

#endif
