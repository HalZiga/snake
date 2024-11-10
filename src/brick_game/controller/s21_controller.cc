#include "s21_controller.h"

void snake_controller(s21::UserAction_t signal, s21::Parametrs* paramet) {
  s21::Snake_Game snake;
  snake.check_speed_in_game(paramet, signal);
  snake.setSignal(signal);
  if (signal != s21::Pause) {
    snake.set_parametrs(paramet);
  }
}