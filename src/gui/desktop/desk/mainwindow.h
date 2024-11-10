#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColor>
#include <QDebug>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QRadioButton>

#include "../../../brick_game/controller/s21_controller.h"
#include "../../../brick_game/snake/s21_snake.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void initGameSnake();
    void gameOver();
    void youwin();
    ~MainWindow();

protected:
    void timerEvent(QTimerEvent *e) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    Ui::MainWindow *ui;
    int timerId;
    static constexpr int DELAY = 200;
    static constexpr int DELAY_TETRIS = DELAY * 3;
    static constexpr int FAST_DELAY = 50;
    static constexpr int FIELD_HEIGHT = 20;
    static constexpr int FIELD_WIGTH = 10;
    static constexpr int PIX_HEIGHT = 25;
    static constexpr int PIX_WIGTH = 25;
    static constexpr int ACCELERATION_COEFFICIENT = 10;
    static constexpr int SIZE_FIGURA = 4;
    static constexpr int CORDINATE_X_OF_NEXT_FIGURA = 11;
    static constexpr int CORDINATE_Y_OF_NEXT_FIGURA = 12;
    s21::Parametrs parametr;
    s21::UserAction_t signal;
    QLabel *label;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QRadioButton *radiobutton1;
    bool FastGame = false;
    bool INGame = false;
    bool game_snake = true;
    int inGamelevel = 1;
    void doDrawing();
    void timeEventSnake();
    void renderingStartSidebar();
    void renderingSidebarDuringTheSnakeGame();
    void settingUpFPSforSnake();
    void set_defoult_parametr();
};
#endif // MAINWINDOW_H
