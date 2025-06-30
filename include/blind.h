#ifndef BLIND_H
#define BLIND_H

#include "sprite.h"

enum BlindType
{
    SMALL_BLIND,
    BIG_BLIND,
    BOSS_BLIND
};

void blinds_init();

Sprite *blind_token_new(enum BlindType type, int x, int y, int sprite_index); 

#endif // BLIND_H