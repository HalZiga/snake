#ifndef FRONTEND_H
#define FRONTEND_H

#include <ctype.h>
#include <locale.h>
#include <ncurses.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "../../brick_game/controller/s21_controller.h"

extern "C" {
#include "../../brick_game/tetris/backend.c"
#include "../../brick_game/tetris/backend.h"
#include "front_tetris.c"
#include "front_tetris.h"
}

#define BOARDS_BEGIN (2)
#define BOARD_HEIGHT (20)
#define HUD_WIDTH (12)
#define FIELD_HEIGHT (20)
#define FIELD_WIGTH (10)
#define ENTER_KEY (10)
#define PAUSE_KEY ('p')
#define ACTION_KEY ('q')

#define WIN_INIT(time)     \
  {                        \
    initscr();             \
    noecho();              \
    curs_set(0);           \
    keypad(stdscr, TRUE);  \
    timeout(time);         \
    nodelay(stdscr, TRUE); \
    start_color();         \
  }

#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)

typedef enum {
  GSNone = 0,
  GSSnake = 1,
  GSExit = 2,
} GameState;

typedef enum {
  kRed = 0,
  kYellow = 1,
  kGreen = 2,
  kWhite = 3,
  kOrange = 4,
  kBlue = 5,
  kPink = 6,
  kViolet = 7,
  kGray = 8,
  kNextfig = 9,

} ColorGame;

void PrintBegin();
int CreateMenu(const char *options[], int size);
void PrintRectangle(int top_y, int bottom_y, int left_x, int right_x);
void init_color();
s21::UserAction_t get_signal(int ch);
void check_fast(s21::Parametrs *paramet, s21::UserAction_t signal1);
void PrintEnd(s21::Parametrs parametr);
void set_defolt_parametr(s21::Parametrs *parametr);
void HandleSnakeLoop();
void SnakeLoop(s21::Parametrs *p_parameters);
void doDrawing_snake(s21::Parametrs *parametr);
void paint_snake_tablo(const s21::Parametrs *parametr);

#endif