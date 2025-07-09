#include <tonc_core.h>
#include <tonc_tte.h>

#include "graphic_utils.h"

void main_bg_se_clear_rect(Rect rect)
{
    for (int y = rect.top; y < rect.bottom; y++)
    {
        for (int x = rect.left; x < rect.right; x++)
        {
            // I also tried to use se_mat and it didn't work
            memset16(&se_mem[MAIN_BG_SBB][x + SCREENBLOCK_ROW_LEN * y], 0x000, 1);
        }
    }
}

void tte_erase_rect_wrapper(Rect rect)
{
    tte_erase_rect(rect.left, rect.top, rect.right, rect.bottom);
}

