#include "card.h"

#include <maxmod.h>
#include <stdlib.h>

// Audio
#include "soundbank.h"

#define MAX_HAND_SIZE 16
#define MAX_DECK_SIZE 52
#define MAX_SELECTION_SIZE 5

// Card sprites lookup table. First index is the suit, second index is the rank. The value is the tile index.
const static u16 card_sprite_lut[4][13] = {
    {0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192},
    {208, 224, 240, 256, 272, 288, 304, 320, 336, 352, 368, 384, 400},
    {416, 432, 448, 464, 480, 496, 512, 528, 544, 560, 576, 592, 608},
    {624, 640, 656, 672, 688, 704, 720, 736, 752, 768, 784, 800, 816}
};

static enum HandState hand_state = HAND_DRAW;

static CardObject *played[MAX_SELECTION_SIZE] = {NULL};
static CardObject *hand[MAX_HAND_SIZE] = {NULL};
static Card *deck[MAX_DECK_SIZE] = {NULL};

static int played_top = -1;
static int hand_top = -1;
static int deck_top = -1;

static int hand_size = 8; // Default hand size is 8
static int cards_drawn = 0;
static int hand_selections = 0;

static int card_focused = 0;

static bool sort_by_suit = false;

// Sorting functions
static inline void sort_cards()
{
    if (sort_by_suit)
    {
        for (int a = 0; a < hand_top; a++)
        {
            for (int b = a + 1; b <= hand_top; b++)
            {
                if (hand[a] == NULL || (hand[b] != NULL && (hand[a]->card->suit > hand[b]->card->suit || (hand[a]->card->suit == hand[b]->card->suit && hand[a]->card->rank > hand[b]->card->rank))))
                {
                    CardObject *temp = hand[a];
                    hand[a] = hand[b];
                    hand[b] = temp;
                }
            }
        }
    }
    else
    {
        for (int a = 0; a < hand_top; a++)
        {
            for (int b = a + 1; b <= hand_top; b++)
            {
                if (hand[a] == NULL || (hand[b] != NULL && hand[a]->card->rank > hand[b]->card->rank))
                {
                    CardObject *temp = hand[a];
                    hand[a] = hand[b];
                    hand[b] = temp;
                }
            }
        }
    }

    // Update the sprites in the hand by destroying them and creating new ones in the correct order
    // (This is feels like a diabolical solution but like literally how else would you do this)
    for (int i = 0; i <= hand_top; i++)
    {
        if (hand[i] != NULL)
        {
            sprite_destroy(&hand[i]->sprite);
        }
    }

    for (int i = 0; i <= hand_top; i++)
    {
        if (hand[i] != NULL)
        {
            hand[i]->sprite = sprite_new(ATTR0_SQUARE | ATTR0_4BPP | ATTR0_AFF, ATTR1_SIZE_32, card_sprite_lut[hand[i]->card->suit][hand[i]->card->rank], 0, i);
            sprite_position(hand[i]->sprite, fx2int(hand[i]->x), fx2int(hand[i]->y));
        }
    }
}

// Played stack
inline void played_push(CardObject *card_object)
{
    if (played_top >= MAX_SELECTION_SIZE - 1) return;
    played[++played_top] = card_object;
}

inline CardObject *played_pop()
{
    if (played_top < 0) return NULL;
    return played[played_top--];
}

// Deck stack
inline void deck_push(Card *card)
{
    if (deck_top >= MAX_DECK_SIZE - 1) return;
    deck[++deck_top] = card;
}

inline Card *deck_pop()
{
    if (deck_top < 0) return NULL;
    return deck[deck_top--];
}

// Card methods
Card *card_new(u8 suit, u8 rank)
{
    Card *card = malloc(sizeof(Card));

    card->suit = suit;
    card->rank = rank;

    return card;
}

void card_destroy(Card **card)
{
    if (*card == NULL) return;
    free(*card);
    *card = NULL;
}

// CardObject methods
CardObject *card_object_new(Card *card)
{
    CardObject *card_object = malloc(sizeof(CardObject));

    card_object->card = card;
    card_object->sprite = NULL;
    card_object->x = 0;
    card_object->y = 0;
    card_object->vx = 0;
    card_object->vy = 0;
    card_object->selected = false;

    return card_object;
}

