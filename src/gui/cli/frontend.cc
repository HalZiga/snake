#include "frontend.h"

void check_fast(s21::Parametrs *paramet, s21::UserAction_t signal1) {
  if (signal1 != paramet->dir) {
    paramet->inFast = false;
    paramet->numFast = 0;
  } else if (!paramet->inFast && signal1 == s21::Down) {
    paramet->numFast += 1;
    if (paramet->numFast >= 2) {
      paramet->inFast = true;
    }
  }
}

s21::UserAction_t get_signal(int ch) {
  s21::UserAction_t signal = s21::Skip;
  if (ch == KEY_LEFT) {
    signal = s21::Left;
  } else if (ch == KEY_RIGHT) {
    signal = s21::Right;
  } else if (ch == 27) {
    signal = s21::Pause;
  } else if (ch == ACTION_KEY) {
    signal = s21::Action;
  } else if (ch == KEY_DOWN) {
    signal = s21::Down;
  } else if (ch == KEY_UP) {
    signal = s21::Up;
  }
  return signal;
}

void set_defolt_parametr(s21::Parametrs *parametr) {
  parametr->inGame = false;
  parametr->inFast = false;
  parametr->inPause = false;
  parametr->numFast = 0;
  parametr->level = 1;
  parametr->record = 0;
}

int CreateMenu(const char *options[], int size) {
  int highlight = 0;
  int choice = -1;
  int key;

  while (1) {
    clear();

    for (int i = 0; i < size; i++) {
      if (i == highlight) {
        attron(A_REVERSE);
      }
      mvprintw(i + 1, 1, "%s", options[i]);
      attroff(A_REVERSE);
    }
    key = getch();
    switch (key) {
      case KEY_UP:
        highlight = (highlight - 1 + size) % size;
        break;
      case KEY_DOWN:
        highlight = (highlight + 1) % size;
        break;
      case 10:
        choice = highlight;
        break;
    }
    if (choice != -1) {
      break;
    }
  }
  return choice;
}

void PrintRectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int i_2 = top_y + 1; i_2 < bottom_y; i_2++) {
    MVADDCH(i_2, left_x, ACS_VLINE);
    MVADDCH(i_2, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void PrintEnd(s21::Parametrs parametr) {
  int key;
  clear();
  PrintRectangle(0, BOARDS_BEGIN + BOARD_HEIGHT - 1, 0,
                 BOARDS_BEGIN + BOARD_WIDTH + HUD_WIDTH + 1);
  if (parametr.size_snake == 200) {
    mvprintw(10, 6, "YOU WIN!!!");
  }
  mvprintw(11, 6, "Press Enter");
  mvprintw(12, 6, "You account: %d", parametr.size_snake);
  mvprintw(13, 6, "Record: %d", parametr.record);
  refresh();
  while (1) {
    key = getch();
    if (key == ENTER_KEY) {
      return;
    }
  }
}

void init_color() {
  init_pair(kRed, COLOR_RED, COLOR_RED);
  init_pair(kYellow, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(kGreen, COLOR_GREEN, COLOR_GREEN);
  init_pair(kWhite, COLOR_WHITE, COLOR_WHITE);
  init_pair(kBlue, COLOR_BLUE, COLOR_BLUE);
  init_pair(kOrange, COLOR_RED, COLOR_RED);
  init_pair(kPink, COLOR_RED, COLOR_BLUE);
  init_pair(kViolet, COLOR_BLUE, COLOR_RED);
  init_pair(kGray, COLOR_BLACK, COLOR_BLACK);
  init_pair(kNextfig, COLOR_MAGENTA, COLOR_MAGENTA);
}

void HandleSnakeLoop() {
  s21::Parametrs parametr;
  set_defolt_parametr(&parametr);
  SnakeLoop(&parametr);
  parametr.size_snake -= 4;
  PrintEnd(parametr);
}

void doDrawing_snake(s21::Parametrs *parametr) {
  clear();
  attron(COLOR_PAIR(kWhite));
  for (int i = 0; i < FIELD_WIGTH; i++) {
    for (int j = 0; j < FIELD_HEIGHT; j++) {
      mvaddch(j, i, ' ');
    }
  }
  attroff(COLOR_PAIR(kWhite));
  attron(COLOR_PAIR(kOrange));
  mvaddch(parametr->apple.y_p, parametr->apple.x_p, ' ');
  attroff(COLOR_PAIR(kOrange));
  attron(COLOR_PAIR(kRed));
  for (int i = 0; i < parametr->size_snake; i++) {
    mvaddch(parametr->snake_arr[i].y_p, parametr->snake_arr[i].x_p, ' ');
  }
  attroff(COLOR_PAIR(kRed));
}

void paint_snake_tablo(const s21::Parametrs *parametr) {
  mvprintw(0, 11, "Game: Snake");
  mvprintw(1, 11, "Current level: %d", parametr->level);
  mvprintw(2, 11, "Current account: %d", parametr->size_snake - 4);
  mvprintw(3, 11, "Record: %d", parametr->record);
}

void SnakeLoop(s21::Parametrs *p_parameters) {
  s21::UserAction_t signal = s21::Start;
  int level_in_game = 0;
  int sec_kol = 330000000 - level_in_game * 40000000;
  struct timespec sp_start, sp_end, ts1, ts2 = {0, 0};
  snake_controller(signal, p_parameters);
  signal = s21::Skip;
  while (p_parameters->inGame && p_parameters->size_snake != 200) {
    clock_gettime(CLOCK_MONOTONIC, &sp_start);
    int ch = getch();
    flushinp();
    signal = get_signal(ch);
    if (signal == s21::Down) {
      signal = s21::Up;
    } else if (signal == s21::Up) {
      signal = s21::Down;
    }
    doDrawing_snake(p_parameters);
    paint_snake_tablo(p_parameters);
    refresh();
    snake_controller(signal, p_parameters);
    if (p_parameters->inFast) {
      level_in_game = 10;
    } else {
      level_in_game = p_parameters->level;
    }
    sec_kol = 330000000 - level_in_game * 20000000;
    clock_gettime(CLOCK_MONOTONIC, &sp_end);
    if (sp_end.tv_sec - sp_start.tv_sec <= 0 &&
        (ts2.tv_nsec = sec_kol - (sp_end.tv_nsec - sp_start.tv_nsec)) > 0) {
      nanosleep(&ts2, &ts1);
    }
  }
}

void tetris() {
  const char *high_score_file = "brick_game/tetris/highscore.txt";
  srand(time(0));
  initscr();
  keypad(stdscr, TRUE);  //вкл обработки функциональных клавиш

  GameInfo_t game;
  GameFigure_t figures;
  int stop_game = 0;
  int end_game = 0;
  Print_start_tetris(&stop_game);
  while (stop_game != 1 && end_game == 0) {
    start_teris(&game, &figures, high_score_file);
    while (end_game == 0 && stop_game == 0) {
      spawn_teris(&game, &figures);
      int figure_fell = 0;
      Print_tetris(&game, &figures);
      timeout(300);
      while (figure_fell == 0 && stop_game == 0) {
        timeout(game.speed);
        noecho();
        cbreak();
        int ch = getch();
        while (ch == '\t') {
          game.pause = !game.pause;
          if (game.pause) {
            mvprintw(10, 14, "Game Paused");
            refresh();
          }
          ch = getch();
        }
        if (!game.pause) {
          moving_teris(&game, ch, &figure_fell, &stop_game);
          Print_tetris(&game, &figures);
        }
      }
      attaching_tetris(&game, &end_game, high_score_file);
    }
  }
  endwin();
}

int main(void) {
  WIN_INIT(50);
  setlocale(LC_ALL, "");
  srand(time(NULL));
  init_color();
  GameState state = GSNone;
  const char *options[] = {"Snake", "Tetris", "Exit"};
  int choice;
  while (state != GSExit) {
    choice = CreateMenu(options, 3);
    if (choice == 0) {
      state = GSSnake;
      HandleSnakeLoop();
    } else if (choice == 1) {
      tetris();
    } else {
      state = GSExit;
    }
  }
  endwin();
  return 0;
}