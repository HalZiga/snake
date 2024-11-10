#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::set_defoult_parametr() {
    parametr.inGame = false;
    parametr.inFast = false;
    parametr.inPause = false;
    parametr.numFast = 0;
    parametr.level = 1;
    parametr.record = 0;
    parametr.dir = s21::Skip;
    parametr.size_snake = 0;
    parametr.apple.x_p = 0;
    parametr.apple.y_p = 0;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    timerId = startTimer(DELAY);
    renderingStartSidebar();
    set_defoult_parametr();
}

void MainWindow::timerEvent(QTimerEvent *e) {
    Q_UNUSED(e);
    if (INGame && game_snake) {
        timeEventSnake();}
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    if (key == Qt::Key_Left) {
        signal = s21::UserAction_t::Left;
    } else if (key == Qt::Key_Up) {
        signal = s21::UserAction_t::Down;
    } else if (key == Qt::Key_Right) {
        signal = s21::UserAction_t::Right;
    } else if (key == Qt::Key_Down) {
        signal = s21::UserAction_t::Up;
    } else if (key == Qt::Key_Q) {
        signal = s21::UserAction_t::Action;
    } else if ((key == Qt::Key_Return || key == Qt::Key_Enter) &&
               INGame == false) {
        signal = s21::UserAction_t::Start;
        INGame = true;

        label->setText("Для паузы:\nнажмите P");
    } else if (key == Qt::Key_P && INGame == true) {
        signal = s21::UserAction_t::Pause;
        if (parametr.inPause) {
            label->setText("Игра.Для паузы:\nнажмите P");
        } else {
            label->setText("Пауза.Для продолжения:\nнажмите P");
        }
    } else if (key == Qt::Key_Escape) {
        gameOver();
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    if (game_snake) {
        doDrawing();}
}


void MainWindow::timeEventSnake() {
    settingUpFPSforSnake();
    snake_controller(signal, &parametr);
    if (parametr.inGame) {
        this->repaint();
    } else {
        gameOver();
    }
    renderingSidebarDuringTheSnakeGame();
    if (parametr.size_snake == FIELD_HEIGHT * FIELD_WIGTH) {
        youwin();
    }
    signal = s21::UserAction_t::Skip;
}

void MainWindow::renderingStartSidebar() {
    label = new QLabel("Для старта игры:\nнажмите Enter", this);
    label->setGeometry(260, 110, 270, 50);
    label1 = new QLabel("Текущий счет : ", this);
    label1->setGeometry(260, 30, 200, 50);
    label2 = new QLabel("Максимальный счет : ", this);
    label2->setGeometry(260, 65, 200, 50);
    label3 = new QLabel("Уровень : ", this);
    label3->setGeometry(260, 0, 200, 50);
    label4 = new QLabel("Скорость игры : ", this);
    label4->setGeometry(260, 150, 200, 50);
}

void MainWindow::renderingSidebarDuringTheSnakeGame() {
    label1->setText("Текущий счет : " + QString::number(parametr.size_snake - 4));
    label2->setText("Максимальный счет : " + QString::number(parametr.record));
    label3->setText("Уровень : " + QString::number(parametr.level));
}

void MainWindow::settingUpFPSforSnake() {
    if (inGamelevel != parametr.level) {
        inGamelevel = parametr.level;
        killTimer(timerId);
        timerId = startTimer(DELAY - inGamelevel * ACCELERATION_COEFFICIENT);
    } else if (FastGame == false && parametr.inFast == true) {
        killTimer(timerId);
        timerId = startTimer(FAST_DELAY);
        FastGame = true;
    } else if (FastGame == true && parametr.inFast == false) {
        killTimer(timerId);
        timerId = startTimer(DELAY - inGamelevel * ACCELERATION_COEFFICIENT);
        FastGame = false;
    }
}

void MainWindow::doDrawing() {
    QPainter qp(this);
    for (int i = 0; i < FIELD_WIGTH; i++) {
        for (int j = 0; j < FIELD_HEIGHT; j++) {
            qp.drawRect(i * PIX_WIGTH, j * PIX_HEIGHT, PIX_WIGTH, PIX_HEIGHT);
        }
    }
    qp.setBrush(Qt::red);
    qp.drawRect(parametr.apple.x_p * PIX_WIGTH, parametr.apple.y_p * PIX_HEIGHT,
                PIX_WIGTH, PIX_HEIGHT);
    for (int i = 1; i < parametr.size_snake; i++) {
        qp.setBrush(Qt::green);
        qp.drawRect(parametr.snake_arr[i].x_p * PIX_WIGTH,
                    parametr.snake_arr[i].y_p * PIX_HEIGHT, PIX_WIGTH, PIX_HEIGHT);
    }
    qp.setBrush(Qt::darkGreen);
    if(parametr.snake_arr[0].x_p >= 0 && parametr.snake_arr[0].x_p < FIELD_WIGTH && parametr.snake_arr[0].y_p >= 0 && parametr.snake_arr[0].y_p < FIELD_HEIGHT){
        qp.drawRect(parametr.snake_arr[0].x_p * PIX_WIGTH,
        parametr.snake_arr[0].y_p * PIX_HEIGHT, PIX_WIGTH, PIX_HEIGHT);}
    
}

void MainWindow::initGameSnake() {
    this->signal = s21::UserAction_t::Start;
    parametr.inGame = false;
    this->setWindowTitle("BreakGame School 21");
    snake_controller(signal, &parametr);
    this->signal = s21::UserAction_t::Pause;
}

void MainWindow::gameOver() {
    INGame = false;
    killTimer(timerId);
    timerId = startTimer(DELAY);
    QMessageBox::StandardButton replay =
        QMessageBox::question(this, "Game over", "Do you want to play again?",
                              QMessageBox::Yes | QMessageBox::No);
    if (replay == QMessageBox::No) {
        QApplication::quit();
    }
    label->setText("Для старта игры:\nнажмите Enter");
}

void MainWindow::youwin() {
    QMessageBox::information(this, "You Win", "You realy win");
    INGame = false;
    initGameSnake();
    label->setText("Для старта игры нажмите Enter");
}

MainWindow::~MainWindow() { delete ui; }
