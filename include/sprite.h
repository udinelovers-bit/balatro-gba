#ifndef SPRITE_H
#define SPRITE_H

#include <tonc.h>

typedef struct 
{
    OBJ_ATTR *obj;
    OBJ_AFFINE *aff;
    u32 tid, pb;
    POINT pos;
} Sprite;

// Sprite methods
Sprite *sprite_new(u16 a0, u16 a1, u32 tid, u32 pb, int sprite_index);
Sprite *affine_sprite_new(u16 a0, u16 a1, u32 tid, u32 pb);
void sprite_destroy(Sprite **sprite);
int sprite_get_layer(Sprite *sprite);
INLINE void sprite_position(Sprite *sprite, int x, int y)
{
    sprite->pos.x = x;
    sprite->pos.y = y;

    obj_set_pos(sprite->obj, x, y);
}

// Sprite functions
void sprite_init();
void sprite_draw();

#endif // SPRITE_H