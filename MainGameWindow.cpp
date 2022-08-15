
#include "MainGameWindow.h"
#include <QDebug>

MainGameWindow::MainGameWindow(){
    corner_user = new QPoint(120, 70);
    corner_robot = new QPoint(750, 70);

    newGameButton = new QPushButton("New game");
    connect(newGameButton, SIGNAL(clicked(bool)), SLOT(onNewGame()));

    isUsersTurn = true;
    isShootingInProcess = false;
    gameOver = false;

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerAlarm()));
    control = false;

    gameOverLabel = new QLabel;
    QFont font("Calibri", 20);
    gameOverLabel->setFont(font);
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::red);
    gameOverLabel->setPalette(palette);
    gameOverLabel->hide();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(gameOverLabel);
    mainLayout->addWidget(newGameButton);
    mainLayout->setContentsMargins(500, 620, 500, 20);
    setLayout(mainLayout);

    setMouseTracking(true);
}

void MainGameWindow::SetUpWindow(Ship **mas_ships_user_, Ship **mas_ships_robot_) {
    field_user = new Field(mas_ships_user_);
    field_robot = new Field(mas_ships_robot_);

    for (int i = 0; i < 10; i++) {
        mas_ships_user[i] = mas_ships_user_[i];
        mas_ships_robot[i] = mas_ships_robot_[i];
    }
}

void MainGameWindow::paintEvent(QPaintEvent *) {
    PaintEmptyFields();
    PaintShotRobotsShips();
    PaintShips();
    PaintShotUsersShips();
    if (isShootingInProcess) PaintPosRobot();
    if (isUsersTurn) PaintPosUser();
    PaintTriangle();
}

void MainGameWindow::mousePressEvent(QMouseEvent *event) {
    if(gameOver) return;
    if (EventFilter(event)) return;

    srand(time(NULL));
    if (event->button() == Qt::LeftButton) {

        if (field_rect_robot->contains(event->pos())) {
            Coordinates shoot = Coordinates::TranslateCoordinates(event->pos(), *corner_robot, deck_size);
            if (field_robot->matrix[shoot.getX()][shoot.getY()] > 0) {
                int shipNumber = field_robot->matrix[shoot.getX()][shoot.getY()] / 10;
                int deckNumber = field_robot->matrix[shoot.getX()][shoot.getY()] % 10;
                mas_ships_robot[shipNumber - 1]->mas_decks[deckNumber - 1]->SetState(Deck::SHOT_DECK);

                for (int i = 0; i < mas_ships_robot[shipNumber - 1]->size; i++) {
                    if (mas_ships_robot[shipNumber - 1]->mas_decks[i]->GetState() == Deck::SAFE_DECK) break;
                    else if (i == mas_ships_robot[shipNumber - 1]->size - 1) {
                        mas_ships_robot[shipNumber - 1]->SetState(Ship::SHOT_SHIP);

                        for (int j = 0; j < mas_ships_robot[shipNumber - 1]->size; j++) {
                            Deck *cur_deck = mas_ships_robot[shipNumber - 1]->mas_decks[j];
                            Coordinates coord = cur_deck->coord;
                            for (int l = -1; l <= 1; l++) {
                                for (int k = -1; k <= 1; k++) {
                                    if (coord.getX() + l >= 0 && coord.getX() + l < 10 && coord.getY() + k >= 0 &&
                                        coord.getY() + k < 10) {
                                        if (field_robot->matrix[coord.getX() + l][coord.getY() + k] != -2)
                                            field_robot->matrix[coord.getX() + l][coord.getY() + k] = -1;
                                    }
                                }
                            }
                        }
                        if(isGameOver(mas_ships_robot)){
                            gameOverLabel->setText("Victory!");
                            gameOverLabel->show();
                        }
                    }
                }

                field_robot->matrix[shoot.getX()][shoot.getY()] = -2;
            } else if (field_robot->matrix[shoot.getX()][shoot.getY()] == 0) {
                field_robot->matrix[shoot.getX()][shoot.getY()] = -1;
                isUsersTurn = false;
            }
            repaint();
        }

        while (!isUsersTurn) {
            control = true;
            Sleep(1500);

            do {
                suggestedX = rand() % 10;
                suggestedY = rand() % 10;
            } while (field_user->matrix[suggestedX][suggestedY] < 0);

            isShootingInProcess = true;
            repaint();
            isShootingInProcess = false;
            Sleep(1000);
            timer->start(100);

            if (field_user->matrix[suggestedX][suggestedY] > 0) {
                int shipNumber = field_user->matrix[suggestedX][suggestedY] / 10;
                int deckNumber = field_user->matrix[suggestedX][suggestedY] % 10;
                mas_ships_user[shipNumber - 1]->mas_decks[deckNumber - 1]->SetState(Deck::SHOT_DECK);

                for (int i = 0; i < mas_ships_user[shipNumber - 1]->size; i++) {
                    if (mas_ships_user[shipNumber - 1]->mas_decks[i]->GetState() == Deck::SAFE_DECK) break;
                    else if (i == mas_ships_user[shipNumber - 1]->size - 1) {
                        mas_ships_user[shipNumber - 1]->SetState(Ship::SHOT_SHIP);

                        for (int j = 0; j < mas_ships_user[shipNumber - 1]->size; j++) {
                            Deck *cur_deck = mas_ships_user[shipNumber - 1]->mas_decks[j];
                            Coordinates coord = cur_deck->coord;
                            for (int l = -1; l <= 1; l++) {
                                for (int k = -1; k <= 1; k++) {
                                    if (coord.getX() + l >= 0 && coord.getX() + l < 10 && coord.getY() + k >= 0 &&
                                        coord.getY() + k < 10) {
                                        if (field_user->matrix[coord.getX() + l][coord.getY() + k] != -2)
                                            field_user->matrix[coord.getX() + l][coord.getY() + k] = -1;
                                    }
                                }
                            }
                        }
                        if(isGameOver(mas_ships_user)){
                            gameOverLabel->setText("Defeat!");
                            gameOverLabel->show();
                        }

                    }
                }
                field_user->matrix[suggestedX][suggestedY] = -2;
            }else if (field_user->matrix[suggestedX][suggestedY] == 0) {
                    field_user->matrix[suggestedX][suggestedY] = -1;
                    isUsersTurn = true;
                }

                repaint();
            }
        }
    }

