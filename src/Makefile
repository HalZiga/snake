CXX = g++
FLAGS = -Wextra -Werror -Wall -std=c++17
CC = gcc
CFLAGS = -Wall -Werror -Wextra  -std=c11

SNAKE = brick_game/controller/s21_controller.cc brick_game/snake/s21_snake.cc
TETRIS = brick_game/tetris/backend.c
FRONTEND = gui/cli/frontend.cc gui/cli/front_tetris.c
TESTS = tests/test_main.cc tests/test.cc
TESTS_TETR = tests/test_tetris.c

OS = $(shell uname)

ifeq ($(OS),Linux)
    det_OS = -lcheck -lm -lrt -lpthread -lsubunit -lncurses -lgtest
	project = desk.pro
	inst_s = cd gui && cd desktop && cd desk && qmake  && make && rm Makefile && mkdir -p ../../../build && mv $(project) ../../../build/

else
    det_OS = -lcheck -lm -lpthread -lgtest -lncurses
	project = desk.app
	my_inst = cd ./gui/desktop/desk && qmake desk.pro && make && rm Makefile && mkdir -p ../../../build && mv desk.app ../../../build/
endif

all: install_libraries  gcov_reports style dvi

install_libraries:
	sudo apt-get update
	sudo apt-get install -y libncurses5-dev libncursesw5-dev libcurses-dev check libgtest-dev lcov

cans:
	$(CXX) $(FLAGS) $(SNAKE) $(TETRIS) $(FRONTEND) -o game -lncurses

console:
	$(CXX) $(FLAGS) $(SNAKE) $(TETRIS) $(FRONTEND) -o game -lncurses
	./game
web:
	./gui/desktop/desk/release/desk.exe
	
test_snake: clean
	$(CXX) $(FLAGS) $(SNAKE) $(TESTS) -o test $(det_OS)
	./test
	
gcov_reports: test
	$(CXX) --coverage $(FLAGS) $(SNAKE) $(TESTS) -o test $(det_OS)
	./test
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory report
	open report/index.html

test: clean
	$(CC) $(CFLAGS) --coverage $(TETRIS) $(TESTS_TETR) -o test_tetr $(det_OS)
	./test_tetr

style:
	clang-format -style=google -n  brick_game/controller/* brick_game/snake/*.h \
	brick_game/snake/*.cc gui/cli/* brick_game/tetris/* gui/cli/* tests/*
	
dvi:
	open ../Documentation.md

clean: 
	rm -fr *.o gui/desktop/desk/*.o *.a *.gch *.gcno *.gcda *.info gcov_test s21_test.info ./snake test ../build ../dist *.txt report ./test_tetr rm -rf ../dist/* game .record.txt build