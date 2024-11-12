# **Документация по проекту Snake**

В данном проекте реализованы игра «Тетрис» на языке программирования С игра «Змейка» на языке программирования C++.

## Содержание

- [Общая информация](#общая-информация)
- [Инструкция по работе с играми](#инструкция-по-работе-с-игрой)

## Общая информация

- Программа разработана на языке Си стандарта C11 и на языке C++ стандарта C++17.
- Программа «Тетрис» состоит из двух частей: библиотеки, реализующей логику игры тетрис, и терминального интерфейса с использованием библиотеки `ncurses`.
- Программа «Змейка» состоит из трех частей: библиотеки, реализующей логику игры тетрис, котроллера и терминального интерфейса с использованием библиотеки `ncurses`.
- Игра змейка имеет поддержку дескотопного интерфейса с использованием фреймворка Qt.
- Код библиотеки «Змейки» находится в папке `src/brick_game/snake`.
- Код библиотеки «Тетриса» находится в папке `src/brick_game/tetris`.
- Код с интерфейсом программ находится в папке `src/gui`.
- Сборка программы настроена с помощью Makefile со стандартным набором целей для GNU-программ:      
    - **all: запускт тестов, проверка стиля кода и открытие README**
    - **console: запуск консольной версии приложения**
    - **web: запуск web версии приложения**
    - **test_snake: запуск тестов для библиотеки s21_snake.a**
    - **clean: производится удаление объектных файлов в папке `src` и файлов, генерируемых для выполнения целей gcov_report и test**
    - **dvi: открытие файла с документацией (documentation.md)**
    - **test: запуск тестов для библиотеки s21_tetris.a**
    - **test_snake: запуск тестов для библиотеки s21_snake.a**
    - **gcov_report: запуск тестов для игр змейки и тетриса с отображением процента покрытия кода тестами в виде html странички**
- Обеспечено покрытие библиотеки s21_snake.a unit-тестами, с помощью библиотеки `gtest` (тесты проходят на ОС Darwin/Ubuntu).
- Обеспечено покрытие библиотеки s21_tetris.a unit-тестами, с помощью библиотеки `check` (тесты проходят на ОС Darwin/Ubuntu).
- В игре «Змейка» реализованы следующие механики:
  - Самостоятельное перемещение змейки по полю по истечению игрового таймера;
  - Увеличение змейки на 1 едииницу при "поедании" яблока;
  - Окончание игры при достижении змейкой длины в 200 единиц;
  - Изменение направления движения змейки с помощью стрелок (змейка может поворачивать только налево или направо относительно текущего направления движения);
  - Завершение игры при столкновении змейки с собой или с границой игрового поля.
- В игре «Тетрис» реализованы следующие механики:
  - Вращение фигур;
  - Перемещение фигуры по горизонтали;
  - Ускорение падения фигуры (при зажатии кнопки фигура перемещается до конца вниз);
  - Показ следующей фигуры;
  - Уничтожение заполненных линий;
  - Фигура, после достижения нижней границы поля или соприкосновения с другой фигурой, остановливается. После этого происходит генерация следующей фигуры, показанной на превью, если игра не оканчивается;
  - Завершение игры при достижении верхней границы игрового поля;
  - В игре присутствовуют все виды фигур из классического тетриса.
- Игровое поле соответствует размерам игрового поля консоли — десять «пикселей» в ширину и двадцать «пикселей» в высоту (без дополнительного поля с текущей информацией о игре).
- Пользовательский интерфейс поддерживает отрисовку игрового поля и дополнительной информации.

## Инструкция по работе с играми

- В консольеой версии вначале тебя встречает приветственный экран с возможностью выбора игры.
- В процессе игры ты в любой момент можешь нажать на паузу.
- После завершения игры, твой лучший результат будет сохранен в спциальный файл.

- В web версии сразу запускается игра змейка.

