#ifndef BLIND_H
#define BLIND_H

#include "sprite.h"

#define MAX_ANTE 8 // The GBA's max uint value is around 4 billion, so we're going to not add endless mode for simplicity's sake

// +1 is added because we'll actually be indexing at 1, but if something causes you to go to ante 0, there will still be a value there.
static const int ante_lut[MAX_ANTE + 1] = {100, 300, 800, 2000, 5000, 11000, 20000, 35000, 50000};

#define MAX_BLINDS 3

#define SMALL_BLIND 0
#define BIG_BLIND 1
#define BOSS_BLIND 2

enum BlindState
{
    BLIND_CURRENT,
    BLIND_UPCOMING,
    BLIND_DEFEATED,
    BLIND_SKIPPED
};

void blinds_init();

int blind_get_requirement(int type, int ante);
int blind_get_reward(int type);

Sprite *blind_token_new(int type, int x, int y, int sprite_index); 

#endif // BLIND_H