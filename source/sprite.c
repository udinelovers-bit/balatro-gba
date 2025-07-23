#include "sprite.h"

#include <tonc.h>
#include <stdlib.h>

#define MAX_SPRITES 128
#define MAX_AFFINES 32

OBJ_ATTR obj_buffer[MAX_SPRITES];
OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE*)obj_buffer;

static Sprite *free_sprites[MAX_SPRITES] = {NULL};
static bool free_affines[MAX_AFFINES] = {false};

// Sprite methods
Sprite *sprite_new(u16 a0, u16 a1, u32 tid, u32 pb, int sprite_index)
{
    Sprite *sprite = malloc(sizeof(Sprite));

    sprite->obj = NULL;
    sprite->aff = NULL;

    if(!free_sprites[sprite_index])
    {
        free_sprites[sprite_index] = sprite;
    }
    else
    {
        free(sprite);
        return NULL;
    }

    if (a0 & ATTR0_AFF)
    {
        int aff_index = MAX_AFFINES;

        for(int i = 0; i < MAX_AFFINES; i++)
        {
            if(!free_affines[i])
            {
                free_affines[i] = true;
                aff_index = i;
                break;
            }
        }

        if (aff_index == MAX_AFFINES)
        {
            free(sprite);
            return NULL;
        }

        a1 = a1 | ATTR1_AFF_ID(aff_index);

        sprite->obj = &obj_buffer[sprite_index];
        sprite->aff = &obj_aff_buffer[aff_index];
        obj_set_attr(sprite->obj, a0, a1, ATTR2_PALBANK(pb) | tid);
        obj_aff_identity(&obj_aff_buffer[aff_index]);
        return sprite;
    }
    else
    {
        sprite->obj = &obj_buffer[sprite_index];
        obj_set_attr(sprite->obj, a0, a1, ATTR2_PALBANK(pb) | tid);
        return sprite;
    }
}

void sprite_destroy(Sprite **sprite)
{
    if (*sprite == NULL) return;
    obj_hide((*sprite)->obj);
    free_sprites[(*sprite)->obj - obj_buffer] = NULL;
    if ((*sprite)->aff != NULL)
    {
        free_affines[(*sprite)->aff - obj_aff_buffer] = false;
    }
    free(*sprite);
    *sprite = NULL;
}

int sprite_get_layer(Sprite *sprite)
{
    if (sprite == NULL || sprite->obj == NULL) return -1;
    return sprite->obj - obj_buffer;
}

// Sprite functions
void sprite_init()
{
    oam_init(obj_buffer, MAX_SPRITES); 
}

void sprite_draw()
{
    obj_copy(obj_mem, obj_buffer, MAX_SPRITES);
    obj_aff_copy(obj_aff_mem, obj_aff_buffer, MAX_AFFINES);
    oam_copy(oam_mem, obj_buffer, MAX_SPRITES);
}