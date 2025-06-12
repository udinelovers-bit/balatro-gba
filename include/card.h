#ifndef CARD_H
#define CARD_H

#include <tonc.h>

#include "sprite.h"

// Card suits
#define HEARTS 0
#define DIAMONDS 1
#define CLUBS 2
#define SPADES 3

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

// Hand types
enum HandType
{
    NONE,
    HIGH_CARD,
    PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH,
    FIVE_OF_A_KIND,
    FLUSH_HOUSE,
    FLUSH_FIVE
};

// Hand states
enum HandState
{
    HAND_DRAW,
    HAND_SELECT,
    HAND_DISCARD,
    HAND_PLAY,
    HAND_PLAYING
};

enum PlayState
{
    PLAY_PLAYING,
    PLAY_SCORING,
    PLAY_ENDING,
    PLAY_ENDED
};

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
    FIXED x, y; // position
    FIXED vx, vy; // velocity
    FIXED scale;
    FIXED vscale;
    FIXED rotation;
    FIXED vrotation;
    bool selected;
} CardObject;

// Card methods
Card *card_new(u8 suit, u8 rank);
void card_destroy(Card **card);
u8 card_get_value(Card *card);

// CardObject methods
CardObject *card_object_new(Card *card);
void card_object_destroy(CardObject **card_object);

// Card functions
void card_init();
void card_update();

// Hand functions
void hand_set_focus(int index); // This makes the card at the given index the one that's being looked at
int hand_get_focus(); // This gets the index of the card that's being looked at
void hand_select(); // This lets the player select the card they're looking at for the next action
void hand_change_sort(); // This sorts the hand by suit or rank
int hand_get_size(); // This gets the size of the hand
int hand_get_max_size(); // This gets the maximum size of the hand (this doesn't get the array max, it gets the total amount of cards that you have)
enum HandType hand_get_type(); // This gets the type of the hand
enum HandState hand_get_state(); // This gets the current state of the hand
bool hand_discard(); // This discards the selected cards
bool hand_play(); // This plays the selected cards

// Play functions
enum PlayState play_get_state(); // This gets the current state of the play
Card *play_get_scored_card(); // This gets the card that was scored in the last play

// Deck functions
int deck_get_size(); // This gets the size of the deck
int deck_get_max_size(); // This gets the maximum size of the deck (this doesn't get the array max, it gets the total amount of cards that you have)

#endif // CARD_H