#include "joker.h"
#include "util.h"


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

const JokerInfo joker_registry[] = {
    { COMMON_JOKER, 2, default_joker_effect },  // DEFAULT_JOKER_ID = 0
    { COMMON_JOKER, 5, greedy_joker_effect },   // GREEDY_JOKER_ID = 1
    { COMMON_JOKER, 5, lusty_joker_effect },    // etc...
    { COMMON_JOKER, 5, wrathful_joker_effect },
    { COMMON_JOKER, 5, gluttonous_joker_effect },
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
