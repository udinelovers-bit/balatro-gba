#include <tonc.h>

#include "joker.h"
#include "joker_gfx.h"
#include "graphic_utils.h"

const static u8 joker_data_lut[MAX_JOKERS][2] = // Rarity, Value
{
    {COMMON_JOKER, 2}, // Default Joker
    {COMMON_JOKER, 5}, // Greedy Joker
};

void joker_init()
{
    GRIT_CPY(&tile_mem[4][JOKER_TID], joker_gfxTiles);
    memcpy16(&pal_obj_mem[PAL_ROW_LEN * JOKER_PB], joker_gfxPal, sizeof(joker_gfxPal) / 2);
}

Joker *joker_new(u8 id)
{
    Joker *joker = malloc(sizeof(Joker));

    joker->id = id; // TODO: Make this random later
    joker->modifier = BASE_EDITION; // TODO: Make this random later
    joker->value = joker_data_lut[id][1];
    joker->rarity = joker_data_lut[id][0];

    return joker;
}

void joker_destroy(Joker **joker)
{
    if (*joker == NULL) return;
    free(*joker);
    *joker = NULL;
}