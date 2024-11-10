#include <check.h>

#include "../brick_game/tetris/backend.h"

void assert_matrix_eq(int mat1[4][4], int mat2[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(mat1[i][j], mat2[i][j]);
    }
  }
}

void initialize_game_with_lines(GameInfo_t *game, GameFigure_t *figures) {
  start_teris(game, figures, "test_highscore.txt");

  // Установка тестовых значений на игровом поле
  for (int y = 0; y < 24; y++) {
    for (int x = 0; x < LENGTH; x++) {
      game->field[y][x] = (y == 15) ? 2 : 0;
    }
  }
}

START_TEST(test_spawn_teris_initialization) {
  GameInfo_t game;
  GameFigure_t figures;
  srand(42);  // Устанавливаем фиксированный seed для предсказуемости

  start_teris(&game, &figures, "test_highscore.txt");

  // Задать фиксированные фигуры для теста
  int test_figure[4][4] = {
      {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  copy_figure(figures.figure0, test_figure);
  copy_figure(figures.figure1, test_figure);
  copy_figure(figures.figure2, test_figure);
  copy_figure(figures.figure3, test_figure);
  copy_figure(figures.figure4, test_figure);
  copy_figure(figures.figure5, test_figure);
  copy_figure(figures.figure6, test_figure);

  // Очистить игровое поле
  for (int i = 0; i < LENGTH * 2 + 4; i++) {
    for (int j = 0; j < LENGTH; j++) {
      game.field[i][j] = 0;
    }
  }

  // Вызвать функцию spawn_teris
  spawn_teris(&game, &figures);

  // Проверить, что следующая фигура была сгенерирована
  ck_assert_int_ge(game.next, 0);
  ck_assert_int_le(game.next, 6);

  // Проверить, что текущая фигура была добавлена в игровое поле
  int expected_field[24][10] = {0};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      expected_field[20 + i][3 + j] = test_figure[i][j];
    }
  }

  for (int i = 0; i < 24; i++) {
    for (int j = 0; j < 10; j++) {
      ck_assert_int_eq(game.field[i][j], expected_field[i][j]);
    }
  }
}
END_TEST

Suite *tetris_spawn_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("test_tetris");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_spawn_teris_initialization);
  suite_add_tcase(s, tc_core);

  return s;
}

START_TEST(test_start_teris_initialization) {
  GameInfo_t game;
  GameFigure_t figures;
  const char *filename = "brick_game/tetris/highscore.txt";

  start_teris(&game, &figures, filename);
  save_high_score("brick_game/tetris/highscore.txt", 500);

  ck_assert_int_eq(game.score, 0);
  ck_assert_int_eq(game.high_score, 500);
  ck_assert_int_eq(game.level, 1);
  ck_assert_int_eq(game.next, -1);
  ck_assert_int_eq(game.speed, 2000);
  ck_assert_int_eq(game.pause, 0);

  // Проверка, что игровое поле инициализировано нулями
  for (int i = 0; i < LENGTH * 2 + 4; i++) {
    for (int j = 0; j < LENGTH; j++) {
      ck_assert_int_eq(game.field[i][j], 0);
    }
  }

  // Здесь можно добавить проверки для figures, если необходимо
}
END_TEST

Suite *tetris_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("test_tetris_start");
  tc_core = tcase_create("test_tetris_start");

  tcase_add_test(tc_core, test_start_teris_initialization);
  suite_add_tcase(s, tc_core);

  return s;
}

START_TEST(test_rotate_matrix_90_right) {
  int mat[4][4] = {
      {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};

  int expected[4][4] = {
      {13, 9, 5, 1}, {14, 10, 6, 2}, {15, 11, 7, 3}, {16, 12, 8, 4}};

  rotate_matrix_90_right(mat);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(mat[i][j], expected[i][j]);
    }
  }
}
END_TEST

START_TEST(test_shift_matrix_to_top_left) {
  int mat[4][4] = {{0, 0, 3, 4}, {0, 0, 7, 8}, {0, 0, 11, 12}, {0, 0, 15, 16}};

  int expected[4][4] = {
      {3, 4, 0, 0}, {7, 8, 0, 0}, {11, 12, 0, 0}, {15, 16, 0, 0}};

  shift_matrix_to_top_left(mat);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(mat[i][j], expected[i][j]);
    }
  }
}
END_TEST

