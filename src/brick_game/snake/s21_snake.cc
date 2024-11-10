#include "s21_snake.h"

namespace s21 {

Snake_Game::Snake_Game() {}

void Snake_Game::set_parametrs(Parametrs* paramet) {
  if (!paramet->inPause) {
    if (this->signal == Start) {
      init_defolt_snake_game();
    } else {
      converting_parametr_to_game(paramet);
      check_signal();
      move_snake();
      check_fail_game();
    }
    check_apple_in_game();
    conversion_to_parametrs(paramet);
  }
}

void Snake_Game::check_speed_in_game(Parametrs* paramet, UserAction_t signal1) {
  if (signal1 != paramet->dir) {
    paramet->inFast = false;
    paramet->numFast = 0;
  } else if (!paramet->inFast) {
    paramet->numFast += 1;
    if (paramet->numFast >= 2) {
      paramet->inFast = true;
    }
  }
  if (paramet->inPause == false && signal1 == Pause) {
    paramet->inPause = true;
  } else if (paramet->inPause == true && signal1 == Pause) {
    paramet->inPause = false;
  }
}

void Snake_Game::setSignal(UserAction_t signal1) { this->signal = signal1; }

void Snake_Game::copy_snake(Parametrs* paramet) {
  for (int i = 0; i < paramet->size_snake; i++) {
    paramet->snake_arr[i].x_p = m_snake[i].x_p;
    paramet->snake_arr[i].y_p = m_snake[i].y_p;
  }
}

void Snake_Game::converting_parametr_to_game(Parametrs* paramet) {
  game_status = paramet->inGame;
  m_apple.x_p = paramet->apple.x_p;
  m_apple.y_p = paramet->apple.y_p;
  m_dir = paramet->dir;
  m_snake.resize(paramet->size_snake);
  current_level = paramet->level;
  for (int i = 0; i < paramet->size_snake; i++) {
    m_snake[i].y_p = paramet->snake_arr[i].y_p;
    m_snake[i].x_p = paramet->snake_arr[i].x_p;
  }
}

void Snake_Game::init_defolt_snake_game() {
  game_status = true;
  m_snake.resize(4);
  m_dir = Up;
  current_level = 1;
  for (int i = 0; i < static_cast<int>(m_snake.size()); i++) {
    m_snake[i].x_p = INITION_POSITION_SNAKE_X;
    m_snake[i].y_p = INITION_POSITION_SNAKE_Y - i;
  }
  apple_location();
}

void Snake_Game::apple_location() {
  srand(time(0));
  if (m_snake.size() < 190) {
    m_apple.x_p = rand() % FIELD_WIDTH;
    m_apple.y_p = rand() % FIELD_HEIGHT;
    while (check_apple_in_snake()) {
      m_apple.x_p = rand() % FIELD_WIDTH;
      m_apple.y_p = rand() % FIELD_HEIGHT;
    }
  } else {
    bool flag = false;
    for (int i = 0; i < FIELD_WIDTH && flag == false; i++) {
      for (int j = 0; j < FIELD_HEIGHT && flag == false; j++) {
        m_apple.x_p = i;
        m_apple.y_p = j;
        flag = !(check_apple_in_snake());
      }
    }
  }
}

bool Snake_Game::check_apple_in_snake() {
  bool res = false;
  for (int i = 0; i < static_cast<int>(m_snake.size()) && res == false; i++) {
    if (m_apple.x_p == m_snake[i].x_p && m_apple.y_p == m_snake[i].y_p) {
      res = true;
    }
  }
  return res;
}

void Snake_Game::check_apple_in_game() {
  if (m_apple.x_p == m_snake[0].x_p && m_apple.y_p == m_snake[0].y_p) {
    m_snake.push_back(m_snake.back());
    apple_location();
    if ((m_snake.size() - START_SIZE_SNAKE) % DIFFERENCE_BETWEEN_LEVEL == 0 &&
        current_level < MAX_LEVEL) {
      current_level += 1;
    }
  }
}

void Snake_Game::move_snake() {
  for (int i = m_snake.size() - 1; i > 0; --i) {
    m_snake[i] = m_snake[i - 1];
  }
  if (m_dir == Down) {
    m_snake[0].y_p -= 1;
  } else if (m_dir == Up) {
    m_snake[0].y_p += 1;
  } else if (m_dir == Left) {
    m_snake[0].x_p -= 1;
  } else if (m_dir == Right) {
    m_snake[0].x_p += 1;
  }
}

void Snake_Game::check_fail_game() {
  if (m_snake[0].x_p < 0 || m_snake[0].y_p < 0) {
    game_status = false;
  } else if (m_snake[0].x_p >= FIELD_WIDTH || m_snake[0].y_p >= FIELD_HEIGHT) {
    game_status = false;
  }
  for (int i = 1; i < static_cast<int>(m_snake.size()) && game_status == true;
       i++) {
    if (m_snake[i].x_p == m_snake[0].x_p && m_snake[i].y_p == m_snake[0].y_p) {
      game_status = false;
    }
  }
}

void Snake_Game::check_signal() {
  if (signal == Down && m_dir != Up) {
    m_dir = Down;
  } else if (signal == Up && m_dir != Down) {
    m_dir = Up;
  } else if (signal == Left && m_dir != Right) {
    m_dir = Left;
  } else if (signal == Right && m_dir != Left) {
    m_dir = Right;
  }
}

void Snake_Game::conversion_to_parametrs(Parametrs* paramet) {
  paramet->inGame = game_status;
  paramet->dir = m_dir;
  paramet->size_snake = m_snake.size();
  copy_snake(paramet);
  paramet->apple.x_p = m_apple.x_p;
  paramet->apple.y_p = m_apple.y_p;
  s21::save_record_to_file(paramet->size_snake - 4);
  paramet->record = s21::record_in_file();
  paramet->level = current_level;
}

void save_record_to_file(int current_size) {
  std::ifstream inputFile(".record.txt");
  int record;
  if (inputFile.is_open()) {
    if (inputFile >> record) {
      inputFile.close();
    } else {
      inputFile.close();
      std::ofstream outputFile(".record.txt");
      if (outputFile.is_open()) {
        outputFile << 0;
        outputFile.close();
      }
      return;
    }
    if (record < current_size) {
      std::ofstream outputFile(".record.txt");
      if (outputFile.is_open()) {
        outputFile << current_size;
        outputFile.close();
      }
    }
  } else {
    std::ofstream outputFile(".record.txt");
    if (outputFile.is_open()) {
      outputFile << 0;
      outputFile.close();
    }
  }
}

int record_in_file() {
  std::ifstream inputFile(".record.txt");
  int record = 0;
  if (inputFile.is_open()) {
    if (inputFile >> record) {
      inputFile.close();
    }
  }
  return record;
}

void defoult_parametr(Parametrs* parametr) {
  parametr->inGame = false;
  parametr->inFast = false;
  parametr->inPause = false;
  parametr->numFast = 0;
  parametr->level = 1;
  parametr->record = 0;
  parametr->dir = Skip;
  parametr->size_snake = 0;
  parametr->apple.x_p = 0;
  parametr->apple.y_p = 0;
}

};  // namespace s21