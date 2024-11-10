#ifndef CPP3_BRICK_GAME_V_2_0_1_BRICK_GAME_SNAKE_S21_SNAKE_H
#define CPP3_BRICK_GAME_V_2_0_1_BRICK_GAME_SNAKE_S21_SNAKE_H

#include <stdbool.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

namespace s21 {

#define BOARD_HEIGHT (20)
#define BOARD_WIDTH (10)

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  Skip
} UserAction_t;

typedef struct my_snake {
  int x_p;
  int y_p;
} My_snake;

typedef struct Parametrs {
  My_snake snake_arr[200];
  My_snake apple;
  UserAction_t dir;
  bool inGame;
  bool inPause;
  bool inFast;
  int numFast;
  int size_snake;
  int record;
  int level;
} Parametrs;

class Snake_Game {
 public:
  Snake_Game();
  void init_defolt_snake_game();
  void converting_parametr_to_game(Parametrs* paramet);
  void apple_location();
  void move_snake();
  void check_fail_game();
  void check_signal();
  void check_apple_in_game();
  void check_speed_in_game(Parametrs* paramet, UserAction_t signal1);
  void copy_snake(Parametrs* paramet);
  void setSignal(UserAction_t signal1);
  void set_parametrs(Parametrs* paramet);
  void conversion_to_parametrs(Parametrs* paramet);
  bool check_apple_in_snake();

 private:
  static constexpr int FIELD_HEIGHT = 20;
  static constexpr int FIELD_WIDTH = 10;
  static constexpr int INITION_POSITION_SNAKE_X = FIELD_WIDTH / 2;
  static constexpr int INITION_POSITION_SNAKE_Y = FIELD_HEIGHT / 2 + 1;
  static constexpr int START_SIZE_SNAKE = 4;
  static constexpr int MAX_LEVEL = 10;
  static constexpr int DIFFERENCE_BETWEEN_LEVEL = 5;
  bool game_status = false;
  int current_level;
  std::vector<My_snake> m_snake;
  UserAction_t m_dir;
  UserAction_t signal = Start;
  My_snake m_apple;
};

void save_record_to_file(int record);
int record_in_file();
void defoult_parametr(Parametrs* parametr);
}  // namespace s21
#endif