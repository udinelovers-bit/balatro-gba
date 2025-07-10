#ifndef GRAPHIC_UTILS_H
#define GRAPHIC_UTILS_H

/* This file contains general utils and wrappers that relate to 
 * graphics/video/vram and generally displaying things on the screen.
 * Mostly wrappers and defines for using tonc.
 * 
 * Note: the code here assumes we're working with a single screenblock 
 * which should be true for this entire game since a screenblock 
 * is enough to contain a full screen (and more)
 * and there isn't any scrolling etc.
 */

/* Reminder:
 *  Screen Base Block is the base for the screenblock entries i.e. tilemap
 *  Character Base Block is the base for the tiles themselves
 */
#define MAIN_BG_SBB 31 
#define MAIN_BG_CBB 1
#define TTE_SBB 30
#define TTE_CBB 0
#define AFFINE_BG_SBB 2
#define AFFINE_BG_CBB 2

/* Dimensions for a screenblock.
 * A 1024 size screenblock is arranged in a grid of 32x32 screen entries
 * Interestingly since each block is 8x8 pixels, the 240x160 GBA screen
 * is smaller than the screenblock, only the top left part of the screenblock
 * is displayed on the screen.
 */
#define SE_ROW_LEN 32
#define SE_COL_LEN 32

// Since y direction goes from the top of the screen to the bottom
#define SE_UP	-1
#define SE_DOWN 1

typedef struct
{
	int left;
	int top;
	int right;
	int bottom;
} Rect;

#define UNDEFINED -1

/* Clears a rect in the main background.
 * The se_rect dimensions need to be in number of tiles.
 */
void main_bg_se_clear_rect(Rect se_rect);

/* Copies a rect in the main background vertically in direction by a single tile.
 * direction must be either SE_UP or SE_DOWN.
 * se_rect dimensions are in number of tiles.
 */
void main_bg_se_copy_rect_1_tile_vert(Rect se_rect, int direction);

// A wrapper for tte_erase_rect that would use the rect struct
void tte_erase_rect_wrapper(Rect rect);

#endif //GRAPHIC_UTILS_H