Suite *matrix_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("test_matrix");
  tc_core = tcase_create("test_matrix");

  tcase_add_test(tc_core, test_rotate_matrix_90_right);
  tcase_add_test(tc_core, test_shift_matrix_to_top_left);
  suite_add_tcase(s, tc_core);

  return s;
}

void initialize_test_game(GameInfo_t *game, GameFigure_t *figures) {
  // Инициализация игры
  start_teris(game, figures, "test_highscore.txt");

  // Установить фиксированные фигуры для теста
  int test_figure[4][4] = {
      {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  copy_figure(figures->figure0, test_figure);
  copy_figure(figures->figure1, test_figure);
  copy_figure(figures->figure2, test_figure);
  copy_figure(figures->figure3, test_figure);
  copy_figure(figures->figure4, test_figure);
  copy_figure(figures->figure5, test_figure);
  copy_figure(figures->figure6, test_figure);

  // Спавн начальной фигуры
  spawn_teris(game, figures);
}

START_TEST(test_moving_teris_shift_down) {
  GameInfo_t game;
  GameFigure_t figures;
  int figure_fall = 0;
  int stop_game = 0;

  initialize_test_game(&game, &figures);

  // Начальное положение фигуры
  int initial_position[4][4] = {
      {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  // Проверить начальное положение фигуры
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(game.field[20 + i][3 + j], initial_position[i][j]);
    }
  }

  // Симулировать нажатие клавиши вниз
  moving_teris(&game, KEY_DOWN, &figure_fall, &stop_game);

  // Проверить положение фигуры после перемещения вниз
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(game.field[i][3 + j], initial_position[i][j]);
    }
  }

  // Проверить, что переменная figure_fall осталась 0
  ck_assert_int_eq(figure_fall, 1);

  // Проверить, что переменная stop_game осталась 0
  ck_assert_int_eq(stop_game, 0);
}
END_TEST

Suite *tetris_down_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("test_moving_down_tetris");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_moving_teris_shift_down);
  suite_add_tcase(s, tc_core);

  return s;
}

START_TEST(test_moving_teris_left_down) {
  GameInfo_t game;
  GameFigure_t figures;
  int figure_fall = 0;
  int stop_game = 0;

  initialize_test_game(&game, &figures);

  // Начальное положение фигуры
  int initial_position[4][4] = {
      {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  // Проверить начальное положение фигуры
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(game.field[20 + i][3 + j], initial_position[i][j]);
    }
  }

  moving_teris(&game, KEY_LEFT, &figure_fall, &stop_game);

  // Проверить положение фигуры после перемещения вниз
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(game.field[19 + i][2 + j], initial_position[i][j]);
    }
  }

  // Проверить, что переменная figure_fall осталась 0
  ck_assert_int_eq(figure_fall, 0);

  // Проверить, что переменная stop_game осталась 0
  ck_assert_int_eq(stop_game, 0);
}
END_TEST

Suite *tetris_left_down_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("test_left_tetris");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_moving_teris_left_down);
  suite_add_tcase(s, tc_core);

  return s;
}

START_TEST(test_moving_teris_right_down) {
  GameInfo_t game;
  GameFigure_t figures;
  int figure_fall = 0;
  int stop_game = 0;

  initialize_test_game(&game, &figures);

  // Начальное положение фигуры
  int initial_position[4][4] = {
      {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  // Проверить начальное положение фигуры
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(game.field[20 + i][3 + j], initial_position[i][j]);
    }
  }

  moving_teris(&game, KEY_RIGHT, &figure_fall, &stop_game);

  // Проверить положение фигуры после перемещения вниз
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(game.field[19 + i][4 + j], initial_position[i][j]);
    }
  }

  // Проверить, что переменная figure_fall осталась 0
  ck_assert_int_eq(figure_fall, 0);

  // Проверить, что переменная stop_game осталась 0
  ck_assert_int_eq(stop_game, 0);
}
END_TEST

Suite *tetris_right_down_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("test_right_tetris");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_moving_teris_right_down);
  suite_add_tcase(s, tc_core);

  return s;
}

