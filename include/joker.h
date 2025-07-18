#ifndef JOKER_H
#define JOKER_H

#define JOKER_TID 880 // Tile ID for the starting index in the tile memory
#define JOKER_PB 4 // Palette bank for the default jokers

#include "sprite.h"
#include "card.h"

#define BASE_EDITION 0
#define FOIL_EDITION 1
#define HOLO_EDITION 2
#define POLY_EDITION 3
#define NEGATIVE_EDITION 4

#define COMMON_JOKER 0
#define UNCOMMON_JOKER 1
#define RARE_JOKER 2
#define LEGENDARY_JOKER 3

#define MAX_JOKERS 2 // The current maximum jokers added

#define DEFAULT_JOKER_ID 0
#define GREEDY_JOKER_ID 1 // This is just an example to show the patern of making joker IDs

typedef struct 
{
    u8 id; // Unique ID for the joker, used to identify different jokers
    u8 modifier; // base, foil, holo, poly, negative
    u8 value;
    u8 rarity;
} Joker;

typedef struct // copy of CardObject in card.h
{
    Joker *joker;
    Sprite *sprite;
    FIXED tx, ty;
    FIXED x, y;
    FIXED vx, vy;
    FIXED tscale;
    FIXED scale;
    FIXED vscale;
    FIXED trotation;
    FIXED rotation;
    FIXED vrotation;
} JokerObject;

typedef struct  // These jokers are triggered after the played hand has finished scoring.
{
    int chips;
    int mult;
    int xmult;
    int money;
    bool retrigger; // Retrigger played hand (e.g. "Dusk" joker, even though on the wiki it says "On Scored" it makes more sense to have it here)
} IndependentEffect;

typedef struct 
{
    int chips;
    int mult;
    int xmult;
    int money;
    bool retrigger; // Retrigger scored card
} OnScoredEffect;

void joker_init();

Joker *joker_new(u8 id);
void joker_destroy(Joker **joker);

// Unique effects like "Four Fingers" or "Credit Card" will be hard coded into game.c with a conditional check for the joker ID from the players owned jokers
// game.c should probably be restructured so most of the variables in it are moved to some sort of global variable header file so they can be easily accessed and modified for the jokers
IndependentEffect joker_independent_effect(Joker *joker);
OnScoredEffect joker_on_scored(Joker *joker, Card *scored_card);

void joker_object_score(JokerObject *joker_object); // This doesn't actually score anything, it just performs an animation and plays a sound effect

#endif // JOKER_H