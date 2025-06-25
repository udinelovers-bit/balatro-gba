#include "game.h"

#include <maxmod.h>
#include <tonc.h>
#include <string.h>
#include <stdlib.h>

#include "sprite.h"
#include "card.h"

#include "background_gfx.h"
#include "background_play_gfx.h"

static enum GameState game_state = GAME_PLAYING;

static int hand = 4;
static int discard = 4;

static int blind_requirement = 300; // Hard coded for now
static int score = 0;
static int temp_score = 0; // This is the score that shows in the same spot as the hand type.
static FIXED lerped_score = 0;
static FIXED lerped_temp_score = 0;

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

        tte_erase_rect(128, 152, 152, 160);
    }
    else if (id == 2) // playing
    {
        memcpy(&se_mem[30][0], background_play_gfxMap, background_play_gfxMapLen);

        tte_erase_rect(128, 128, 152, 136);
    }

    background = id;
}

void set_chips()
{
    tte_erase_rect(8, 80, 32, 88);

    if (chips < 10)
    {
        tte_printf("#{cx:0; P:24,80;}%d", chips); // Chips
    }
    else if (chips < 100)
    {
        tte_printf("#{cx:0; P:16,80;}%d", chips);
    }
    else
    {
        tte_printf("#{cx:0; P:8,80;}%d", chips);
    }
}

void set_mult()
{
    tte_erase_rect(40, 80, 64, 88);
    tte_printf("#{cx:0; P:40,80;}%d", mult); // Mult
}

void set_hand()
{
    tte_erase_rect(8, 64, 64, 72); // Hand type

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
    case NONE:
        chips = 0;
        mult = 0;
        break;
    }

    set_chips();
    set_mult();
}

void game_init()
{
    change_background(1);

    tte_printf("#{P:128,128; cx:0}%d/%d", hand_get_size(), hand_get_max_size()); // Hand size/max size
    tte_printf("#{P:200,152}%d/%d", deck_get_size(), deck_get_max_size()); // Deck size/max size

    tte_printf("#{P:40,24; cx:0x3000}%d", blind_requirement); // Blind requirement
    tte_printf("#{P:40,32; cx:0x1000}$3"); // Blind reward

    tte_printf("#{P:32,48; cx:0}%d", 0); // Score

    tte_printf("#{P:24,80;}%d", 0); // Chips
    tte_printf("#{P:40,80;}%d", 0); // Mult

    tte_printf("#{P:16,104; cx:0x2000}%d", hand); // Hand
    tte_printf("#{P:48,104; cx:0x3000}%d", discard); // Discard

    tte_printf("#{P:24,120; cx:0x1000}$%d", 4); // Money

    tte_printf("#{P:48,144}%d", 1); // Round
    tte_printf("#{P:8,144}%d#{cx:0}/%d", 1, 8); // Ante
}

void game_playing()
{
    if (hand_get_state() == HAND_DRAW || hand_get_state() == HAND_DISCARD || hand_get_state() == HAND_SELECT)
    {
        change_background(1);
    }
    else
    {
        change_background(2);
    }

    if (score >= blind_requirement) // This is for wrapping everything up at the end of the round
    {
        if (hand_get_state() == HAND_DRAW)
        {
            deck_shuffle();
        }
        else if (hand_get_state() == HAND_SELECT)
        {
            game_state = GAME_ROUND_END;
        }
    }

    if (hand_get_state() == HAND_SELECT)
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
            set_hand();
        }

        if (key_hit(KEY_B))
        {
            hand_change_sort();
        }

        if (key_hit(KEY_SELECT) && discard > 0 && hand_discard())
        {
            set_hand();
            discard--;
            tte_printf("#{P:48,104; cx:0x3000}%d", discard);
        }

        if (key_hit(KEY_START) && hand > 0 && hand_play())
        {
            hand--;
            tte_printf("#{P:16,104; cx:0x2000}%d", hand);
        }
    }
    else if (play_get_state() == PLAY_SCORING)
    {
        Card *scored_card = play_get_scored_card();
        if (scored_card != NULL)
        {
            chips += card_get_value(scored_card);
            set_chips();
        }
    }
    else if (play_get_state() == PLAY_ENDING)
    {
        if (mult > 0)
        {
            temp_score = chips * mult;
            lerped_temp_score = int2fx(temp_score);
            lerped_score = int2fx(score);
            
            tte_erase_rect(8, 64, 64, 72);
            tte_printf("#{P:8,64;}%d", temp_score); // Score

            chips = 0;
            mult = 0;
            set_mult();
            set_chips();
        }
    }
    else if (play_get_state() == PLAY_ENDED)
    {
        lerped_temp_score -= int2fx(temp_score) / 40;
        lerped_score += int2fx(temp_score) / 40;

        if (lerped_temp_score > 0)
        {
            tte_erase_rect(8, 64, 64, 72);
            tte_printf("#{P:8,64;}%d", fx2int(lerped_temp_score)); // Temp Score
            
            // We actually don't need to erase this because the score only increases
            tte_printf("#{P:32,48;}%d", fx2int(lerped_score)); // Score

            if (temp_score <= 0)
            {
                tte_erase_rect(8, 64, 64, 72);
            }
        }
        else
        {
            score += temp_score;
            temp_score = 0;
            lerped_temp_score = 0;
            lerped_score = 0;

            tte_erase_rect(8, 64, 64, 72); // Just erase the temp score
            
            tte_printf("#{P:32,48;}%d", score); // Score
        }
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

void game_round_end()
{
    // Handle round end logic here
    // This could include resetting the game state, updating scores, etc.
}

void game_update()
{
    if (game_state == GAME_PLAYING)
    {
        game_playing();
    }
    else if (game_state == GAME_ROUND_END)
    {
        // Handle round end logic here
    }
    else if (game_state == GAME_SHOP)
    {
        // Handle shop logic here
    }
    else if (game_state == GAME_BLIND_SELECT)
    {
        // Handle blind select logic here
    }
}