#include <tonc.h>

#include "joker.h"
#include "joker_gfx.h"
#include "graphic_utils.h"
#include "card.h"

#include <maxmod.h>
#include <stdlib.h>

const static u8 joker_data_lut[MAX_JOKERS][2] = // Rarity, Value
{
    {COMMON_JOKER, 2}, // Default Joker
    {COMMON_JOKER, 5}, // Greedy Joker
};

const static u8 edition_price_lut[MAX_EDITIONS] =
{
    0, // BASE_EDITION
    2, // FOIL_EDITION
    3, // HOLO_EDITION
    5, // POLY_EDITION
    5, // NEGATIVE_EDITION
};

/* So for the card objects, I needed them to be properly sorted
   which is why they let you specify the layer index when creating a new card object.
   Since the cards would overlap a lot in your hand, If they weren't sorted properly, it would look like a mess.
   The joker objects are functionally identical to card objects, so they use the same logic.
   But I'm going to use a simpler approach for the joker objects
   since I'm lazy and sorting them wouldn't look good enough to warrant the effort.
*/
static bool used_layers[MAX_JOKER_OBJECTS] = {false}; // Track used layers for joker sprites

void joker_init()
{
    GRIT_CPY(&tile_mem[4][JOKER_TID], joker_gfxTiles);
    memcpy16(&pal_obj_mem[PAL_ROW_LEN * JOKER_PB], joker_gfxPal, sizeof(joker_gfxPal) / 2);
}

Joker *joker_new(u8 id)
{
    Joker *joker = malloc(sizeof(Joker));

    joker->id = id; // TODO: Make this random later
    joker->modifier = BASE_EDITION; // TODO: Make this random later
    joker->value = joker_data_lut[id][1] + edition_price_lut[joker->modifier]; // Base value + edition price
    joker->rarity = joker_data_lut[id][0];
    joker->proccessed = false;

    return joker;
}

void joker_destroy(Joker **joker)
{
    if (*joker == NULL) return;
    free(*joker);
    *joker = NULL;
}

IndependentEffect joker_independent_effect(Joker *joker)
{
    IndependentEffect effect = {0};

    switch (joker->id)
    {
        case DEFAULT_JOKER_ID: // Default Joker
            effect.mult = 4;
            break;
        case GREEDY_JOKER_ID: // Greedy Joker
            // Nothing because this joker is a scored type
            break;
        default:
            break;
    }

    return effect;
}

// JokerObject methods
JokerObject *joker_object_new(Joker *joker)
{
    JokerObject *joker_object = malloc(sizeof(JokerObject));

    int layer = 0;
    for (int i = 0; i < MAX_JOKER_OBJECTS; i++)
    {
        if (!used_layers[i])
        {
            layer = i;
            used_layers[i] = true; // Mark this layer as used
            break;
        }
    }

    joker_object->joker = joker;
    joker_object->sprite = sprite_new
    (
        ATTR0_SQUARE | ATTR0_4BPP | ATTR0_AFF, 
        ATTR1_SIZE_32, 
        JOKER_TID + (JOKER_SPRITE_OFFSET * joker->id), 
        JOKER_PB,
        JOKER_STARTING_LAYER + layer
    );
    joker_object->tx = 0; // Target position
    joker_object->ty = 0;
    joker_object->x = 0;
    joker_object->y = 0;
    joker_object->vx = 0;
    joker_object->vy = 0;
    joker_object->tscale = float2fx(1.0f); // Target scale
    joker_object->scale = float2fx(1.0f);
    joker_object->vscale = float2fx(0.0f);
    joker_object->trotation = float2fx(0.0f); // Target rotation
    joker_object->rotation = float2fx(0.0f);
    joker_object->vrotation = float2fx(0.0f);
    joker_object->selected = false;

    return joker_object;
}

void joker_object_destroy(JokerObject **joker_object)
{
    if (*joker_object == NULL) return;

    int layer = sprite_get_layer((*joker_object)->sprite) - JOKER_STARTING_LAYER;
    used_layers[layer] = false;
    sprite_destroy(&(*joker_object)->sprite); // Destroy the sprite
    joker_destroy(&(*joker_object)->joker); // Destroy the joker
    free(*joker_object);
    *joker_object = NULL;
}

void joker_object_update(JokerObject *joker_object)
{
    // Re-use the card object update logic by casting the JokerObject to a CardObject temporarily
    // This works because card_object_update doesn't use any card data, which is why CardObject should've been generic. my bad
    // TODO: Refactor and separate the relevant fields to a generic struct, maybe to Sprite or make a SpriteObject
    CardObject *card_object = (CardObject *)joker_object;
    card_object_update(card_object);
}

void joker_object_score(JokerObject *joker_object, mm_word sound_id) // Another derived function of card_object
{
    CardObject *card_object = (CardObject *)joker_object;
    card_object_score(card_object, sound_id);
}
