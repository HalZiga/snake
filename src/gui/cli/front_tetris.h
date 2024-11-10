#ifndef FRONTTETRIS_H
#define FRONTTETRIS_H

#include "../../brick_game/tetris/backend.h"

void Print_tetris(GameInfo_t *game, GameFigure_t *figures);
void Print_start_tetris(int *stop_game);
void Print_next_figure_tetris(GameInfo_t *game, GameFigure_t *figures);
#endif