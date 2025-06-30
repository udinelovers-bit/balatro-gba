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

Sprite *blind_token_new(enum BlindType type, int x, int y, int sprite_index)
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
