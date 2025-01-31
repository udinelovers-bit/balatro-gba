#include "card.h"

#include <maxmod.h>
#include <stdlib.h>

// Audio
#include "soundbank.h"

#define MAX_HAND_SIZE 16
#define MAX_DECK_SIZE 52

// Card sprites lookup table. First index is the suit, second index is the rank. The value is the tile index.
const static u16 card_sprite_lut[4][13] = {
    {0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192},
    {208, 224, 240, 256, 272, 288, 304, 320, 336, 352, 368, 384, 400},
    {416, 432, 448, 464, 480, 496, 512, 528, 544, 560, 576, 592, 608},
    {624, 640, 656, 672, 688, 704, 720, 736, 752, 768, 784, 800, 816}
};

static CardObject *hand[MAX_HAND_SIZE] = {NULL};
static Card *deck[MAX_DECK_SIZE] = {NULL};

static int deck_top = -1;
static int hand_top = -1;

static int hand_size = 8;
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

    // Update the sprites in the hand by destroying them and creating new ones in the correct order.
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
        }
    }
}

// Deck private functions
void deck_push(Card *card)
{
    if (deck_top >= MAX_DECK_SIZE - 1) return;
    deck[++deck_top] = card;
}

Card *deck_pop()
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
    card_destroy(&(*card_object)->card);
    sprite_destroy(&(*card_object)->sprite);
    free(*card_object);
    *card_object = NULL;
}

void card_draw()
{
    if (deck_top < 0 || hand_top >= hand_size - 1 || hand_top >= MAX_HAND_SIZE - 1) return;

    CardObject *card_object = card_object_new(deck_pop());
    
    static const FIXED deck_x = 208 << FIX_SHIFT;
    static const FIXED deck_y = 110 << FIX_SHIFT;

    card_object->x = deck_x;
    card_object->y = deck_y;

    hand[++hand_top] = card_object;

    // Sort the hand after drawing a card
    sort_cards();

    static mm_sound_effect sfx_draw = {{ SFX_DRAW }, (int)(1.0f * (1<<10)),	0, 255,	128,};

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
    for (int i = 0; i < MAX_HAND_SIZE; i++)
    {
        if (hand[i] != NULL)
        {
            static const int spacing_lut[MAX_HAND_SIZE] = {28, 28, 28, 28, 27, 21, 18, 15, 13, 12, 10, 9, 9, 8, 8, 7}; // This is a stupid way to do this but I don't care
            static const FIXED hand_x = 120 << FIX_SHIFT;
            static const FIXED hand_y = 90 << FIX_SHIFT;

            FIXED hand_y_offset = hand_y;

            if (i == card_focused && !hand[i]->selected)
            {
                hand_y_offset -= (10 << FIX_SHIFT);
            }
            else if (i != card_focused && hand[i]->selected)
            {
                hand_y_offset -= (15 << FIX_SHIFT);
            }
            else if (i == card_focused && hand[i]->selected)
            {
                hand_y_offset -= (20 << FIX_SHIFT);
            }

            hand[i]->vx += (hand_x + (int2fx(i) - int2fx(hand_top) / 2) * -spacing_lut[hand_top] - hand[i]->x) / 8; // TODO: Change this later to reference a 2D LUT of positions
            hand[i]->vy += (hand_y_offset - hand[i]->y) / 8;

            // set velocity to 0 if it's close enough to the target
            if (hand[i]->vx < float2fx(0.01f) && hand[i]->vx > float2fx(-0.01f) && hand[i]->vy < float2fx(0.01f) && hand[i]->vy > float2fx(-0.01f))
            {
                hand[i]->vx = 0;
                hand[i]->vy = 0;

                hand[i]->x = hand_x + (int2fx(i) - int2fx(hand_top) / 2) * -spacing_lut[hand_top];
                hand[i]->y = hand_y_offset;
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
void hand_index(int index)
{
    if (index < 0 || index > hand_top) return;
    card_focused = index;
}

void hand_select()
{
    if (hand[card_focused] == NULL) return;
    hand[card_focused]->selected = !hand[card_focused]->selected;
}

void hand_change_sort()
{
    sort_by_suit = !sort_by_suit;
    sort_cards();
}