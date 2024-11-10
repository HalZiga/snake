#include "backend.h"

void copy_figure(int basic[4][4], int copy[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      basic[i][j] = copy[i][j];
    }
  }
}

void rotate_matrix_90_right(int mat[4][4]) {
  int temp[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      temp[i][j] = mat[i][j];
    }
  }

  for (int i = 0; i < 4; i++) {  //поворачиваем на 90
    for (int j = 0; j < 4; j++) {
      mat[j][3 - i] = temp[i][j];
    }
  }
}

void shift_matrix_to_top_left(int mat[4][4]) {
  int top_row = 4, left_col = 4;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (mat[i][j] != 0) {
        if (i < top_row) top_row = i;
        if (j < left_col) left_col = j;
      }
    }
  }

  int temp[4][4] = {0};

  for (int i = top_row; i < 4; i++) {
    for (int j = left_col; j < 4; j++) {
      temp[i - top_row][j - left_col] = mat[i][j];
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      mat[i][j] = temp[i][j];
    }
  }
}

void init_figures(GameFigure_t *figures) {
  int temp0[4][4] = {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int temp1[4][4] = {{0, 1, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int temp2[4][4] = {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int temp3[4][4] = {{0, 1, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int temp4[4][4] = {{0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int temp5[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}};
  int temp6[4][4] = {{0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  copy_figure(figures->figure0, temp0);
  copy_figure(figures->figure1, temp1);
  copy_figure(figures->figure2, temp2);
  copy_figure(figures->figure3, temp3);
  copy_figure(figures->figure4, temp4);
  copy_figure(figures->figure5, temp5);
  copy_figure(figures->figure6, temp6);
}

void start_teris(GameInfo_t *game, GameFigure_t *figures,
                 const char *filename) {
  game->score = 0;
  game->high_score = load_high_score(filename);
  game->level = 1;
  game->next = -1;
  game->speed = 2000;
  game->pause = 0;

  // Заполняем игровое поле для примера
  for (int i = 0; i < LENGTH * 2 + 4.; i++) {
    for (int j = 0; j < LENGTH; j++) {
      game->field[i][j] = 0;
    }
  }
  init_figures(figures);
}

void spawn_teris(GameInfo_t *game, GameFigure_t *figures) {
  int num = rand() % 7;
  if (game->next == -1) {
    game->next = rand() % 7;
  } else {
    num = game->next;
    game->next = rand() % 7;
  }
  int current_figure[4][4] = {
      {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  switch (num) {
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
      break;
  }

  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      game->field[20 + i][3 + j] = current_figure[i][j];
    }
  }
}

void moving_teris(GameInfo_t *game, int ch, int *figure_fall, int *stop_game) {
  int where_t_figure[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
  int counter = 0;
  for (int y = 0; y < 24; y++) {
    for (int x = 0; x < LENGTH; x++) {
      if (game->field[y][x] == 1) {
        where_t_figure[counter] = x;
        where_t_figure[counter + 1] = y;
        counter += 2;
      }
    }
  }

  if (ch == 10) {
    move_rotate(game, where_t_figure);
  } else if (ch == 27) {
    *stop_game = 1;
  } else if (ch == KEY_LEFT) {
    move_left(game, where_t_figure);
  } else if (ch == KEY_RIGHT) {
    move_right(game, where_t_figure);
  } else if (ch == KEY_DOWN) {
    move_down(game, where_t_figure);
  }

  Shifting_tetris(game, where_t_figure, figure_fall);
}

void Shifting_tetris(GameInfo_t *game, int where_t_figure[8],
                     int *figure_fall) {
  if (where_t_figure[1] == 0 || where_t_figure[3] == 0) {
    *figure_fall = 1;
  } else if (where_t_figure[1] > 0) {
    for (int x = 0; x < 8; x += 2) {
      if (game->field[where_t_figure[x + 1] - 1][where_t_figure[x]] == 2) {
        *figure_fall = 1;
      }
    }
  }

  if (*figure_fall == 0) {
    for (int y = 0; y < 24; y++) {
      for (int x = 0; x < LENGTH; x++) {
        if (game->field[y][x] == 1) {
          game->field[y][x] = 0;
        }
      }
    }
    for (int i = 1; i < 8; i += 2) {
      where_t_figure[i] -= 1;
    }
    for (int i = 0; i < 8; i += 2) {
      game->field[where_t_figure[i + 1]][where_t_figure[i]] = 1;
    }
    for (int i = 0; i < 8; i += 2) {
      game->field[where_t_figure[i + 1]][where_t_figure[i]] = 1;
    }
  }
}

void move_left(GameInfo_t *game, int where_t_figure[8]) {
  int permit = 0;
  int required_squares = 0;

  for (int y = 0; y < 24; y++) {
    for (int x = 0; x < 10; x++) {
      if (game->field[y][x] == 1) {
        required_squares++;
        if (x - 1 >= 0 && game->field[y][x - 1] != 2) {
          permit++;
        }
        continue;
      }
    }
  }
  if (permit == required_squares) {
    for (int y = 0; y < 24; y++) {
      for (int x = 0; x < LENGTH; x++) {
        if (game->field[y][x] == 1) {
          game->field[y][x] = 0;
        }
      }
    }
    for (int i = 0; i < 8; i += 2) {
      where_t_figure[i] -= 1;
    }
    for (int i = 0; i < 8; i += 2) {
      game->field[where_t_figure[i + 1]][where_t_figure[i]] = 1;
    }
  }
}

void move_down(GameInfo_t *game, int where_t_figure[8]) {
  int count = 0;
  int proverka = 0;

  for (int y = 1; y < 24 && proverka == 0; y++) {
    for (int i = 0; i < 8; i += 2) {
      if (where_t_figure[i + 1] - y < 0 ||
          game->field[where_t_figure[i + 1] - y][where_t_figure[i]] == 2) {
        count = y - 1;
        proverka = 1;
        break;
      }
    }
  }

  if (count != 0) {
    for (int y = 0; y < 24; y++) {
      for (int x = 0; x < LENGTH; x++) {
        if (game->field[y][x] == 1) {
          game->field[y][x] = 0;
        }
      }
    }

    for (int i = 1; i < 8; i += 2) {
      where_t_figure[i] -= count;
    }

    for (int i = 0; i < 8; i += 2) {
      game->field[where_t_figure[i + 1]][where_t_figure[i]] = 1;
    }
  }
}

void move_right(GameInfo_t *game, int where_t_figure[8]) {
  int permit = 0;
  int required_squares = 0;

  for (int y = 0; y < 24; y++) {
    for (int x = 9; x >= 0; x--) {
      if (game->field[y][x] == 1) {
        required_squares++;
        if (x + 1 < 10 && game->field[y][x + 1] != 2) {
          permit++;
        }
        continue;
      }
    }
  }
  if (permit == required_squares) {
    for (int y = 0; y < 24; y++) {
      for (int x = 0; x < LENGTH; x++) {
        if (game->field[y][x] == 1) {
          game->field[y][x] = 0;
        }
      }
    }
    for (int i = 0; i < 8; i += 2) {
      where_t_figure[i] += 1;
    }
    for (int i = 0; i < 8; i += 2) {
      game->field[where_t_figure[i + 1]][where_t_figure[i]] = 1;
    }
  }
}

void move_rotate(GameInfo_t *game, int where_t_figure[8]) {
  int temp[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int line = 30;
  int column = 20;
  for (int i = 0; i < 8; i += 2) {
    if (where_t_figure[i] < column) {
      column = where_t_figure[i];
    }
  }
  for (int i = 1; i < 8; i += 2) {
    if (where_t_figure[i] < line) {
      line = where_t_figure[i];
    }
  }
  for (int i = 0; i < 8; i += 2) {
    temp[where_t_figure[i + 1] - line][where_t_figure[i] - column] = 1;
  }
  rotate_matrix_90_right(temp);
  shift_matrix_to_top_left(temp);
  int can_insert = 0;
  for (int i = 0; i < 4 && can_insert == 0; i++) {
    for (int j = 0; j < 4 && can_insert == 0; j++) {
      if (temp[i][j] == 1) {
        if (column + j > 9 || line + i >= 24 ||
            game->field[line + i][column + j] == 2) {
          can_insert = 1;
        }
      }
    }
  }
  if (can_insert == 0) {
    int counter = 0;
    for (int y = 0; y < 4; y++) {
      for (int x = 0; x < 4; x++) {
        if (temp[y][x] == 1) {
          where_t_figure[counter] = column + x;
          where_t_figure[counter + 1] = line + y;
          counter += 2;
        }
      }
    }
    for (int y = 0; y < 24; y++) {
      for (int x = 0; x < LENGTH; x++) {
        if (game->field[y][x] == 1) {
          game->field[y][x] = 0;
        }
      }
    }
    for (int i = 0; i < 8; i += 2) {
      game->field[where_t_figure[i + 1]][where_t_figure[i]] = 1;
    }
  }
}

void attaching_tetris(GameInfo_t *game, int *end_game, const char *filename) {
  for (int y = 0; y < 24; y++) {
    for (int x = 0; x < LENGTH; x++) {
      if (game->field[y][x] == 1) {
        game->field[y][x] = 2;
      }
    }
  }

  remove_full_lines(game);

  int new_level = game->score / 700 + 1;

  if (new_level > game->level) {
    game->speed = (int)(game->speed * pow(0.9, new_level - game->level));
    game->level = new_level;
  }

  for (int x = 0; x < LENGTH; x++) {
    if (game->field[19][x] == 2) {
      *end_game = 1;
    }
  }

  if (game->score > game->high_score) {
    game->high_score = game->score;
    save_high_score(filename, game->high_score);
  }
}

void remove_full_lines(GameInfo_t *game) {
  int count_f_line = 0;
  int full_line = 0;
  for (int y = 0; y < 24; y++) {
    for (int x = 0; x < LENGTH; x++) {
      if (game->field[y][x] == 2) {
        full_line++;
      }
    }

    if (full_line == 10) {
      count_f_line++;

      for (int x = 0; x < LENGTH; x++) {
        game->field[y][x] = 0;
      }

      for (int x = 0; x < LENGTH; x++) {
        for (int i = y; i < 19; i++) {
          if (game->field[i][x] != 1) {
            game->field[i][x] = game->field[i + 1][x];
          }
        }
      }
      y--;  //чтобы проверить смещенную линию снова
    }
    full_line = 0;
  }

  switch (count_f_line) {
    case 1:
      game->score += 100;
      break;
    case 2:
      game->score += 300;
      break;
    case 3:
      game->score += 700;
      break;
    case 4:
      game->score += 1500;
      break;
    default:
      printf("Кака это возможно");
      break;
  }
}

int load_high_score(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    return 0;
  }

  int high_score;
  fscanf(file, "%d", &high_score);
  fclose(file);
  return high_score;
}

void save_high_score(const char *filename, int high_score) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Ошибка при открытии файла для записи.\n");
    return;
  }

  fprintf(file, "%d", high_score);
  fclose(file);
}