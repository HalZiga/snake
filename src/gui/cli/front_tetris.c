#include "front_tetris.h"

void Print_start_tetris(int *stop_game) {
  clear();

  for (int i = 0; i < 22; i++) {
    for (int j = 0; j < 30; j++) {
      if (i == 0 || j == 0 || i == 21 || j == 29) mvaddch(i, j, '#');
    }
  }

  mvprintw(6, 6, "To start the game");
  mvprintw(7, 9, "press Enter");
  mvprintw(12, 1, "If you want to end the game");
  mvprintw(13, 9, "press \"esc\"");

  int ch = getch();

  if (ch == 27) {
    *stop_game = 1;
  }

  refresh();
}

void Print_tetris(GameInfo_t *game, GameFigure_t *figures) {
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_WHITE);  //белый текст на белом фоне

  clear();

  for (int i = 0; i < 22; i++) {
    for (int j = 0; j < 12; j++) {
      if (i == 0 || j == 0 || i == 21 || j == 11) {
        mvaddch(i, j, '#');
      } else {
        int cell_value = game->field[i - 1][j - 1];
        if (cell_value == 1 || cell_value == 2) {
          attron(COLOR_PAIR(1));
          mvaddch(21 - i, j, ' ');
          attroff(COLOR_PAIR(1));
        } else {
          mvaddch(21 - i, j, ' ');
        }
      }
    }
  }

  for (int i = 0; i < 22; i++) {
    for (int j = 0; j < 30; j++) {
      if (i == 0 || j == 0 || i == 21 || j == 29) mvaddch(i, j, '#');
    }
  }

  mvprintw(7, 16, "Score: %d", game->score);
  mvprintw(10, 16, "Level: %d", game->level);
  mvprintw(13, 16, "Max score:");
  mvprintw(14, 19, "%d", game->high_score);
  Print_next_figure_tetris(game, figures);

  refresh();
}

void Print_next_figure_tetris(GameInfo_t *game, GameFigure_t *figures) {
  init_pair(1, COLOR_WHITE, COLOR_WHITE);
  int current_figure[4][4] = {
      {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  switch (game->next) {
    case 0:
      copy_figure(current_figure, figures->figure0);
      break;
    case 1:
      copy_figure(current_figure, figures->figure1);
      break;
    case 2:
      copy_figure(current_figure, figures->figure2);
      break;
    case 3:
      copy_figure(current_figure, figures->figure3);
      break;
    case 4:
      copy_figure(current_figure, figures->figure4);
      break;
    case 5:
      copy_figure(current_figure, figures->figure5);
      break;
    case 6:
      copy_figure(current_figure, figures->figure6);
      break;
    default:
      printf("Кака это возможно");
      break;
  }
  for (int i = 3; i >= 0; i--) {
    for (int j = 3; j >= 0; j--) {
      if (current_figure[i][j] == 1) {
        attron(COLOR_PAIR(1));
        mvaddch(4 - i, 17 + j, ' ');
        attroff(COLOR_PAIR(1));
      }
    }
  }
}