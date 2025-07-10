#include <tonc.h>

#include "blind.h"

#include "blinds_gfx.h"

void blinds_init()
{
    // Blind graphics 
    // Small Blind (fighting grit every step of the way as usual)
    //memcpy(&tile_mem[4][832], blinds_gfxTiles, blinds_gfxTilesLen);
    GRIT_CPY(&tile_mem[4][832], blinds_gfxTiles);
    const u16 small_blind_token_palette[4] = {0x7FFF, 0x34A1, 0x5DCB, 0x5104};
    memcpy16(&pal_obj_mem[17], &small_blind_token_palette, sizeof(small_blind_token_palette) / 2);
}

int blind_get_requirement(int type, int ante)
{
    if (ante < 0 || ante > MAX_ANTE)
    {
        ante = 0; // Ensure ante is within valid range
    }

    switch (type)
    {
        case SMALL_BLIND:
            return ante_lut[ante];
        case BIG_BLIND:
            return (ante_lut[ante] * 3) / 2; // X1.5
        case BOSS_BLIND:
            return ante_lut[ante] * 2; // X2
        default:
            return 0; // Invalid type
    }
}

int blind_get_reward(int type)
{
    switch (type)
    {
        case SMALL_BLIND:
            return 3;
        case BIG_BLIND:
            return 4;
        case BOSS_BLIND:
            return 5;
        default:
            return 0; // Invalid type
    }
}

Sprite *blind_token_new(int type, int x, int y, int sprite_index)
{
    Sprite *sprite = NULL;

    switch (type)
    {
        case SMALL_BLIND:
            sprite = sprite_new(ATTR0_SQUARE | ATTR0_4BPP, ATTR1_SIZE_32x32, 832, 1, sprite_index);
            break;
        case BIG_BLIND:
            // Use the big blind sprite
            break;
        case BOSS_BLIND:
            // Use the boss blind sprite
            break;
        default:
            return NULL;
    }

    sprite_position(sprite, x, y);

    return sprite;
}
