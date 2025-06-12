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
static enum PlayState play_state = PLAY_PLAYING;

static enum HandType hand_type = NONE;

static CardObject *played[MAX_SELECTION_SIZE] = {NULL};
static CardObject *hand[MAX_HAND_SIZE] = {NULL};
static Card *deck[MAX_DECK_SIZE] = {NULL};
static Card *scored_card = NULL;

static int played_top = -1;
static int hand_top = -1;
static int deck_top = -1;

static int hand_size = 8; // Default hand size is 8
static int cards_drawn = 0;
static int hand_selections = 0;

static int card_focused = 0;

static bool sort_by_suit = false;

// General functions
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
static inline void played_push(CardObject *card_object)
{
    if (played_top >= MAX_SELECTION_SIZE - 1) return;
    played[++played_top] = card_object;
}

static inline CardObject *played_pop()
{
    if (played_top < 0) return NULL;
    return played[played_top--];
}

// Deck stack
static inline void deck_push(Card *card)
{
    if (deck_top >= MAX_DECK_SIZE - 1) return;
    deck[++deck_top] = card;
}

static inline Card *deck_pop()
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

u8 card_get_value(Card *card)
{
    if (card->rank == JACK || card->rank == QUEEN || card->rank == KING)
    {
        return 10; // Face cards are worth 10
    }
    else if (card->rank == ACE)
    {
        return 11; // Ace is worth 11
    }
    else
    {
        return card->rank + 2; // 2-10 are worth their rank + 2 (because rank starts at 0)
    }

    return 0; // Should never reach here, but just in case
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
    card_object->scale = float2fx(1.0f);
    card_object->vscale = float2fx(0.0f);
    card_object->rotation = 0;
    card_object->vrotation = 0;
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
    static int timer = 1;
    timer++;

    if (hand_state == HAND_DRAW && cards_drawn < hand_size)
    {
        if (timer % 10 == 0) // Draw a card every 10 frames
        {
            cards_drawn++;
            card_draw();
        }
    }
    else if (hand_state == HAND_DRAW)
    {
        hand_state = HAND_SELECT; // Change the hand state to select after drawing all the cards
        cards_drawn = 0;
        timer = 1;
    }

    static int played_selections = 0;
    static bool sound_played = false;
    bool discarded_card = false;

    // Cards in hand update loop
    for (int i = hand_top + 1; i >= 0; i--) // Start from the end of the hand and work backwards because that's how Balatro does it
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
                                //timer = 1;

                                hand_y = hand[i]->y;
                                hand_x = hand[i]->x; 
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

                    if (i == 0 && discarded_card == false && timer % 10 == 0)
                    {
                        hand_state = HAND_DRAW;
                        sound_played = false;
                        cards_drawn = 0;
                        hand_selections = 0;
                        timer = 1;
                        break;
                    }

                    break;
                case HAND_PLAY:
                    hand_x = hand_x + (int2fx(i) - int2fx(hand_top) / 2) * -spacing_lut[hand_top];
                    hand_y += (24 << FIX_SHIFT);

                    if (hand[i]->selected && timer % 10 == 0)
                    {
                        hand[i]->selected = false;
                        played_push(hand[i]);
                        sprite_destroy(&hand[i]->sprite);
                        hand[i] = NULL;
                        sort_cards();

                        const int pitch_lut[MAX_SELECTION_SIZE] = {1024, 960, 896, 832, 768};
                        mm_sound_effect sfx_draw = {{SFX_CARD_DRAW}, pitch_lut[cards_drawn], 0, 255, 128,};
                        mmEffectEx(&sfx_draw);

                        hand_top--;
                        hand_selections--;
                        cards_drawn++;

                        timer = 1;
                    }

                    if (i == 0 && timer % 10 == 0)
                    {
                        hand_state = HAND_PLAYING;
                        cards_drawn = 0;
                        hand_selections = 0;
                        timer = 1;
                        played_selections = played_top + 1;
                        
                        switch (hand_type) // select the cards that apply to the hand type
                        {
                            case NONE:
                                break;
                            case HIGH_CARD:
                                int highest_rank_index = 0;

                                for (int i = 0; i <= played_top; i++) // find the card with the highest rank in the hand
                                {                                    
                                    if (played[i]->card->rank > played[highest_rank_index]->card->rank)
                                    {
                                        highest_rank_index = i;
                                    }
                                }

                                played[highest_rank_index]->selected = true;
                                break;
                            case PAIR:
                                for (int i = 0; i <= played_top - 1; i++) // find two cards with the same rank
                                {
                                    if (played[i]->card->rank == played[i + 1]->card->rank)
                                    {
                                        played[i]->selected = true;
                                        played[i + 1]->selected = true;
                                        break;
                                    }
                                }
                                break;
                            case TWO_PAIR:
                                for (int i = 0; i <= played_top - 1; i++) // find two pairs of cards with the same rank
                                {
                                    if (played[i]->card->rank == played[i + 1]->card->rank)
                                    {
                                        played[i]->selected = true;
                                        played[i + 1]->selected = true;
                                    }
                                }
                                break;
                            case THREE_OF_A_KIND:
                                for (int i = 0; i <= played_top - 2; i++) // find three cards with the same rank
                                {
                                    if (played[i]->card->rank == played[i + 1]->card->rank && played[i]->card->rank == played[i + 2]->card->rank)
                                    {
                                        played[i]->selected = true;
                                        played[i + 1]->selected = true;
                                        played[i + 2]->selected = true;
                                        break;
                                    }
                                }
                                break;
                            case STRAIGHT:
                                for (int i = 0; i <= played_top; i++) // find five cards in a row
                                {
                                    played[i]->selected = true; // we don't have to check if cards are being played because a straight needs 5 cards which is the max selection size
                                }
                                break;
                            case FLUSH:
                                for (int i = 0; i <= played_top; i++) // find five cards with the same suit
                                {
                                    played[i]->selected = true; // flush functions the same as straight
                                }
                                break;
                            case FULL_HOUSE:
                                for (int i = 0; i <= played_top; i++) // find three cards with the same rank and two cards with the same rank
                                {
                                    played[i]->selected = true; // full house functions the same as straight
                                }
                                break;
                            case FOUR_OF_A_KIND:
                                for (int i = 0; i <= played_top - 3; i++) // find four cards with the same rank
                                {
                                    if (played[i]->card->rank == played[i + 1]->card->rank && played[i]->card->rank == played[i + 2]->card->rank && played[i]->card->rank == played[i + 3]->card->rank)
                                    {
                                        played[i]->selected = true;
                                        played[i + 1]->selected = true;
                                        played[i + 2]->selected = true;
                                        played[i + 3]->selected = true;
                                        break;
                                    }
                                }
                                break;
                            case STRAIGHT_FLUSH:
                                for (int i = 0; i <= played_top; i++) // find five cards in a row with the same suit
                                {
                                    played[i]->selected = true; // straight flush functions the same as straight
                                }
                                break;
                            case ROYAL_FLUSH:
                                for (int i = 0; i <= played_top; i++) // find five cards in a row with the same suit starting with a 10
                                {
                                    played[i]->selected = true; // royal flush functions the same as straight
                                }
                                break;
                            case FIVE_OF_A_KIND:
                                for (int i = 0; i <= played_top; i++) // find five cards with the same rank
                                {
                                    played[i]->selected = true; // five of a kind functions the same as straight
                                }
                                break;
                            case FLUSH_HOUSE:
                                for (int i = 0; i <= played_top; i++) // find three cards with the same rank and two cards with the same suit
                                {
                                    played[i]->selected = true; // flush house functions the same as straight
                                }
                                break;
                            case FLUSH_FIVE:
                                for (int i = 0; i <= played_top; i++) // find five cards with the same suit
                                {
                                    played[i]->selected = true; // flush five functions the same as straight
                                }
                                break;
                        }
                    }

                    break;
                case HAND_PLAYING: // Don't need to do anything here, just wait for the player to select cards
                    hand_x = hand_x + (int2fx(i) - int2fx(hand_top) / 2) * -spacing_lut[hand_top];
                    hand_y += (24 << FIX_SHIFT);
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

            obj_aff_rotscale(hand[i]->sprite->aff, hand[i]->scale, hand[i]->scale, -hand[i]->vx + hand[i]->rotation); // Apply rotation and scale to the sprite
            sprite_position(hand[i]->sprite, fx2int(hand[i]->x), fx2int(hand[i]->y));
        }
    }

    // Played cards update loop
    for (int i = 0; i <= played_top; i++) // So this one is a bit fucking weird because I have to work kinda backwards for everything because of the order of the pushed cards from the hand to the play stack (also crazy that the company that published Balatro is called "Playstack" and this is a play stack, but I digress)
    {
        if (played[i] != NULL)
        {
            if (played[i]->sprite == NULL)
            {
                played[i]->sprite = sprite_new(ATTR0_SQUARE | ATTR0_4BPP | ATTR0_AFF, ATTR1_SIZE_32, card_sprite_lut[played[i]->card->suit][played[i]->card->rank], 0, i + MAX_HAND_SIZE);
            }

            FIXED played_x = int2fx(120);
            FIXED played_y = int2fx(70);
            FIXED played_scale = float2fx(1.0f);
            FIXED played_rotation = int2fx(0.0f);

            played_x = played_x + (int2fx(played_top - i) - int2fx(played_top) / 2) * -27;

            switch (play_state)
            {
                case PLAY_PLAYING:
                    if (i == 0 && (timer % 10 == 0 || played[played_top - played_selections]->selected == false) && timer > 40)
                    {
                        played_selections--;

                        if (played_selections == 0)
                        {
                            play_state = PLAY_SCORING;
                            timer = 1;
                        }
                    }

                    if (played[i]->selected && played_top - i >= played_selections)
                    {
                        played_y -= (10 << FIX_SHIFT);
                    }
                    break;
                case PLAY_SCORING:
                    if (i == 0 && (timer % 30 == 0) && timer > 40)
                    {
                        // So pretend "played_selections" is now called "scored_cards" and it counts the number of cards that have been scored
                        int scored_cards = 0;
                        for (int j = played_top; j >= 0; j--)
                        {
                            if (played[played_top - j]->selected)
                            {
                                scored_cards++;
                                if (scored_cards > played_selections)
                                {
                                    tte_erase_rect(72, 48, 240, 56);
                                    tte_set_pos(fx2int(played[played_top - j]->x) + 16, 48); // Offset of 16 pixels to center the text on the card
                                    tte_set_special(0x2000); // Set text color to blue from background memory
 
                                    // Write the score to a character buffer variable
                                    char score_buffer[4]; // Assuming the maximum score is 99, we need 4 characters (2 digits + null terminator)
                                    snprintf(score_buffer, sizeof(score_buffer), "%d", card_get_value(played[played_top - j]->card));
                                    tte_write(score_buffer);
                                    
                                    played_selections = scored_cards;
                                    //played[j]->vy += (3 << FIX_SHIFT);
                                    played[played_top - j]->vscale = float2fx(0.3f); // Scale down the card when it's scored
                                    played[played_top - j]->vrotation = float2fx(8.0f); // Rotate the card when it's scored

                                    mm_sound_effect sfx_select = {{SFX_CARD_SELECT}, 1024, 0, 255, 128,};
                                    mmEffectEx(&sfx_select);

                                    scored_card = played[played_top - j]->card; // Set the scored card to the one that was just scored. This allows us to get the value of the card later in game.c
                                    break;
                                }
                            }

                            if (j == 0 && scored_cards == played_selections) // Check if it's the last card 
                            {
                                tte_erase_rect(72, 48, 240, 56);
                                play_state = PLAY_ENDING;
                                timer = 1;
                                played_selections = played_top + 1; // Reset the played selections to the top of the played stack
                                scored_card = NULL; // Reset the scored card
                                break;
                            }
                        }
                    }

                    if (played[i]->selected)
                    {
                        played_y -= (10 << FIX_SHIFT);
                    }
                    break;
                case PLAY_ENDING: // This is the reverse of PLAY_PLAYING. The cards get reset back to their neutral position sequentially
                    if (i == 0 && (timer % 10 == 0 || played[played_top - played_selections]->selected == false) && timer > 40)
                    {
                        played_selections--;

                        if (played_selections == 0)
                        {
                            play_state = PLAY_ENDED;
                            timer = 1;
                        }
                    }

                    if (played[i]->selected && played_top - i <= played_selections - 1)
                    {
                        played_y -= (10 << FIX_SHIFT);
                    }
                    break;
                case PLAY_ENDED: // Basically a copy of HAND_DISCARD
                    if (!discarded_card && played[i] != NULL && timer > 40)
                    {
                        played_x = int2fx(240);
                        played_y = int2fx(70);

                        if (!sound_played)
                        {
                            const int pitch_lut[MAX_SELECTION_SIZE] = {1024, 960, 896, 832, 768};
                            mm_sound_effect sfx_draw = {{SFX_CARD_DRAW}, pitch_lut[cards_drawn], 0, 255, 128,};
                            mmEffectEx(&sfx_draw);
                            sound_played = true;
                        }

                        if (played[i]->x >= played_x)
                        {
                            card_object_destroy(&played[i]);

                            //played_top--; 
                            cards_drawn++; // This technically isn't drawing cards, I'm just reusing the variable
                            sound_played = false;

                            if (i == played_top)
                            {
                                hand_state = HAND_DRAW;
                                play_state = PLAY_PLAYING;
                                cards_drawn = 0;
                                hand_selections = 0;
                                played_selections = 0;
                                played_top = -1; // Reset the played stack
                                break; // Break out of the loop to avoid accessing an invalid index
                            }
                        }

                        discarded_card = true;
                    }

                    break;
            }

            played[i]->vx += (played_x - played[i]->x) / 8;
            played[i]->vy += (played_y - played[i]->y) / 8;

            played[i]->vscale += (played_scale - played[i]->scale) / 8; // Scale up the card when it's played

            played[i]->vrotation += (played_rotation - played[i]->rotation) / 8; // Rotate the card when it's played

            // Set velocity to 0 if it's close enough to the target
            const float epsilon = float2fx(0.01f);
            if (played[i]->vx < epsilon && played[i]->vx > -epsilon && played[i]->vy < epsilon && played[i]->vy > -epsilon)
            {
                played[i]->vx = 0;
                played[i]->vy = 0;

                played[i]->x = played_x;
                played[i]->y = played_y;
            }
            else
            {
                played[i]->vx = (played[i]->vx * 7) / 10;
                played[i]->vy = (played[i]->vy * 7) / 10;

                played[i]->x += played[i]->vx;
                played[i]->y += played[i]->vy;
            }

            // Set scale to 0 if it's close enough to the target
            if (played[i]->vscale < epsilon && played[i]->vscale > -epsilon)
            {
                played[i]->vscale = 0;
                played[i]->scale = played_scale;
            }
            else
            {
                played[i]->vscale = (played[i]->vscale * 7) / 10;
                played[i]->scale += played[i]->vscale;
            }

            // Set rotation to 0 if it's close enough to the target
            if (played[i]->vrotation < epsilon && played[i]->vrotation > -epsilon)
            {
                played[i]->vrotation = 0;
                played[i]->rotation = played_rotation;
            }
            else
            {
                played[i]->vrotation = (played[i]->vrotation * 7) / 10;
                played[i]->rotation += played[i]->vrotation;
            }
        
            obj_aff_rotscale(played[i]->sprite->aff, played[i]->scale, played[i]->scale, -played[i]->vx + played[i]->rotation);
            sprite_position(played[i]->sprite, fx2int(played[i]->x), fx2int(played[i]->y));
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

int hand_get_size()
{
    return hand_top + 1;
}

int hand_get_max_size()
{
    return hand_size;
}

enum HandType hand_get_type()
{
    // Idk if this is how Balatro does it but this is how I'm doing it
    if (hand_selections == 0 || hand_state == HAND_DISCARD)
    {
        hand_type = NONE;
        return hand_type;
    }

    hand_type = HIGH_CARD;

    u8 suits[4] = {0};
    u8 ranks[13] = {0};

    for (int i = 0; i <= hand_top; i++)
    {
        if (hand[i] != NULL && hand[i]->selected)
        {
            suits[hand[i]->card->suit]++;
            ranks[hand[i]->card->rank]++;
        }
    }

    // Check for flush
    for (int i = 0; i < 4; i++)
    {
        if (suits[i] >= MAX_SELECTION_SIZE) // if i add jokers just MAX_SELECTION_SIZE - 1 for four fingers
        {
            hand_type = FLUSH;
            break;
        }
    }

    // Check for straight
    for (int i = 0; i < 9; i++)
    {
        if (ranks[i] && ranks[i + 1] && ranks[i + 2] && ranks[i + 3] && ranks[i + 4])
        {
            if (hand_type == FLUSH)
            {
                hand_type = STRAIGHT_FLUSH;
            }
            else
            {
                hand_type = STRAIGHT;
            }
            break;
        }
    }

    // Check for ace low straight
    if (ranks[ACE] && ranks[TWO] && ranks[THREE] && ranks[FOUR] && ranks[FIVE])
    {
        hand_type = STRAIGHT;
    }

    // Check for royal flush
    if (hand_type == STRAIGHT_FLUSH && ranks[TEN] && ranks[JACK] && ranks[QUEEN] && ranks[KING] && ranks[ACE])
    {
        hand_type = ROYAL_FLUSH;
        return hand_type;
    }

    // Check for straight flush
    if (hand_type == STRAIGHT_FLUSH)
    {
        hand_type = STRAIGHT_FLUSH;
        return hand_type;
    }

    // Check for flush five and five of a kind
    if (hand_type == FLUSH && ranks[FIVE] >= 5)
    {
        hand_type = FLUSH_FIVE;
        return hand_type;
    }
    else if (ranks[FIVE] >= 5)
    {
        return FIVE_OF_A_KIND;
    }
    
    // Check for for of a kind
    for (int i = 0; i < 13; i++)
    {
        if (ranks[i] >= 4)
        {
            hand_type = FOUR_OF_A_KIND;
            return hand_type;
        }
        else if (ranks[i] == 3)
        {   
            if (hand_type == PAIR)
            {
                hand_type = FULL_HOUSE;
                return hand_type;
            }
            else
            {
                hand_type = THREE_OF_A_KIND;
            }
        }
        else if (ranks[i] == 2)
        {
            if (hand_type == THREE_OF_A_KIND)
            {
                hand_type = FULL_HOUSE;
                return hand_type;
            }
            else if (hand_type == PAIR)
            {
                hand_type = TWO_PAIR;
                return hand_type;
            }
            else
            {
                hand_type = PAIR;
            }
        }
    }

    return hand_type;
}

enum HandState hand_get_state()
{
    return hand_state;
}

bool hand_discard()
{
    if (hand_state != HAND_SELECT || hand_selections == 0) return false;
    hand_state = HAND_DISCARD;
    card_focused = 0;
    return true;
}

bool hand_play()
{
    if (hand_state != HAND_SELECT || hand_selections == 0) return false;
    hand_state = HAND_PLAY;
    card_focused = 0;
    return true;
}

// Play functions
enum PlayState play_get_state()
{
    return play_state;
}

Card *play_get_scored_card()
{
    // Return value and set old reference to NULL
    Card *scored_card_temp = scored_card;
    scored_card = NULL; // Reset the scored card so it doesn't get returned again
    return scored_card_temp;
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