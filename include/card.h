#ifndef CARD_H
#define CARD_H

#include <tonc.h>

#include "sprite.h"

// Card suits
#define HEARTS 0
#define DIAMONDS 1
#define CLUBS 2
#define SPADES 2

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

typedef struct
{
    u8 suit;
    u8 rank;
} Card;

typedef struct
{
    Card *card;
    Sprite *sprite;
    FIXED x, y; // position
    FIXED vx, vy; // velocity
    bool selected;
} CardObject;

// Card methods
Card *card_new(u8 suit, u8 rank);
void card_destroy(Card **card);

// CardObject methods
CardObject *card_object_new(Card *card);
void card_object_destroy(CardObject **card_object);

// Card functions
void card_init();
void card_update();
void card_draw();

// Hand functions
void hand_index(int index); // This makes the card at the given index the one that's being looked at
void hand_select(); // This lets the player select the card they're looking at for the next action
void hand_change_sort(); // This sorts the hand by suit or rank

#endif // CARD_H