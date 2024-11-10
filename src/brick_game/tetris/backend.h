#ifndef BACKEND_H
#define BACKEND_H

#include <curses.h>
#include <locale.h>
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#define LENGTH 10

typedef struct {
  int field[LENGTH * 2 + 4][LENGTH];
  int next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct {
  int figure0[4][4];
  int figure1[4][4];
  int figure2[4][4];
  int figure3[4][4];
  int figure4[4][4];
  int figure5[4][4];
  int figure6[4][4];
} GameFigure_t;

void start_teris(GameInfo_t *game, GameFigure_t *figures, const char *filename);
void init_figures(GameFigure_t *figures);

void spawn_teris(GameInfo_t *game, GameFigure_t *figures);

void moving_teris(GameInfo_t *game, int ch, int *figure_fall, int *stop_game);
void move_left(GameInfo_t *game, int where_t_figure[8]);
void move_right(GameInfo_t *game, int where_t_figure[8]);
void move_down(GameInfo_t *game, int where_t_figure[8]);
void move_rotate(GameInfo_t *game, int where_t_figure[8]);

void attaching_tetris(GameInfo_t *game, int *end_game, const char *filename);
void remove_full_lines(GameInfo_t *game);

void Shifting_tetris(GameInfo_t *game, int where_t_figure[8], int *figure_fall);

void rotate_matrix_90_right(int mat[4][4]);
void shift_matrix_to_top_left(int mat[4][4]);
void copy_figure(int basic[4][4], int copy[4][4]);

int load_high_score(const char *filename);
void save_high_score(const char *filename, int high_score);

#endif