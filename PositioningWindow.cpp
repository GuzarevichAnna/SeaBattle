
#include "PositioningWindow.h"

PositioningWindow::PositioningWindow() {
    field_rect = new QRect(QPoint(70, 70), QSize(length, length));
    finishButton = new QPushButton("Finish positioning");
    connect(finishButton, SIGNAL(clicked(bool)), SLOT(onFinish()));

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(finishButton, 22);
    mainLayout->setContentsMargins(500, 670, 500, 20);
    setLayout(mainLayout);

    mas_rect[0] = new QRect(QPoint(field_rect->right() + length * 0.3, field_rect->top()),
                            QSize(deck_size * 4, deck_size));
    mas_rect[1] = new QRect(mas_rect[0]->bottomLeft() + QPoint(0, deck_size),
                            QSize(deck_size * 3, deck_size));
    mas_rect[2] = new QRect(mas_rect[1]->topRight() + QPoint(deck_size * 1.3, 0),
                            QSize(deck_size * 3, deck_size));
    mas_rect[3] = new QRect(mas_rect[1]->bottomLeft() + QPoint(0, deck_size),
                            QSize(deck_size * 2, deck_size));
    mas_rect[4] = new QRect(mas_rect[3]->topRight() + QPoint(deck_size * 1.3, 0),
                            QSize(deck_size * 2, deck_size));
    mas_rect[5] = new QRect(mas_rect[4]->topRight() + QPoint(deck_size * 1.3, 0),
                            QSize(deck_size * 2, deck_size));
    mas_rect[6] = new QRect(mas_rect[3]->bottomLeft() + QPoint(2, deck_size),
                            QSize(deck_size, deck_size));
    mas_rect[7] = new QRect(mas_rect[6]->topRight() + QPoint(deck_size * 1.3, 0),
                            QSize(deck_size, deck_size));
    mas_rect[8] = new QRect(mas_rect[7]->topRight() + QPoint(deck_size * 1.3, 0),
                            QSize(deck_size, deck_size));
    mas_rect[9] = new QRect(mas_rect[8]->topRight() + QPoint(deck_size * 1.3, 0),
                            QSize(deck_size, deck_size));
}

void PositioningWindow::onFinish() {
    std::vector<int> ship_1_cell_1;
    ship_1_cell_1.push_back(0);
    ship_1_cell_1.push_back(0);

    std::vector<int> ship_1_cell_2;
    ship_1_cell_2.push_back(1);
    ship_1_cell_2.push_back(0);

    std::vector<std::vector<int>> loc_1;
    loc_1.push_back(ship_1_cell_1);
    loc_1.push_back(ship_1_cell_2);

    Ship ship_1(loc_1);

    std::vector<Ship *> ships;
    ships.push_back(&ship_1);

    gameWindow = new MainGameWindow(ships, ships);
    gameWindow->resize(1100, 700);
    gameWindow->show();
    this->close();
}

void PositioningWindow::paintEvent(QPaintEvent *event) {
    PaintEmptyField();
    PaintRects();
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
}

void PositioningWindow::PaintRects() {

    QPainter painter(this);
    QPen bluePen(QColorConstants::Svg::blue, 2);
    QBrush blueBrush(QColorConstants::Svg::lightskyblue);
    QPen greenPen(Qt::green, 1);
    QBrush greenBrush(Qt::green);
    QPen darkGreenPen(Qt::darkGreen);

    if (isInField && isPositioningInProcess) {
        QRect suggested_pos= CalculateSuggestedPos(mas_rect[which_ship]);

        painter.setBrush(greenBrush);
        painter.setPen(darkGreenPen);
        painter.drawRect(suggested_pos);
        for (int i = 1; i < suggested_pos.width() / deck_size; i++) {
            painter.drawLine(suggested_pos.topLeft() + QPoint(deck_size * i, 0),
                             suggested_pos.bottomLeft() + QPoint(deck_size * i, 0));
        }
    }

    painter.setBrush(blueBrush);
    painter.setPen(bluePen);

    for (int i = 0; i < sizeof(mas_rect) / sizeof(QRect *); i++) {
        painter.drawRect(*mas_rect[i]);
    }

    for (int i = 1; i <= 3; i++) {
        painter.drawLine(mas_rect[0]->topLeft() + QPoint(deck_size * i, 0),
                         mas_rect[0]->bottomLeft() + QPoint(deck_size * i, 0));
    }

    for (int i = 1; i <= 2; i++) {
        painter.drawLine(mas_rect[1]->topLeft() + QPoint(deck_size * i, 0),
                         mas_rect[1]->bottomLeft() + QPoint(deck_size * i, 0));
    }

    for (int i = 1; i <= 2; i++) {
        painter.drawLine(mas_rect[2]->topLeft() + QPoint(deck_size * i, 0),
                         mas_rect[2]->bottomLeft() + QPoint(deck_size * i, 0));
    }

    painter.drawLine(mas_rect[3]->topLeft() + QPoint(deck_size, 0),
                     mas_rect[3]->bottomLeft() + QPoint(deck_size, 0));
    painter.drawLine(mas_rect[4]->topLeft() + QPoint(deck_size, 0),
                     mas_rect[4]->bottomLeft() + QPoint(deck_size, 0));
    painter.drawLine(mas_rect[5]->topLeft() + QPoint(deck_size, 0),
                     mas_rect[5]->bottomLeft() + QPoint(deck_size, 0));

}

void PositioningWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        for (int i = 0; i < sizeof(mas_rect) / sizeof(QRect *); i++) {
            if (mas_rect[i]->contains(event->pos())) {
                previous_point = event->pos();
                which_ship = i;
                isPositioningInProcess = true;
                break;
            }
            else if (i ==sizeof(mas_rect) / sizeof(QRect *) -1){
                which_ship = -1;
            }
        }
    }
}

void PositioningWindow::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton)) {
        cur_point = event->pos();

        if(which_ship!= -1) {
            MoveRects(mas_rect[which_ship]);
        }

        update();
        previous_point = cur_point;
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

QRect PositioningWindow::CalculateSuggestedPos(QRect *rect) {
    int distX = (rect->x() - field_rect->x()) / deck_size;
    int distY = ((rect->y() + deck_size - field_rect->y()) / deck_size);

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

void PositioningWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (isInField && which_ship!= -1) {
        QRect suggested_pos = CalculateSuggestedPos(mas_rect[which_ship]);
        mas_rect[which_ship]->moveTopLeft(suggested_pos.topLeft());
        isPositioningInProcess = false;
        update();
    }
}