void MainGameWindow::mouseMoveEvent(QMouseEvent *event) {
    if(gameOver) return;
    if (isUsersTurn && field_rect_robot->contains(event->pos()))
        suggestedCoord = Coordinates::TranslateCoordinates(event->pos(), *corner_robot, deck_size);
    else suggestedCoord = Coordinates(-1, -1);
    update();
}

void MainGameWindow::PaintEmptyFields() {

    QPainter painter(this);
    QPen blackPen(QColorConstants::Svg::black);
    painter.setPen(blackPen);

    QSize field_size(length, length);

    field_rect_user = new QRect(*corner_user, field_size);
    painter.drawRect(*field_rect_user);

    for (int i = 1; i < 10; i++) {
        painter.drawLine(*corner_user + QPoint(deck_size * i, 0), *corner_user + QPoint(deck_size * i, length));
        painter.drawLine(*corner_user + QPoint(0, deck_size * i), *corner_user + QPoint(length, deck_size * i));
    }

    painter.setFont(QFont("Calibri", 15, QFont::Normal));
    for (int i = 0; i < 9; i++) {
        painter.drawText(*corner_user + QPoint(-deck_size * 0.7, deck_size * (i + 0.8)), QString::number(i + 1));
    }
    painter.drawText(*corner_user + QPoint(-deck_size, deck_size * (9.8)), QString::number(10));

    for (char i = 65; i < 75; i++) {
        painter.drawText(*corner_user + QPoint(deck_size * (i - 65 + 0.3), -deck_size * 0.2), QString(QChar(i)));
    }


    field_rect_robot = new QRect(*corner_robot, field_size);
    painter.drawRect(*field_rect_robot);

    for (int i = 1; i < 10; i++) {
        painter.drawLine(*corner_robot + QPoint(deck_size * i, 0), *corner_robot + QPoint(deck_size * i, length));
        painter.drawLine(*corner_robot + QPoint(0, deck_size * i), *corner_robot + QPoint(length, deck_size * i));
    }

    for (int i = 0; i < 9; i++) {
        painter.drawText(*corner_robot + QPoint(-deck_size * 0.7, deck_size * (i + 0.8)), QString::number(i + 1));
    }
    painter.drawText(*corner_robot + QPoint(-deck_size, deck_size * (9.8)), QString::number(10));

    for (char i = 65; i < 75; i++) {
        painter.drawText(*corner_robot + QPoint(deck_size * (i - 65 + 0.3), -deck_size * 0.2), QString(QChar(i)));
    }
}

