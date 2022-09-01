
#include "PositioningWindow.h"

PositioningWindow::PositioningWindow() {
    field_rect = new QRect(QPoint(100, 70), QSize(length, length));
    finishButton = new QPushButton("Finish positioning");
    autoButton = new QPushButton("Auto positioning");
    connect(finishButton, SIGNAL(clicked(bool)), SLOT(onFinish()));
    connect(autoButton, SIGNAL(clicked(bool)), SLOT(onAuto()));
    finishButton->setEnabled(false);

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            matrix[i][j] = 0;
        }
    }

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            matrix_robot[i][j] = 0;
        }
    }

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(autoButton);
    mainLayout->addWidget(finishButton);
    mainLayout->setContentsMargins(500, 670, 500, 20);
    setLayout(mainLayout);

    for (int i = 0; i < sizeof(mas_rect) / sizeof(QRect *); i++) {
        QSize rect_size;
        if (i == 0) rect_size = QSize(deck_size * 4, deck_size);
        else if (i < 3) rect_size = QSize(deck_size * 3, deck_size);
        else if (i < 6) rect_size = QSize(deck_size * 2, deck_size);
        else rect_size = QSize(deck_size, deck_size);
        mas_rect[i] = new QRect(GetStartPos(i), rect_size);
    }

    gameWindow = new MainGameWindow();
    connect(gameWindow, &MainGameWindow::openPosWindow, this, &PositioningWindow::newGame);
}

void PositioningWindow::paintEvent(QPaintEvent *event) {
    PaintEmptyField();
    PaintRects();
}

void PositioningWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        for (int i = 0; i < sizeof(mas_rect) / sizeof(QRect *); i++) {
            if (mas_rect[i]->contains(event->pos())) {
                if (field_rect->contains(event->pos())) {
                    EditMatrix(mas_rect[i], 0);
                }

                previous_point = event->pos();
                which_ship = i;
                isPositioningInProcess = true;
                break;
            } else if (i == sizeof(mas_rect) / sizeof(QRect *) - 1) {
                which_ship = -1;
            }
        }
    }
}

void PositioningWindow::mouseMoveEvent(QMouseEvent *event) {
    if (((event->buttons() & Qt::LeftButton))) {

        if (which_ship != -1) {
            if (!((mas_rect[which_ship]->top() <= 0 ||
                   mas_rect[which_ship]->bottom() >= this->height()) && (event->y() < 0 || event->y() >
                                                                                           this->height()))) {
                cur_point.setY(event->y());
            }

            if (!((mas_rect[which_ship]->left() <= 0 ||
                   mas_rect[which_ship]->right() >= this->width()) && (event->x() < 0 || event->x() >
                                                                                         this->width()))) {
                cur_point.setX(event->x());
            }
            MoveRects(mas_rect[which_ship]);
        }

        update();
        previous_point = cur_point;
    }
}

void PositioningWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (which_ship != -1) {
        if (isInField) {
            QRect suggested_pos = CalculateSuggestedPos(mas_rect[which_ship]);
            if (CheckPos(suggested_pos)) {
                mas_rect[which_ship]->moveTopLeft(suggested_pos.topLeft());
                EditMatrix(mas_rect[which_ship], 1);
                isPositioningInProcess = false;
                for (int i = 0; i < 10; i++) {
                    if (!field_rect->contains(*mas_rect[i])) break;
                    if (i == 9) finishButton->setEnabled(true);
                }
                update();
                return;
            }
        }
        if (mas_rect[which_ship]->width() < mas_rect[which_ship]->height()) {
            int w = mas_rect[which_ship]->width();
            mas_rect[which_ship]->setWidth(mas_rect[which_ship]->height());
            mas_rect[which_ship]->setHeight(w);
        }
        mas_rect[which_ship]->moveTopLeft(GetStartPos(which_ship));
        isPositioningInProcess = false;
        finishButton->setEnabled(false);
        update();
    }
}

void PositioningWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    for (int i = 0; i < sizeof(mas_rect) / sizeof(QRect *); i++) {
        if (mas_rect[i]->contains(event->pos())) {
            if (field_rect->contains(event->pos())) {
                EditMatrix(mas_rect[which_ship], 0);
                int w = mas_rect[which_ship]->width();
                mas_rect[which_ship]->setWidth(mas_rect[which_ship]->height());
                mas_rect[which_ship]->setHeight(w);
            }
        }
    }
}

void PositioningWindow::MoveRects(QRect *rect) {
    QPoint dist_point = cur_point - previous_point;
    rect->moveTo(rect->topLeft() + dist_point);

    if (field_rect->contains(rect->center())) {
        isInField = true;
    } else {
        isInField = false;
    }
}