void card_object_destroy(CardObject **card_object)
{
    if (*card_object == NULL) return;
    sprite_destroy(&(*card_object)->sprite);
    card_destroy(&(*card_object)->card);
    free(*card_object);
    *card_object = NULL;
}

void card_draw()
{
    if (deck_top < 0 || hand_top >= hand_size - 1 || hand_top >= MAX_HAND_SIZE - 1) return;

    CardObject *card_object = card_object_new(deck_pop());

    const FIXED deck_x = int2fx(208);
    const FIXED deck_y = int2fx(110);

    card_object->x = deck_x;
    card_object->y = deck_y;

    hand[++hand_top] = card_object;

    // Sort the hand after drawing a card
    sort_cards();

    const int pitch_lut[MAX_HAND_SIZE] = {1024, 1048, 1072, 1096, 1120, 1144, 1168, 1192, 1216, 1240, 1264, 1288, 1312, 1336, 1360, 1384};
    mm_sound_effect sfx_draw = {{SFX_CARD_DRAW}, pitch_lut[cards_drawn], 0, 255, 128,};
    mmEffectEx(&sfx_draw);
}

// Card functions
void card_init()
{
    // Fill the deck with all the cards
    for (int suit = 0; suit < 4; suit++)
    {
        for (int rank = 0; rank < 13; rank++)
        {
            Card *card = card_new(suit, rank);
            deck_push(card);
        }
    }

    // Shuffle the deck
    for (int i = 0; i < MAX_DECK_SIZE; i++)
    {
        int j = rand() % MAX_DECK_SIZE;
        Card *temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void card_update() // This whole function is currently pretty unoptimized due to the fixed point math, but that can be fixed later with a LUT
{
    const int update_frame = 10; // This is the number of frames between each card event

    static int timer = 0;
    if (hand_state == HAND_DRAW && cards_drawn < hand_size)
    {
        if (timer % update_frame == 0) // Draw a card every 10 frames
        {
            cards_drawn++;
            card_draw();
        }
        timer++;
    }
    else if (hand_state == HAND_DRAW)
    {
        hand_state = HAND_SELECT; // Change the hand state to select after drawing all the cards
        cards_drawn = 0;
        timer = 0;
    }

    bool discarded_card = false;

    for (int i = 0; i < MAX_HAND_SIZE; i++)
    {
        if (hand[i] != NULL)
        {
            const int spacing_lut[MAX_HAND_SIZE] = {28, 28, 28, 28, 27, 21, 18, 15, 13, 12, 10, 9, 9, 8, 8, 7}; // This is a stupid way to do this but I don't care
            
            FIXED hand_x = int2fx(120);
            FIXED hand_y = int2fx(90);

            switch (hand_state)
            {
                case HAND_DRAW:
                    hand_x = hand_x + (int2fx(i) - int2fx(hand_top) / 2) * -spacing_lut[hand_top];
                    break;
                case HAND_SELECT:
                    if (i == card_focused && !hand[i]->selected)
                    {
                        hand_y -= (10 << FIX_SHIFT);
                    }
                    else if (i != card_focused && hand[i]->selected)
                    {
                        hand_y -= (15 << FIX_SHIFT);
                    }
                    else if (i == card_focused && hand[i]->selected)
                    {
                        hand_y -= (20 << FIX_SHIFT);
                    }

                    if (i != card_focused && hand[i]->y > hand_y)
                    {
                        hand[i]->y = hand_y;
                        hand[i]->vy = 0;
                    }

                    hand_x = hand_x + (int2fx(i) - int2fx(hand_top) / 2) * -spacing_lut[hand_top]; // TODO: Change this later to reference a 2D LUT of positions
                    break;
                case HAND_DISCARD: // TODO: Add sound
                    static bool sound_played = false;

                    if (hand[i]->selected)
                    {
                        if (!discarded_card)
                        {
                            hand_x = int2fx(240);
                            hand_y = int2fx(70);

                            if (!sound_played)
                            {
                                const int pitch_lut[MAX_SELECTION_SIZE] = {1024, 960, 896, 832, 768};
                                mm_sound_effect sfx_draw = {{SFX_CARD_DRAW}, pitch_lut[cards_drawn], 0, 255, 128,};
                                mmEffectEx(&sfx_draw);
                                sound_played = true;
                            }

                            if (hand[i]->x >= hand_x)
                            {
                                card_object_destroy(&hand[i]);
                                sort_cards();                    

                                hand_top--;
                                cards_drawn++; // This technically isn't drawing cards, I'm just reusing the variable
                                sound_played = false;
                            }

                            discarded_card = true;
                        }
                        else
                        {
                            hand_y -= (15 << FIX_SHIFT);
                            hand_x = hand_x + (int2fx(i) - int2fx(hand_top) / 2) * -spacing_lut[hand_top];
                        }
                    }
                    else
                    {
                        hand_x = hand_x + (int2fx(i) - int2fx(hand_top) / 2) * -spacing_lut[hand_top];
                    }

                    if (i == hand_top && discarded_card == false)
                    {
                        hand_state = HAND_DRAW;
                        sound_played = false;
                        cards_drawn = 0;
                        hand_selections = 0;
                        timer = 0;
                    }

                    timer++;
                    break;
                case HAND_PLAY:
                    
                    break;
            }

            hand[i]->vx += (hand_x - hand[i]->x) / 8;
            hand[i]->vy += (hand_y - hand[i]->y) / 8;

            // set velocity to 0 if it's close enough to the target
            const float epsilon = float2fx(0.01f);
            if (hand[i]->vx < epsilon && hand[i]->vx > -epsilon && hand[i]->vy < epsilon && hand[i]->vy > -epsilon)
            {
                hand[i]->vx = 0;
                hand[i]->vy = 0;

                hand[i]->x = hand_x;
                hand[i]->y = hand_y;
            }
            else
            {
                hand[i]->vx = (hand[i]->vx * 7) / 10;
                hand[i]->vy = (hand[i]->vy * 7) / 10;

                hand[i]->x += hand[i]->vx;
                hand[i]->y += hand[i]->vy;
            }

            obj_aff_rotate(hand[i]->sprite->aff, -hand[i]->vx);
            sprite_position(hand[i]->sprite, fx2int(hand[i]->x), fx2int(hand[i]->y));
        }
    }
}

// Hand functions
void hand_set_focus(int index)
{
    if (index < 0 || index > hand_top || hand_state != HAND_SELECT) return;
    card_focused = index;

    mm_sound_effect sfx_focus = {{SFX_CARD_FOCUS}, 1024 + rand() % 512, 0, 255, 128,};
    mmEffectEx(&sfx_focus);
}

int hand_get_focus()
{
    return card_focused;
}

void hand_select()
{
    if (hand_state != HAND_SELECT || hand[card_focused] == NULL) return;

    if (hand[card_focused]->selected)
    {
        hand[card_focused]->selected = false;
        hand_selections--;

        mm_sound_effect sfx_select = {{SFX_CARD_SELECT}, 1024, 0, 255, 128,};
        mmEffectEx(&sfx_select);
    }
    else if (hand_selections < MAX_SELECTION_SIZE)
    {
        hand[card_focused]->selected = true;
        hand_selections++;

        mm_sound_effect sfx_deselect = {{SFX_CARD_DESELECT}, 1024, 0, 255, 128,};
        mmEffectEx(&sfx_deselect);
    }
}

void hand_change_sort()
{
    sort_by_suit = !sort_by_suit;
    sort_cards();
}

bool hand_discard()
{
    if (hand_state != HAND_SELECT || hand_selections == 0) return false;
    hand_state = HAND_DISCARD;
    card_focused = 0;
    return true;
}

int hand_get_size()
{
    return hand_top + 1;
}

int hand_get_max_size()
{
    return hand_size;
}

// Deck functions

int deck_get_size()
{
    return deck_top + 1;
}

int deck_get_max_size()
{
    return MAX_DECK_SIZE; // This shouldn't be the array max size, it should be the total amount of cards that you have but I can't do that until I implement a way for discarded cards to still be stored in ram
}