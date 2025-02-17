#include "game.h"

#include <maxmod.h>
#include <tonc.h>
#include <string.h>
#include <stdlib.h>

#include "sprite.h"
#include "card.h"

static int hand = 4;
static int discard = 4;

void game_init()
{
    tte_printf("#{P:128,128; cx:0}%d/%d", hand_get_size(), hand_get_max_size()); // Hand size/max size
    tte_printf("#{P:200,152}%d/%d", deck_get_size(), deck_get_max_size()); // Deck size/max size

    tte_printf("#{P:32,48;}*%d", 0); // Score

    tte_printf("#{P:24,80;}%d", 0); // Chips
    tte_printf("#{P:40,80;}%d", 0); // Mult

    tte_printf("#{P:16,104; cx:0x2000}%d", hand); // Hand
    tte_printf("#{P:48,104; cx:0x3000}%d", discard); // Discard

    tte_printf("#{P:24,120; cx:0x1000}$%d", 4); // Money

    tte_printf("#{P:48,144}%d", 1); // Round
    tte_printf("#{P:8,144}%d#{cx:0}/%d", 1, 8); // Ante
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
        if (discard > 0 && hand_discard())
        {
            discard--;
            tte_printf("#{P:48,104; cx:0x3000}%d", discard);
        }
    }

    static int last_hand_size = 0;
    static int last_deck_size = 0;

    if (last_hand_size != hand_get_size() || last_deck_size != deck_get_size())
    {
        tte_printf("#{P:128,128; cx:0}%d/%d", hand_get_size(), hand_get_max_size()); // Hand size/max size
        tte_printf("#{P:200,152}%d/%d", deck_get_size(), deck_get_max_size()); // Deck size/max size

        last_hand_size = hand_get_size();
        last_deck_size = deck_get_size();
    }
}