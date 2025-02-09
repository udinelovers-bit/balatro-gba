#include "game.h"

#include <maxmod.h>
#include <tonc.h>
#include <string.h>
#include <stdlib.h>

#include "sprite.h"
#include "card.h"

void game_init()
{
    
}

void game_update()
{
    if (key_hit(KEY_LEFT))
    {
        hand_set_focus(hand_get_focus() + 1); // The reason why this adds 1 is because the hand is drawn from right to left. There is no particular reason for this, it's just how I did it.
    }
    else if (key_hit(KEY_RIGHT))
    {
        hand_set_focus(hand_get_focus() - 1);
    }

    if (key_hit(KEY_A))
    {
        hand_select();
    }

    if (key_hit(KEY_B))
    {
        hand_change_sort();
    }

    if (key_hit(KEY_SELECT))
    {
        hand_discard();
    }

    tte_printf("#{es; P:128,130}%d/%d", hand_get_size(), hand_get_max_size());
    tte_printf("#{P:200,155}%d/%d", deck_get_size(), deck_get_max_size());
}