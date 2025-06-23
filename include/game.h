#ifndef GAME_H
#define GAME_H

enum GameState
{
    GAME_PLAYING,
    GAME_ROUND_END,
    GAME_SHOP,
    GAME_BLIND_SELECT
};

// Game functions
void game_init();
void game_update();

#endif // GAME_H