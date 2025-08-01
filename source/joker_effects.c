#include "joker.h"
#include "util.h"
#include "hand_analysis.h"
#include <stdlib.h>

static const int JOKER_STENCIL_ID = 16;

static JokerEffect default_joker_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card == NULL) effect.mult = 4;
    return effect;
}

static JokerEffect sinful_joker_effect(Card *scored_card, u8 sinful_suit) {
    JokerEffect effect = {0};
    if (scored_card != NULL && scored_card->suit == sinful_suit)
        effect.mult = 3;
    return effect;
}

static JokerEffect greedy_joker_effect(Joker *joker, Card *scored_card) {
    return sinful_joker_effect(scored_card, DIAMONDS);
}

static JokerEffect lusty_joker_effect(Joker *joker, Card *scored_card) {
    return sinful_joker_effect(scored_card, HEARTS);
}

static JokerEffect wrathful_joker_effect(Joker *joker, Card *scored_card) {
    return sinful_joker_effect(scored_card, SPADES);
}

static JokerEffect gluttonous_joker_effect(Joker *joker, Card *scored_card) {
    return sinful_joker_effect(scored_card, CLUBS);
}

static JokerEffect jolly_joker_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card != NULL)
        return effect; // if card != null, we are not at the end-phase of scoring yet

    // This is really inefficient but the only way at the moment to check for whole-hand conditions
    u8 suits[NUM_SUITS];
    u8 ranks[NUM_RANKS];
    get_played_distribution(ranks, suits);

    if (hand_contains_n_of_a_kind(ranks) >= 2)
        effect.mult = 8;
    return effect;
}

static JokerEffect zany_joker_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card != NULL)
        return effect; // if card != null, we are not at the end-phase of scoring yet

    u8 suits[NUM_SUITS];
    u8 ranks[NUM_RANKS];
    get_played_distribution(ranks, suits);

    if (hand_contains_n_of_a_kind(ranks) >= 3)
        effect.mult = 12;
    return effect;
}

static JokerEffect mad_joker_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card != NULL)
        return effect; // if card != null, we are not at the end-phase of scoring yet

    u8 suits[NUM_SUITS];
    u8 ranks[NUM_RANKS];
    get_played_distribution(ranks, suits);

    if (hand_contains_two_pair(ranks))
        effect.mult = 10;
    return effect;
}

static JokerEffect crazy_joker_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card != NULL)
        return effect; // if card != null, we are not at the end-phase of scoring yet

    u8 suits[NUM_SUITS];
    u8 ranks[NUM_RANKS];
    get_played_distribution(ranks, suits);

    if (hand_contains_straight(ranks))
        effect.mult = 12;
    return effect;
}

static JokerEffect droll_joker_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card != NULL)
        return effect; // if card != null, we are not at the end-phase of scoring yet

    u8 suits[NUM_SUITS];
    u8 ranks[NUM_RANKS];
    get_played_distribution(ranks, suits);

    if (hand_contains_flush(suits))
        effect.mult = 10;
    return effect;
}

static JokerEffect sly_joker_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card != NULL)
        return effect; // if card != null, we are not at the end-phase of scoring yet

    u8 suits[NUM_SUITS];
    u8 ranks[NUM_RANKS];
    get_played_distribution(ranks, suits);

    if (hand_contains_n_of_a_kind(ranks) >= 2)
        effect.chips = 50;
    return effect;
}

static JokerEffect wily_joker_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card != NULL)
        return effect; // if card != null, we are not at the end-phase of scoring yet

    u8 suits[NUM_SUITS];
    u8 ranks[NUM_RANKS];
    get_played_distribution(ranks, suits);

    if (hand_contains_n_of_a_kind(ranks) >= 3)
        effect.chips = 100;
    return effect;
}

static JokerEffect clever_joker_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card != NULL)
        return effect; // if card != null, we are not at the end-phase of scoring yet

    u8 suits[NUM_SUITS];
    u8 ranks[NUM_RANKS];
    get_played_distribution(ranks, suits);

    if (hand_contains_two_pair(ranks))
        effect.chips = 80;
    return effect;
}

static JokerEffect devious_joker_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card != NULL)
        return effect; // if card != null, we are not at the end-phase of scoring yet

    u8 suits[NUM_SUITS];
    u8 ranks[NUM_RANKS];
    get_played_distribution(ranks, suits);

    if (hand_contains_straight(ranks))
        effect.chips = 100;
    return effect;
}

static JokerEffect crafty_joker_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card != NULL)
        return effect; // if card != null, we are not at the end-phase of scoring yet

    u8 suits[NUM_SUITS];
    u8 ranks[NUM_RANKS];
    get_played_distribution(ranks, suits);

    if (hand_contains_flush(suits))
        effect.chips = 80;
    return effect;
}

static JokerEffect half_joker_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card != NULL)
        return effect; // if card != null, we are not at the end-phase of scoring yet

    if (get_played_top() + 1 <= 3) // game.c has hand_get_size() but it's not accesible here
        effect.mult = 20;

    return effect;
}

static JokerEffect joker_stencil_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card != NULL)
        return effect; // if card != null, we are not at the end-phase of scoring yet

    // +1 xmult per empty joker slot...
    int jokers_top = get_jokers_top();
    effect.xmult = (MAX_JOKERS_HELD_SIZE-1) - jokers_top;

    // ...and also each stencil_joker adds +1 xmult
    JokerObject** jokers = get_jokers();
    for (int i = 0; i < jokers_top; i++ )
    {
        if (jokers[i]->joker->id == JOKER_STENCIL_ID)
            effect.xmult++;
    }

    return effect;
}

static JokerEffect misprint_joker_effect(Joker *joker, Card *scored_card) {
    JokerEffect effect = {0};
    if (scored_card != NULL)
        return effect; // if card != null, we are not at the end-phase of scoring yet

    effect.mult = random() % 24;

    return effect;
}

const JokerInfo joker_registry[] = {
    { COMMON_JOKER, 2, default_joker_effect },  // DEFAULT_JOKER_ID = 0
    { COMMON_JOKER, 5, greedy_joker_effect },   // GREEDY_JOKER_ID = 1
    { COMMON_JOKER, 5, lusty_joker_effect },    // etc...
    { COMMON_JOKER, 5, wrathful_joker_effect },
    { COMMON_JOKER, 5, gluttonous_joker_effect },
    { COMMON_JOKER, 3, jolly_joker_effect },
    { COMMON_JOKER, 4, zany_joker_effect },
    { COMMON_JOKER, 4, mad_joker_effect },
    { COMMON_JOKER, 4, crazy_joker_effect },
    { COMMON_JOKER, 4, droll_joker_effect },
    { COMMON_JOKER, 3, sly_joker_effect },
    { COMMON_JOKER, 4, wily_joker_effect },
    { COMMON_JOKER, 4, clever_joker_effect },
    { COMMON_JOKER, 4, devious_joker_effect },
    { COMMON_JOKER, 4, crafty_joker_effect },
    { COMMON_JOKER, 5, half_joker_effect },
    { UNCOMMON_JOKER, 8, joker_stencil_effect },
    { COMMON_JOKER, 4, misprint_joker_effect },
};

static const size_t joker_registry_size = NUM_ELEM_IN_ARR(joker_registry);

const JokerInfo* get_joker_registry_entry(int joker_id) {
    if (joker_id < 0 || (size_t)joker_id >= joker_registry_size) {
        return NULL;
    }
    return &joker_registry[joker_id];
}

size_t get_joker_registry_size(void) {
    return joker_registry_size;
}
