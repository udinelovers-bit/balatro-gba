#include "affine_background.h"
#include "affine_background_gfx.h"

#include "graphic_utils.h"

BG_AFFINE bgaff;
AFF_SRC_EX asx = {32<<8, 64<<8, 120, 80, 0x0100, 0x0100, 0};
static uint timer = 0;

void affine_background_init()
{
    memcpy32_tile8_with_palette_offset((u32*)&tile8_mem[AFFINE_BG_CBB], (const u32*)affine_background_gfxTiles, affine_background_gfxTilesLen/4, AFFINE_BG_PB);
    GRIT_CPY(&se_mem[AFFINE_BG_SBB], affine_background_gfxMap);
    affine_background_load_palette(affine_background_gfxPal);

    bgaff = bg_aff_default;
}

void affine_background_hblank()
{
    const s32 timer_s32 = timer << 8;
    const s32 vcount_s32 = REG_VCOUNT << 8;
    const s16 vcount_s16 = REG_VCOUNT;

    asx.scr_y = vcount_s16 - 128; // 128 on x and y is an offset used to center the rotation
    asx.scr_x = 128;
    asx.tex_x = lu_sin(timer_s32 + vcount_s32) + timer / 8;
    asx.alpha = (vcount_s32 + timer_s32) / 8;
    
    bg_rotscale_ex(&bgaff, &asx);
    REG_BG_AFFINE[2] = bgaff;
}

void affine_background_update()
{
    timer++;
}

void affine_background_set_color(COLOR color)
{
    memcpy16(&pal_bg_mem[AFFINE_BG_PB], affine_background_gfxPal, AFFINE_BG_PAL_LEN);
    for (int i = 0; i < AFFINE_BG_PAL_LEN; i++)
    {
        clr_rgbscale(&pal_bg_mem[AFFINE_BG_PB] + i, &pal_bg_mem[AFFINE_BG_PB] + i, 1, color);
    }
}

void affine_background_load_palette(const u16 *src)
{
    memcpy16(&pal_bg_mem[AFFINE_BG_PB], src, AFFINE_BG_PAL_LEN);
}