#include "affine_background.h"
#include "affine_background_gfx.h"

#include "graphic_utils.h"

BG_AFFINE bgaff;
AFF_SRC_EX asx = {32<<8, 64<<8, 120, 80, 0x0100, 0x0100, 0};

void affine_background_init()
{
    unsigned int correctedTiles[affine_background_gfxTilesLen / 4];

    for (int i = 0; i < affine_background_gfxTilesLen / 4; i++)
    {   
        correctedTiles[i] = affine_background_gfxTiles[i] | 0xF0F0F0F0;
    }

	memcpy16(&tile8_mem[AFFINE_BG_CBB], correctedTiles, affine_background_gfxTilesLen/2);
    GRIT_CPY(&se_mem[AFFINE_BG_SBB], affine_background_gfxMap);
    memcpy16(&pal_bg_mem[AFFINE_BG_PB], affine_background_gfxPal, AFFINE_BG_PAL_LEN);

	bgaff = bg_aff_default;

    affine_background_set_color(AFFINE_BG_DEFAULT_COLOR);
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
        COLOR old_color;
        memcpy16(&old_color, &pal_bg_mem[AFFINE_BG_PB] + i, 1); // Copy the color from the background palette

        int r, g, b;
        r = (old_color >> 10) & 0b11111; // Extract red component
        g = (old_color >> 5) & 0b11111; // Extract green component
        b = old_color & 0b11111; // Extract blue component

        int brightness = (r + g + b) / 3; // Calculate brightness

        const float brightness_divergence = 0.03f; // The difference in brightness between each color in the palette
        const float brightness_modifier = -0.4f; // The overall brightness modifier for the palette

        clr_adj_brightness(&pal_bg_mem[AFFINE_BG_PB] + i, &color, 1, float2fx(brightness_modifier + (brightness * brightness_divergence))); // Adjust brightness for the palette colors
    }
}