#ifndef BLIND_H
#define BLIND_H

#include "sprite.h"

#define MAX_ANTE 8 // The GBA's max uint value is around 4 billion, so we're going to not add endless mode for simplicity's sake

// +1 is added because we'll actually be indexing at 1, but if something causes you to go to ante 0, there will still be a value there.
static const int ante_lut[MAX_ANTE + 1] = {100, 300, 800, 2000, 5000, 11000, 20000, 35000, 50000};

enum BlindType
{
    SMALL_BLIND,
    BIG_BLIND,
    BOSS_BLIND // This is just for a testing and should be replaced when more blinds are added
};

void blinds_init();

int blind_get_requirement(enum BlindType type, int ante);
int blind_get_reward(enum BlindType type);

Sprite *blind_token_new(enum BlindType type, int x, int y, int sprite_index); 

#endif // BLIND_H