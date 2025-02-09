#include <tonc.h>
#include <maxmod.h>
#include <string.h>

#include "sprite.h"
#include "card.h"
#include "game.h"

// Graphics
#include "deck_gfx.h"
#include "background_gfx.h"

// Audio
#include "soundbank.h"
#include "soundbank_bin.h"

void init()
{
    irq_init(NULL);
    irq_add(II_VBLANK, mmVBlank);

    // Initialize maxmod
    mmInitDefault((mm_addr)soundbank_bin, 12);
    mmStart(MOD_MAIN_THEME, MM_PLAY_LOOP);

    // Initialize text engine
    tte_init_se(1, BG_CBB(1) | BG_SBB(31) | BG_8BPP, 0, CLR_WHITE, 11, NULL, NULL);
    tte_init_con();

    // Set up the video mode
    REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ_1D | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ;
	REG_BG0CNT = BG_PRIO(1) | BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_64x32;
    REG_BG1CNT = BG_PRIO(0) | BG_CBB(1) | BG_SBB(31) | BG_8BPP;

    // Load the tiles and palette
    memcpy(pal_bg_mem, background_gfxPal, background_gfxPalLen);
    memcpy(&tile_mem[0][0], background_gfxTiles, background_gfxTilesLen);   
    memcpy32(&se_mem[30][0], background_gfxMap, background_gfxMapLen);

    memcpy(&tile_mem[4][0], deck_gfxTiles, deck_gfxTilesLen);
    memcpy(pal_obj_mem, deck_gfxPal, deck_gfxPalLen);

    // Initialize subsystems
    sprite_init();
    card_init();
    game_init();
}

void update()
{
    game_update();
    card_update();
}

void draw()
{
    sprite_draw();
}

int main()
{
    init();

	while(true)
    {
        vid_vsync();
        mmFrame();
		key_poll();
        update();
        draw();
    }

	return 0;
}