START_TEST(test_moving_teris_rotate) {
  GameInfo_t game;
  GameFigure_t figures;
  int figure_fall = 0;
  int stop_game = 0;

  initialize_test_game(&game, &figures);

  // Начальное положение фигуры
  int initial_position[4][4] = {
      {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  // Проверить начальное положение фигуры
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(game.field[20 + i][3 + j], initial_position[i][j]);
    }
  }

  moving_teris(&game, 10, &figure_fall, &stop_game);

  int initial_position1[4][4] = {
      {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}};

  // Проверить положение фигуры после перемещения вниз
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(game.field[19 + i][3 + j], initial_position1[i][j]);
    }
  }

  // Проверить, что переменная figure_fall осталась 0
  ck_assert_int_eq(figure_fall, 0);

  // Проверить, что переменная stop_game осталась 0
  ck_assert_int_eq(stop_game, 0);
}
END_TEST

Suite *tetris_rotate_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("test_rotate_tetris");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_moving_teris_rotate);
  suite_add_tcase(s, tc_core);

  return s;
}

START_TEST(test_attaching_tetris_transform_to_blocks) {
  GameInfo_t game;
  GameFigure_t figures;
  int end_game = 0;

  initialize_game_with_lines(&game, &figures);

  // Установка тестовой фигуры
  game.field[22][0] = 1;
  game.field[22][1] = 1;
  game.field[22][2] = 1;
  game.field[22][3] = 1;

  attaching_tetris(&game, &end_game, "test_highscore.txt");

  // Проверка преобразования фигуры в блоки
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(game.field[22][i], 2);
  }

  // Проверка, что остальные блоки не изменились
  for (int y = 0; y < 24; y++) {
    for (int x = 4; x < LENGTH; x++) {
      ck_assert_int_eq(game.field[y][x], 0);
    }
  }
}
END_TEST

START_TEST(test_attaching_tetris_remove_full_lines) {
  GameInfo_t game;
  GameFigure_t figures;
  int end_game = 0;

  initialize_game_with_lines(&game, &figures);

  // Установка полного ряда на 22 линии
  for (int x = 0; x < LENGTH; x++) {
    game.field[14][x] = 2;
  }

  game.field[16][0] = 2;

  attaching_tetris(&game, &end_game, "test_highscore.txt");

  // Проверка удаления полного ряда
  for (int x = 1; x < LENGTH; x++) {
    ck_assert_int_eq(game.field[14][x], 0);
  }

  ck_assert_int_eq(game.field[14][0], 2);
}
END_TEST

START_TEST(test_attaching_tetris_level_up) {
  GameInfo_t game;
  GameFigure_t figures;
  int end_game = 0;

  initialize_game_with_lines(&game, &figures);

  game.score = 1400;  // Значение для перехода на следующий уровень

  attaching_tetris(&game, &end_game, "test_highscore.txt");

  // Проверка увеличения уровня
  ck_assert_int_eq(game.level, 3);
  // Проверка уменьшения скорости
  ck_assert_int_lt(game.speed, 2000);
}
END_TEST

START_TEST(test_attaching_tetris_end_game) {
  GameInfo_t game;
  GameFigure_t figures;
  int end_game = 0;

  initialize_game_with_lines(&game, &figures);

  game.field[19][0] = 2;

  attaching_tetris(&game, &end_game, "test_highscore.txt");

  // Проверка окончания игры
  ck_assert_int_eq(end_game, 1);
}
END_TEST

START_TEST(test_attaching_tetris_high_score) {
  GameInfo_t game;
  GameFigure_t figures;
  int end_game = 0;

  start_teris(&game, &figures, "test_highscore.txt");

  game.score = 5000;  // Установка высокого счёта для теста

  attaching_tetris(&game, &end_game, "test_highscore.txt");

  // Проверка обновления высокого счёта
  ck_assert_int_eq(game.high_score, 5000);

  int high_score = load_high_score("test_highscore.txt");

  ck_assert_int_eq(game.high_score, 5000);
  ck_assert_int_eq(high_score, 5000);
}
END_TEST

Suite *tetris_attaching_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("test_attaching_tetris");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_attaching_tetris_transform_to_blocks);
  tcase_add_test(tc_core, test_attaching_tetris_remove_full_lines);
  tcase_add_test(tc_core, test_attaching_tetris_level_up);
  tcase_add_test(tc_core, test_attaching_tetris_end_game);
  tcase_add_test(tc_core, test_attaching_tetris_high_score);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed = 0;
  Suite *s;
  SRunner *sr;

  s = tetris_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  s = matrix_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  s = tetris_spawn_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  s = tetris_down_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  s = tetris_left_down_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  s = tetris_right_down_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  s = tetris_rotate_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  s = tetris_attaching_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}