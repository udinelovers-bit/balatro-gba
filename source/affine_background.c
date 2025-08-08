#include "affine_background.h"
#include "affine_background_gfx.h"

#include "graphic_utils.h"

BG_AFFINE bgaff;
AFF_SRC_EX asx = {32<<8, 64<<8, 120, 80, 0x0100, 0x0100, 0};

void affine_background_init()
{
    memcpy16_tile8_with_palette_offset((u16*)tile8_mem[AFFINE_BG_CBB], (const u16*)affine_background_gfxTiles, affine_background_gfxTilesLen/2, AFFINE_BG_PB);
    GRIT_CPY(&se_mem[AFFINE_BG_SBB], affine_background_gfxMap);
    affine_background_load_palette(affine_background_gfxPal);

    bgaff = bg_aff_default;
}

void affine_background_update()
{
    static uint timer = 0;
    timer++;

    // These values are not permament, just the current configuration for the affine background
    asx.tex_x += 5;
    asx.tex_y += 12;
    asx.sx = (lu_sin(timer * 100)) >> 8; // Scale the sine value to fit in a s16
    asx.sx += 256; // Add 256 to the sine value to make it positive
    asx.sy = (lu_sin(timer * 100 + 0x4000)) >> 8; // Scale the sine value to fit in a s16
    asx.sy += 256; // Add 256 to the sine value to make it positive
    bg_rotscale_ex(&bgaff, &asx);
    REG_BG_AFFINE[2] = bgaff;
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