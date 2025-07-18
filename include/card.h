#ifndef CARD_H
#define CARD_H

#include <tonc.h>

#include "sprite.h"

// Card suits
#define HEARTS 0
#define DIAMONDS 1
#define CLUBS 2
#define SPADES 3
#define NUM_SUITS 4

// Card ranks
#define TWO 0
#define THREE 1
#define FOUR 2
#define FIVE 3
#define SIX 4
#define SEVEN 5
#define EIGHT 6
#define NINE 7
#define TEN 8
#define JACK 9
#define QUEEN 10
#define KING 11
#define ACE 12
#define NUM_RANKS 13
#define RANK_OFFSET 2 // Because the first rank is 2 and ranks start at 0

// Card types
typedef struct
{
    u8 suit;
    u8 rank;
} Card;

typedef struct
{
    Card *card;
    Sprite *sprite;
    FIXED tx, ty; // target position
    FIXED x, y; // position
    FIXED vx, vy; // velocity
    FIXED tscale;
    FIXED scale;
    FIXED vscale;
    FIXED trotation; // this never gets used so i might remove it later
    FIXED rotation;
    FIXED vrotation;
    bool selected;
} CardObject;

// Card functions
void card_init();

// Card methods
Card *card_new(u8 suit, u8 rank);
void card_destroy(Card **card);
u8 card_get_value(Card *card);

// CardObject methods
CardObject *card_object_new(Card *card);
void card_object_destroy(CardObject **card_object);
void card_object_update(CardObject *card_object); // Update the card object position and scale
void card_object_set_sprite(CardObject *card_object, int layer);

#endif // CARD_H