void PositioningWindow::onFinish() {
    Ship *mas_ships[10];
    for (int i = 0; i < sizeof(mas_rect) / sizeof(QRect *); i++) {

        int size;
        int j;
        bool isShipHorizontal;

        if (mas_rect[i]->width() > mas_rect[i]->height()) {
            size = mas_rect[i]->width() / deck_size;
            isShipHorizontal = true;
        } else {
            size = mas_rect[i]->height() / deck_size;
            isShipHorizontal = false;
        }
        Deck **mas_decks = new Deck *[size];

        if (mas_rect[i]->width() > mas_rect[i]->height()) {
            for (j = 0; j < size; j++) {
                Deck *deck = new Deck(
                        Coordinates::TranslateCoordinates(mas_rect[i]->topLeft() + QPoint(deck_size * j, 0),
                                                          field_rect->topLeft(), deck_size));
                mas_decks[j] = deck;
            }
        } else {
            for (j = 0; j < size; j++) {
                Deck *deck = new Deck(
                        Coordinates::TranslateCoordinates(mas_rect[i]->topLeft() + QPoint(0, deck_size * j),
                                                          field_rect->topLeft(), deck_size));
                mas_decks[j] = deck;
            }
        }

        Ship *ship = new Ship(mas_decks, size, isShipHorizontal);
        mas_ships[i] = ship;
    }

    Ship *mas_ships_robot[10];
    GenerateShipsForRobot(mas_ships_robot);

    gameWindow->SetUpWindow(mas_ships, mas_ships_robot);
    gameWindow->resize(1300, 700);
    gameWindow->show();

    this->close();
}

void PositioningWindow::onAuto() {

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            matrix[i][j] = 0;
        }
    }

    srand(time(NULL));

    for (int i = 0; i < sizeof(mas_rect) / sizeof(QRect *); i++) {
        QRect suggested_pos;

        int length;
        if (i == 0) length = 4;
        else if (i < 3) length = 3;
        else if (i < 6) length = 2;
        else length = 1;

        while (!CheckPos(suggested_pos)) {
            bool isHorizontal = rand() % 2;
            int x, y;
            if (isHorizontal) {
                x = rand() % (10 - length + 1);
                y = rand() % 10;
            } else {
                x = rand() % 10;
                y = rand() % (10 - length + 1);
            }

            QSize rect_size;
            if (isHorizontal) rect_size = QSize(length * deck_size, deck_size);
            else rect_size = QSize(deck_size, length * deck_size);

            suggested_pos = QRect(QPoint(x * deck_size, y * deck_size) + field_rect->topLeft(), rect_size);
        }

        *mas_rect[i] = suggested_pos;
        EditMatrix(mas_rect[i], 1);
    }

    finishButton->setEnabled(true);
    update();
}

void PositioningWindow::newGame() {
    for (int i = 0; i < 10; i++) {
        QPoint topLeft = GetStartPos(i);
        mas_rect[i]->moveTopLeft(topLeft);
        if (mas_rect[i]->width() < mas_rect[i]->height()) {
            int w = mas_rect[i]->width();
            mas_rect[i]->setWidth(mas_rect[i]->height());
            mas_rect[i]->setHeight(w);
        }
    }

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            matrix[i][j] = 0;
        }
    }

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            matrix_robot[i][j] = 0;
        }
    }

    finishButton->setEnabled(false);
    this->show();
}

void PositioningWindow::PaintEmptyField() {

    QPainter painter(this);
    QPen blackPen(QColorConstants::Svg::black);
    painter.setPen(blackPen);

    painter.drawRect(*field_rect);

    for (int i = 1; i < 10; i++) {
        painter.drawLine(field_rect->topLeft() + QPoint(deck_size * i, 0),
                         field_rect->topLeft() + QPoint(deck_size * i, length));
        painter.drawLine(field_rect->topLeft() + QPoint(0, deck_size * i),
                         field_rect->topLeft() + QPoint(length, deck_size * i));
    }

    painter.setFont(QFont("Calibri", 15, QFont::Normal));
    for (int i = 0; i < 9; i++) {
        painter.drawText(field_rect->topLeft() + QPoint(-deck_size * 0.7, deck_size * (i + 0.8)),
                         QString::number(i + 1));
    }
    painter.drawText(field_rect->topLeft() + QPoint(-deck_size, deck_size * (9.8)), QString::number(10));

    for (char i = 65; i < 75; i++) {
        painter.drawText(field_rect->topLeft() + QPoint(deck_size * (i - 65 + 0.3), -deck_size * 0.2),
                         QString(QChar(i)));
    }
}

