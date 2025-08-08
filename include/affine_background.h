#ifndef AFFINE_BACKGROUND_H
#define AFFINE_BACKGROUND_H

#include <tonc.h>

#include "graphic_utils.h"

#define AFFINE_BG_PAL_LEN 5
#define AFFINE_BG_PB (PAL_ROW_LEN * 15) // This isn't really a palette bank, just the starting index of the palette
#define AFFINE_BG_DEFAULT_COLOR RGB15(11, 18, 15) // 0x3E4B

void affine_background_init();
void affine_background_update();
void affine_background_set_color(COLOR color);

#endif // AFFINE_BACKGROUND_H