void MainGameWindow::PaintShips() {
    QPainter painter(this);
    QPen bluePen(QColorConstants::Svg::blue, 2);
    QPen darkBluePen(QColorConstants::Svg::darkblue);
    QPen redPen(QBrush(QColorConstants::Svg::red), 2);
    QBrush blueBrush(QColorConstants::Svg::lightskyblue);
    QBrush darkBlueBrush(QColorConstants::Svg::darkblue);

    for (int column = 0; column < 10; column++) {
        for (int row = 0; row < 10; row++) {
            if (field_user->matrix[column][row] > 0) {

                painter.setPen(bluePen);
                painter.setBrush(blueBrush);
                painter.drawRect(corner_user->x() + deck_size * column, corner_user->y() + deck_size * row,
                                 deck_size,
                                 deck_size);

            } else if (field_user->matrix[column][row] == -2) {

                painter.setPen(bluePen);
                painter.setBrush(blueBrush);
                painter.drawRect(corner_user->x() + deck_size * column, corner_user->y() + deck_size * row,
                                 deck_size,
                                 deck_size);

                painter.setPen(redPen);
                painter.drawLine(
                        QPoint(corner_user->x() + deck_size * column + 2, corner_user->y() + deck_size * row + 2),
                        QPoint(corner_user->x() + deck_size * (column + 1) - 2,
                               corner_user->y() + deck_size * (row + 1) - 2));
                painter.drawLine(
                        QPoint(corner_user->x() + deck_size * (column + 1) - 2, corner_user->y() + deck_size * row + 2),
                        QPoint(corner_user->x() + deck_size * (column) + 2,
                               corner_user->y() + deck_size * (row + 1) - 2));
            } else if (field_user->matrix[column][row] == -1) {

                painter.setPen(darkBluePen);
                painter.setBrush(darkBlueBrush);
                painter.drawEllipse(QPoint(corner_user->x() + deck_size * column + deck_size * 0.5,
                                           corner_user->y() + deck_size * row + deck_size * 0.5), 4, 4);
            }
        }
    }


    for (int column = 0; column < 10; column++) {
        for (int row = 0; row < 10; row++) {

            if (field_robot->matrix[column][row] == -2) {


                painter.setPen(redPen);
                painter.drawLine(
                        QPoint(corner_robot->x() + deck_size * column + 2, corner_robot->y() + deck_size * row + 2),
                        QPoint(corner_robot->x() + deck_size * (column + 1) - 2,
                               corner_robot->y() + deck_size * (row + 1) - 2));
                painter.drawLine(
                        QPoint(corner_robot->x() + deck_size * (column + 1) - 2,
                               corner_robot->y() + deck_size * row + 2),
                        QPoint(corner_robot->x() + deck_size * (column) + 2,
                               corner_robot->y() + deck_size * (row + 1) - 2));
            } else if (field_robot->matrix[column][row] == -1) {

                painter.setPen(darkBluePen);
                painter.setBrush(darkBlueBrush);
                painter.drawEllipse(QPoint(corner_robot->x() + deck_size * column + deck_size * 0.5,
                                           corner_robot->y() + deck_size * row + deck_size * 0.5), 4, 4);
            }
        }
    }
}

void MainGameWindow::PaintPosRobot() {
    if(gameOver) return;
    QPainter painter(this);
    QPen darkBluePen(QColorConstants::Svg::darkblue, 3);
    painter.setPen(darkBluePen);
    painter.drawRect(suggestedX * deck_size + corner_user->x(), suggestedY * deck_size + corner_user->y(), deck_size,
                     deck_size);
}

void MainGameWindow::PaintPosUser() {
    if(gameOver) return;
    QPainter painter(this);
    QPen darkBluePen(QColorConstants::Svg::darkblue, 3);
    painter.setPen(darkBluePen);

    if (suggestedCoord.getX() != -1)
        painter.drawRect(QRect(suggestedCoord.getX() * deck_size + corner_robot->x(),
                               suggestedCoord.getY() * deck_size + corner_robot->y(), deck_size, deck_size));
}

void MainGameWindow::TimerAlarm() {
    control = false;
    timer->stop();
}

bool MainGameWindow::EventFilter(QEvent *event) {
    if (control && event->type() == QEvent::MouseButtonPress) {
        return true;
    }
    return false;
}