void PositioningWindow::PaintRects() {

    QPainter painter(this);
    QPen bluePen(QColorConstants::Svg::blue, 2);
    QBrush blueBrush(QColorConstants::Svg::lightskyblue);
    QBrush greenBrush(Qt::green);
    QPen darkGreenPen(Qt::darkGreen);
    QBrush redBrush(QColorConstants::Svg::pink);
    QPen redPen(QColorConstants::Svg::red, 2);

    if (isInField && isPositioningInProcess) {
        QRect suggested_pos = CalculateSuggestedPos(mas_rect[which_ship]);

        if (CheckPos(suggested_pos)) {
            painter.setBrush(greenBrush);
            painter.setPen(darkGreenPen);
        } else {
            painter.setBrush(redBrush);
            painter.setPen(redPen);
        }

        painter.drawRect(suggested_pos);

        if (suggested_pos.width() > suggested_pos.height()) {
            for (int i = 1; i < suggested_pos.width() / deck_size; i++) {
                painter.drawLine(suggested_pos.topLeft() + QPoint(deck_size * i, 0),
                                 suggested_pos.bottomLeft() + QPoint(deck_size * i, 0));
            }
        } else {
            for (int i = 1; i < suggested_pos.height() / deck_size; i++) {
                painter.drawLine(suggested_pos.topLeft() + QPoint(0, deck_size * i),
                                 suggested_pos.topRight() + QPoint(0, deck_size * i));
            }
        }
    }

    painter.setBrush(blueBrush);
    painter.setPen(bluePen);

    for (int i = 0; i < sizeof(mas_rect) / sizeof(QRect *); i++) {
        painter.drawRect(*mas_rect[i]);
    }

    for (int i = 0; i < sizeof(mas_rect) / sizeof(QRect *); i++) {
        if (mas_rect[i]->width() > mas_rect[i]->height()) {
            for (int j = 1; j <= mas_rect[i]->width() / deck_size - 1; j++) {
                painter.drawLine(mas_rect[i]->topLeft() + QPoint(deck_size * j, 0),
                                 mas_rect[i]->bottomLeft() + QPoint(deck_size * j, 0));
            }
        } else {
            for (int j = 1; j <= mas_rect[i]->height() / deck_size - 1; j++) {
                painter.drawLine(mas_rect[i]->topLeft() + QPoint(0, deck_size * j),
                                 mas_rect[i]->topRight() + QPoint(0, deck_size * j));
            }
        }
    }
}

QPoint PositioningWindow::GetStartPos(int i) {
    if (i >= 0 && i < 10) {
        switch (i) {
            case 0:
                return QPoint(field_rect->right() + length * 0.3, field_rect->top());
            case 1:
                return QPoint(field_rect->right() + length * 0.3, field_rect->top() + deck_size * 2);
            case 2:
                return QPoint(field_rect->right() + length * 0.3 + deck_size * 4.3, field_rect->top() + deck_size * 2);
            case 3:
                return QPoint(field_rect->right() + length * 0.3, field_rect->top() + deck_size * 4);
            case 4:
                return QPoint(field_rect->right() + length * 0.3 + deck_size * 3.3, field_rect->top() + deck_size * 4);
            case 5:
                return QPoint(field_rect->right() + length * 0.3 + deck_size * 6.6, field_rect->top() + deck_size * 4);
            case 6:
                return QPoint(field_rect->right() + length * 0.3 + 2, field_rect->top() + deck_size * 6);
            case 7:
                return QPoint(field_rect->right() + length * 0.3 + deck_size * 2.3 + 2,
                              field_rect->top() + deck_size * 6);
            case 8:
                return QPoint(field_rect->right() + length * 0.3 + deck_size * 4.6 + 2,
                              field_rect->top() + deck_size * 6);
            case 9:
                return QPoint(field_rect->right() + length * 0.3 + deck_size * 6.9 + 2,
                              field_rect->top() + deck_size * 6);
        }
    }
}

QRect PositioningWindow::CalculateSuggestedPos(QRect *rect) {
    int distX = (rect->x() - field_rect->x()) / deck_size;
    int distY = (rect->y() + deck_size * 0.99 - field_rect->y()) / deck_size;

    QRect suggested_pos(QPoint(field_rect->x() + distX * deck_size,
                               field_rect->y() + distY * deck_size), rect->size());

    while (suggested_pos.right() > field_rect->right()) {
        suggested_pos.translate(-deck_size, 0);
    }
    while (suggested_pos.left() < field_rect->left()) {
        suggested_pos.translate(deck_size, 0);
    }
    while (suggested_pos.bottom() > field_rect->bottom()) {
        suggested_pos.translate(0, -deck_size);
    }
    while (suggested_pos.top() < field_rect->top()) {
        suggested_pos.translate(0, deck_size);
    }

    return suggested_pos;
}

