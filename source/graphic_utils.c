#include <tonc_core.h>
#include <tonc_tte.h>
#include <tonc_math.h>

#include "graphic_utils.h"

/* 
 * 
 */
const Rect FULL_SCREENBLOCK_RECT = { 0, 0, SE_ROW_LEN, SE_COL_LEN };


// Clips a rect of screenblock entries to a specified rect
static void clip_se_rect_to_bounding_rect(Rect* rect, const Rect* bounding_rect)
{
    rect->right = min(rect->right, SE_ROW_LEN);
    rect->bottom = min(rect->bottom, SE_COL_LEN);
    rect->left = max(rect->left, 0);
    rect->top = max(rect->top, 0);
}

// Can be unstaticed if needed
// Clips a rect of screenblock entries to screenblock boundaries
// The bounding rect is not required to be within screenblock boundaries
static void clip_se_rect_to_screenblock(Rect* rect)
{
    clip_se_rect_to_bounding_rect(rect, &FULL_SCREENBLOCK_RECT);
}


void main_bg_se_clear_rect(Rect se_rect)
{
    if (se_rect.left > se_rect.right)
        return;
    // Just to make sure we're not overflowing the boundaries
    clip_se_rect_to_screenblock(&se_rect);

    for (int y = se_rect.top; y < se_rect.bottom; y++)
    {
        memset16(&(se_mem[MAIN_BG_SBB][se_rect.left + SE_ROW_LEN * y]), 0x0000, se_rect.right - se_rect.left);
    }
}

void main_bg_se_copy_rect_1_tile_up(Rect se_rect)
{
    if (se_rect.left > se_rect.right)
        return;
    
    Rect bounding_rect = FULL_SCREENBLOCK_RECT;
    bounding_rect.top = 1; // Since we're going up, need to clip to 1 to not overflow
    clip_se_rect_to_bounding_rect(&se_rect, &bounding_rect);

    for (int y = se_rect.top; y < se_rect.bottom; y++)
    {
        memcpy16(&se_mem[MAIN_BG_SBB][se_rect.left + SE_ROW_LEN * (y - 1)], 
			     &se_mem[MAIN_BG_SBB][se_rect.left + SE_ROW_LEN * y], 
                 se_rect.right - se_rect.left);
    }   
}

void tte_erase_rect_wrapper(Rect rect)
{
    tte_erase_rect(rect.left, rect.top, rect.right, rect.bottom);
}