void MainGameWindow::PaintTriangle() {
    QPainter painter(this);
    QPolygon triangle;
    QPen darkBluePen(QColorConstants::Svg::orange, 3);
    QBrush blueBrush(QColorConstants::Svg::beige);
    painter.setPen(darkBluePen);
    painter.setBrush(blueBrush);
    painter.setRenderHint(QPainter::Antialiasing, true);
    if (isUsersTurn) {
        triangle << field_rect_user->topRight() + QPoint(85, deck_size * 3.5);
        triangle << field_rect_user->bottomRight() + QPoint(85, -deck_size * 3.5);
        triangle << field_rect_user->topRight() + QPoint(140, deck_size * 5);
    } else {
        triangle << field_rect_user->topRight() + QPoint(125, deck_size * 3.5);
        triangle << field_rect_user->bottomRight() + QPoint(125, -deck_size * 3.5);
        triangle << field_rect_user->topRight() + QPoint(70, deck_size * 5);
    }
    painter.drawPolygon(triangle);
}

void MainGameWindow::PaintShotRobotsShips() {
    QPainter painter(this);
    QPen bluePen(QColorConstants::Svg::blue, 2);
    QBrush blueBrush(QColorConstants::Svg::lightskyblue);
    QPen redPen(QColorConstants::Svg::red, 3);
    painter.setBrush(blueBrush);

    bool horizontal;

    for (int i = 0; i < 10; i++) {
        if (mas_ships_robot[i]->GetState() == Ship::SHOT_SHIP) {
            Coordinates topLeft = mas_ships_robot[i]->mas_decks[0]->coord;
            QSize rect_size;
            QRect rect;
            rect.setTopLeft(
                    QPoint(topLeft.getX() * deck_size, topLeft.getY() * deck_size) + field_rect_robot->topLeft());
            if (mas_ships_robot[i]->size == 1 ||
                mas_ships_robot[i]->mas_decks[1]->coord.getX() > mas_ships_robot[i]->mas_decks[0]->coord.getX())
                horizontal = true;
            else horizontal = false;

            if (horizontal) {
                rect_size.setWidth(mas_ships_robot[i]->size * deck_size);
                rect_size.setHeight(deck_size);
                rect.setSize(rect_size);
                painter.setPen(redPen);
                painter.drawRect(rect);
                painter.setPen(bluePen);
                for (int j = 1; j < rect.width() / deck_size; j++) {
                    painter.drawLine(rect.topLeft() + QPoint(deck_size * j, 0),
                                     rect.bottomLeft() + QPoint(deck_size * j, 0));
                }
            } else {
                rect_size.setWidth(deck_size);
                rect_size.setHeight(mas_ships_robot[i]->size * deck_size);
                rect.setSize(rect_size);
                painter.setPen(redPen);
                painter.drawRect(rect);
                painter.setPen(bluePen);
                for (int j = 1; j < rect.height() / deck_size; j++) {
                    painter.drawLine(rect.topLeft() + QPoint(0, deck_size * j),
                                     rect.topRight() + QPoint(0, deck_size * j));
                }
            }
        }
    }
}

void MainGameWindow::PaintShotUsersShips() {
    QPainter painter(this);
    QPen redPen(QColorConstants::Svg::red, 3);
    painter.setPen(redPen);

    bool horizontal;

    for (int i = 0; i < 10; i++) {
        if (mas_ships_user[i]->GetState() == Ship::SHOT_SHIP){
            Coordinates topLeft = mas_ships_user[i]->mas_decks[0]->coord;
            QSize rect_size;
            QRect rect;
            rect.setTopLeft(
                    QPoint(topLeft.getX() * deck_size, topLeft.getY() * deck_size) + field_rect_user->topLeft());
            if (mas_ships_user[i]->size == 1 ||
                mas_ships_user[i]->mas_decks[1]->coord.getX() > mas_ships_user[i]->mas_decks[0]->coord.getX())
                horizontal = true;
            else horizontal = false;

            if (horizontal) {
                rect_size.setWidth(mas_ships_user[i]->size * deck_size);
                rect_size.setHeight(deck_size);
                rect.setSize(rect_size);
                painter.drawRect(rect);
            } else {
                rect_size.setWidth(deck_size);
                rect_size.setHeight(mas_ships_user[i]->size * deck_size);
                rect.setSize(rect_size);
                painter.drawRect(rect);
            }
        }
    }
}

bool MainGameWindow::isGameOver(Ship **mas_ships) {
    for (int i = 0; i < 10; i++) {
        if(mas_ships[i]->GetState()==Ship::SAFE_SHIP) return false;
    }
    gameOver = true;
    return true;
}

void MainGameWindow::onNewGame() {
    this->close();
    emit openPosWindow();
}