bool PositioningWindow::CheckPos(const QRect &rect) {
    if (rect.isNull()) return false;
    if (rect.width() > rect.height()) {
        for (int i = 0; i < rect.width() / deck_size; i++) {
            Coordinates coord = Coordinates::TranslateCoordinates(rect.topLeft() + QPoint(deck_size * i, 0),
                                                                  field_rect->topLeft(), deck_size);
            for (int j = -1; j <= 1; j++) {
                for (int k = -1; k <= 1; k++) {
                    if (coord.getX() + j >= 0 && coord.getX() + j < 10 && coord.getY() + k >= 0 &&
                        coord.getY() + k < 10) {
                        if (matrix[coord.getX() + j][coord.getY() + k] == 1) return false;
                    }
                }
            }
        }
    } else {
        for (int i = 0; i < rect.height() / deck_size; i++) {
            Coordinates coord = Coordinates::TranslateCoordinates(rect.topLeft() + QPoint(0, deck_size * i),
                                                                  field_rect->topLeft(), deck_size);
            for (int j = -1; j <= 1; j++) {
                for (int k = -1; k <= 1; k++) {
                    if (coord.getX() + j >= 0 && coord.getX() + j < 10 && coord.getY() + k >= 0 &&
                        coord.getY() + k < 10) {
                        if (matrix[coord.getX() + j][coord.getY() + k] == 1) return false;
                    }
                }
            }
        }
    }

    return true;
}

void PositioningWindow::EditMatrix(QRect *rect, int value) {
    if (rect->width() > rect->height()) {
        for (int j = 0; j < rect->width() / deck_size; j++) {
            Coordinates coord = Coordinates::TranslateCoordinates(
                    rect->topLeft() + QPoint(deck_size * j, 0), field_rect->topLeft(),
                    deck_size);
            matrix[coord.getX()][coord.getY()] = value;
        }
    } else {
        for (int j = 0; j < rect->height() / deck_size; j++) {
            Coordinates coord = Coordinates::TranslateCoordinates(
                    rect->topLeft() + QPoint(0, deck_size * j), field_rect->topLeft(),
                    deck_size);
            matrix[coord.getX()][coord.getY()] = value;
        }
    }
}

void PositioningWindow::GenerateShipsForRobot(Ship **mas_ships) {

    srand(time(NULL));

    for (int i = 0; i < 10; i++) {
        int length;
        if (i == 0) length = 4;
        else if (i < 3) length = 3;
        else if (i < 6) length = 2;
        else length = 1;

        Coordinates suggested_loc[length];
        suggested_loc[0] = Coordinates(-1, -1);

        while (!CheckLocRobot(suggested_loc, length)) {
            bool isHorizontal = rand() % 2;
            int x, y;
            if (isHorizontal) {
                x = rand() % (10 - length + 1);
                y = rand() % 10;
            } else {
                x = rand() % 10;
                y = rand() % (10 - length + 1);
            }

            if (isHorizontal) {
                for (int j = 0; j < length; j++) {
                    Coordinates coord(x + j, y);
                    suggested_loc[j] = coord;
                }
            } else {
                for (int j = 0; j < length; j++) {
                    Coordinates coord(x, y + j);
                    suggested_loc[j] = coord;
                }
            }
        }

        Deck **mas_decks = new Deck *[length];

        for (int j = 0; j < length; j++) {
            Deck *deck = new Deck(suggested_loc[j]);
            mas_decks[j] = deck;
            matrix_robot[suggested_loc[j].getX()][suggested_loc[j].getY()] = 1;
        }


        Ship *ship = new Ship(mas_decks, length);
        mas_ships[i] = ship;
    }
}

bool PositioningWindow::CheckLocRobot(Coordinates *suggested_loc, int length) {
    if (suggested_loc[0].getX() == -1) return false;

    for (int i = 0; i < length; ++i) {
        Coordinates coord = suggested_loc[i];
        for (int j = -1; j <= 1; j++) {
            for (int k = -1; k <= 1; k++) {
                if (coord.getX() + j >= 0 && coord.getX() + j < 10 && coord.getY() + k >= 0 &&
                    coord.getY() + k < 10) {
                    if (matrix_robot[coord.getX() + j][coord.getY() + k] == 1) return false;
                }
            }
        }
    }

    return true;
}
