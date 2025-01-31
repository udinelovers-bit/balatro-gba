#include "game.h"

#include <maxmod.h>
#include <tonc.h>
#include <string.h>

#include "sprite.h"
#include "card.h"

void game_init()
{
    
}

void game_update()
{
    static int timer = 0;
    timer++;

    if (timer % 12 == 0)
    {
        card_draw();
    }

    static int hand_index_selected = 0;
    if (key_hit(KEY_LEFT))
    {
        hand_index_selected++;
        hand_index(hand_index_selected);
    }
    if (key_hit(KEY_RIGHT))
    {
        hand_index_selected--;
        hand_index(hand_index_selected);
    }

    if (key_hit(KEY_A))
    {
        hand_select();
    }

    if (key_hit(KEY_SELECT))
    {
        hand_change_sort();
    }
}