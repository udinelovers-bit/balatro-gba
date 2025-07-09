#ifndef GRAPHIC_UTILS_H
#define GRAPHIC_UTILS_H

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

/* A 1024 size screenblock is arranged in a grid of 32x32 block entries
 * Interestingly since each block is 8x8 pixels, the 240x160 GBA screen
 * is smaller than the screenblock, only the top left part of the screenblock
 * is displayed on the screen.
 */
#define SCREENBLOCK_ROW_LEN 32

typedef struct
{
	int left;
	int top;
	int right;
	int bottom;
} Rect;

/* Clears a rect in the main background screenblock.
 * The rect needs to be in screenblocks
 */
void main_bg_se_clear_rect(Rect rect);


#endif //GRAPHIC_UTILS_H