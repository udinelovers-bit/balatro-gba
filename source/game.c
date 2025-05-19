#include "game.h"

#include <maxmod.h>
#include <tonc.h>
#include <string.h>
#include <stdlib.h>

#include "sprite.h"
#include "card.h"

#include "background_gfx.h"
#include "background_play_gfx.h"

static int hand = 4;
static int discard = 4;

static int chips = 0;
static int mult = 0;

static int background = 0;

void change_background(int id)
{
    if (background == id)
    {
        return;
    }
    else if (id == 1) // selecting
    {
        memcpy(&se_mem[30][0], background_gfxMap, background_gfxMapLen);

        // tte_printf("#{P:128,128; cx:0}%d/%d", hand_get_size(), hand_get_max_size()); // Hand size/max size
        // tte_printf("#{P:200,152}%d/%d", deck_get_size(), deck_get_max_size()); // Deck size/max size

        // tte_printf("#{P:32,48;}*%d", 0); // Score

        // tte_printf("#{P:24,80;}%d", 0); // Chips
        // tte_printf("#{P:40,80;}%d", 0); // Mult

        // tte_printf("#{P:16,104; cx:0x2000}%d", hand); // Hand
        // tte_printf("#{P:48,104; cx:0x3000}%d", discard); // Discard

        // tte_printf("#{P:24,120; cx:0x1000}$%d", 4); // Money

        // tte_printf("#{P:48,144}%d", 1); // Round
        // tte_printf("#{P:8,144}%d#{cx:0}/%d", 1, 8); // Ante

        tte_erase_rect(128, 152, 160, 136);
    }
    else if (id == 2) // playing
    {
        memcpy(&se_mem[30][0], background_play_gfxMap, background_play_gfxMapLen);

        // tte_printf("#{P:128,128; cx:0}%d/%d", hand_get_size(), hand_get_max_size()); // Hand size/max size
        // tte_printf("#{P:200,152}%d/%d", deck_get_size(), deck_get_max_size()); // Deck size/max size

        // tte_printf("#{P:32,48;}*%d", 0); // Score

        // tte_printf("#{P:24,80;}%d", 0); // Chips
        // tte_printf("#{P:40,80;}%d", 0); // Mult

        // tte_printf("#{P:16,104; cx:0x2000}%d", hand); // Hand
        // tte_printf("#{P:48,104; cx:0x3000}%d", discard); // Discard

        // tte_printf("#{P:24,120; cx:0x1000}$%d", 4); // Money

        // tte_printf("#{P:48,144}%d", 1); // Round
        // tte_printf("#{P:8,144}%d#{cx:0}/%d", 1, 8); // Ante

        tte_erase_rect(128, 128, 152, 136);
    }

    background = id;
}

void game_init()
{
    change_background(1);

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

        tte_erase_rect(8, 64, 64, 72);

        tte_erase_rect(8, 80, 32, 88);
        tte_erase_rect(40, 80, 64, 88);

        switch (hand_get_type())
        {
        case HIGH_CARD:
            tte_printf("#{P:8,64;}HIGH C");
            chips = 5;
            mult = 1;
            break;
        case PAIR:
            tte_printf("#{P:8,64;}PAIR");
            chips = 10;
            mult = 2;
            break;
        case TWO_PAIR:
            tte_printf("#{P:8,64;}2 PAIR");
            chips = 20;
            mult = 2;
            break;
        case THREE_OF_A_KIND:
            tte_printf("#{P:8,64;}3 OAK");
            chips = 30;
            mult = 3;
            break;
        case STRAIGHT:
            tte_printf("#{P:8,64;}STRT");
            chips = 30;
            mult = 4;
            break;
        case FLUSH:
            tte_printf("#{P:8,64;}FLUSH");
            chips = 35;
            mult = 4;
            break;
        case FULL_HOUSE:
            tte_printf("#{P:8,64;}FULL H");
            chips = 40;
            mult = 4;
            break;
        case FOUR_OF_A_KIND:
            tte_printf("#{P:8,64;}4 OAK");
            chips = 60;
            mult = 7;
            break;
        case STRAIGHT_FLUSH:
            tte_printf("#{P:8,64;}STRT F");
            chips = 100;
            mult = 8;
            break;
        case ROYAL_FLUSH:
            tte_printf("#{P:8,64;}ROYAL F");
            chips = 100;
            mult = 8;
            break;
        case FIVE_OF_A_KIND:
            tte_printf("#{P:8,64;}5 OAK");
            chips = 120;
            mult = 12;
            break;
        case FLUSH_HOUSE:
            tte_printf("#{P:8,64;}FLUSH H");
            chips = 140;
            mult = 14;
            break;
        case FLUSH_FIVE:
            tte_printf("#{P:8,64;}FLUSH 5");
            chips = 160;
            mult = 16;
            break;
        default:
            chips = 0;
            mult = 0;
            break;
        }

        if (chips < 10)
        {
            tte_printf("#{P:24,80;}%d", chips); // Chips
        }
        else if (chips < 100)
        {
            tte_printf("#{P:16,80;}%d", chips);
        }
        else
        {
            tte_printf("#{P:8,80;}%d", chips);
        }

        tte_printf("#{P:40,80;}%d", mult); // Mult
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
            tte_erase_rect(8, 64, 64, 72);
        }
    }

    if (key_hit(KEY_START))
    {
        hand_play();
        change_background(2);
    }

    static int last_hand_size = 0;
    static int last_deck_size = 0;

    if (last_hand_size != hand_get_size() || last_deck_size != deck_get_size())
    {
        if (background == 1)
        {
            tte_printf("#{P:128,128; cx:0}%d/%d", hand_get_size(), hand_get_max_size()); // Hand size/max size
        }
        else if (background == 2)
        {
            tte_printf("#{P:128,152; cx:0}%d/%d", hand_get_size(), hand_get_max_size()); // Hand size/max size
        }

        tte_printf("#{P:200,152}%d/%d", deck_get_size(), deck_get_max_size()); // Deck size/max size

        last_hand_size = hand_get_size();
        last_deck_size = deck_get_size();
    }
}