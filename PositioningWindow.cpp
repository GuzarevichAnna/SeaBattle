
#include "PositioningWindow.h"

PositioningWindow::PositioningWindow() {
    field_rect = new QRect(QPoint(70, 70), QSize(length, length));
    finishButton = new QPushButton("Finish positioning");
    connect(finishButton, SIGNAL(clicked(bool)), SLOT(onFinish()));

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(finishButton, 22);
    mainLayout->setContentsMargins(500, 670, 500, 20);
    setLayout(mainLayout);

    four_decker_rect = new QRect(QPoint(field_rect->right() + length * 0.3, field_rect->top()),
                                 QSize(deck_size * 4, deck_size));
    three_decker_rect_1 = new QRect(four_decker_rect->bottomLeft() + QPoint(0, deck_size),
                                    QSize(deck_size * 3, deck_size));
    three_decker_rect_2 = new QRect(three_decker_rect_1->topRight() + QPoint(deck_size * 1.3, 0),
                                    QSize(deck_size * 3, deck_size));
    two_decker_rect_1 = new QRect(three_decker_rect_1->bottomLeft() + QPoint(0, deck_size),
                                  QSize(deck_size * 2, deck_size));
    two_decker_rect_2 = new QRect(two_decker_rect_1->topRight() + QPoint(deck_size * 1.3, 0),
                                  QSize(deck_size * 2, deck_size));
    two_decker_rect_3 = new QRect(two_decker_rect_2->topRight() + QPoint(deck_size * 1.3, 0),
                                  QSize(deck_size * 2, deck_size));
    one_decker_rect_1 = new QRect(two_decker_rect_1->bottomLeft() + QPoint(2, deck_size),
                                  QSize(deck_size, deck_size));
    one_decker_rect_2 = new QRect(one_decker_rect_1->topRight() + QPoint(deck_size * 1.3, 0),
                                  QSize(deck_size, deck_size));
    one_decker_rect_3 = new QRect(one_decker_rect_2->topRight() + QPoint(deck_size * 1.3, 0),
                                  QSize(deck_size, deck_size));
    one_decker_rect_4 = new QRect(one_decker_rect_3->topRight() + QPoint(deck_size * 1.3, 0),
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

    if (isInField) {
        QRect suggested_pos;
        switch (which_ship) {
            case FOUR_DECKER_RECT:
                suggested_pos = CalculateSuggestedPos(four_decker_rect);
                break;
            case THREE_DECKER_RECT_1:
                suggested_pos = CalculateSuggestedPos(three_decker_rect_1);
                break;
            case THREE_DECKER_RECT_2:
                suggested_pos = CalculateSuggestedPos(three_decker_rect_2);
                break;
            case TWO_DECKER_RECT_1:
                suggested_pos = CalculateSuggestedPos(two_decker_rect_1);
                break;
            case TWO_DECKER_RECT_2:
                suggested_pos = CalculateSuggestedPos(two_decker_rect_2);
                break;
            case TWO_DECKER_RECT_3:
                suggested_pos = CalculateSuggestedPos(two_decker_rect_3);
                break;
            case ONE_DECKER_RECT_1:
                suggested_pos = CalculateSuggestedPos(one_decker_rect_1);
                break;
            case ONE_DECKER_RECT_2:
                suggested_pos = CalculateSuggestedPos(one_decker_rect_2);
                break;
            case ONE_DECKER_RECT_3:
                suggested_pos = CalculateSuggestedPos(one_decker_rect_3);
                break;
            case ONE_DECKER_RECT_4:
                suggested_pos = CalculateSuggestedPos(one_decker_rect_4);
                break;
            default:
                break;
        }

        painter.setBrush(greenBrush);
        painter.setPen(darkGreenPen);
        painter.drawRect(suggested_pos);
        for (int i = 1; i < suggested_pos.width()/deck_size; i++) {
            painter.drawLine(suggested_pos.topLeft() + QPoint(deck_size * i, 0),
                             suggested_pos.bottomLeft() + QPoint(deck_size * i, 0));
        }
    }

    painter.setBrush(blueBrush);
    painter.setPen(bluePen);

    painter.drawRect(*four_decker_rect);
    painter.drawRect(*three_decker_rect_1);
    painter.drawRect(*three_decker_rect_2);
    painter.drawRect(*two_decker_rect_1);
    painter.drawRect(*two_decker_rect_2);
    painter.drawRect(*two_decker_rect_3);
    painter.drawRect(*one_decker_rect_1);
    painter.drawRect(*one_decker_rect_2);
    painter.drawRect(*one_decker_rect_3);
    painter.drawRect(*one_decker_rect_4);

    for (int i = 1; i <= 3; i++) {
        painter.drawLine(four_decker_rect->topLeft() + QPoint(deck_size * i, 0),
                         four_decker_rect->bottomLeft() + QPoint(deck_size * i, 0));
    }

    for (int i = 1; i <= 2; i++) {
        painter.drawLine(three_decker_rect_1->topLeft() + QPoint(deck_size * i, 0),
                         three_decker_rect_1->bottomLeft() + QPoint(deck_size * i, 0));
    }

    for (int i = 1; i <= 2; i++) {
        painter.drawLine(three_decker_rect_2->topLeft() + QPoint(deck_size * i, 0),
                         three_decker_rect_2->bottomLeft() + QPoint(deck_size * i, 0));
    }

    painter.drawLine(two_decker_rect_1->topLeft() + QPoint(deck_size, 0),
                     two_decker_rect_1->bottomLeft() + QPoint(deck_size, 0));
    painter.drawLine(two_decker_rect_2->topLeft() + QPoint(deck_size, 0),
                     two_decker_rect_2->bottomLeft() + QPoint(deck_size, 0));
    painter.drawLine(two_decker_rect_3->topLeft() + QPoint(deck_size, 0),
                     two_decker_rect_3->bottomLeft() + QPoint(deck_size, 0));

}

void PositioningWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (four_decker_rect->contains(event->pos())) {
            previous_point = event->pos();
            which_ship = FOUR_DECKER_RECT;
        } else if (three_decker_rect_1->contains(event->pos())) {
            previous_point = event->pos();
            which_ship = THREE_DECKER_RECT_1;
        } else if (three_decker_rect_2->contains(event->pos())) {
            previous_point = event->pos();
            which_ship = THREE_DECKER_RECT_2;
        } else if (two_decker_rect_1->contains(event->pos())) {
            previous_point = event->pos();
            which_ship = TWO_DECKER_RECT_1;
        } else if (two_decker_rect_2->contains(event->pos())) {
            previous_point = event->pos();
            which_ship = TWO_DECKER_RECT_2;
        } else if (two_decker_rect_3->contains(event->pos())) {
            previous_point = event->pos();
            which_ship = TWO_DECKER_RECT_3;
        } else if (one_decker_rect_1->contains(event->pos())) {
            previous_point = event->pos();
            which_ship = ONE_DECKER_RECT_1;
        } else if (one_decker_rect_2->contains(event->pos())) {
            previous_point = event->pos();
            which_ship = ONE_DECKER_RECT_2;
        } else if (one_decker_rect_3->contains(event->pos())) {
            previous_point = event->pos();
            which_ship = ONE_DECKER_RECT_3;
        } else if (one_decker_rect_4->contains(event->pos())) {
            previous_point = event->pos();
            which_ship = ONE_DECKER_RECT_4;
        } else {
            which_ship = NONE;
        }
    }
}

void PositioningWindow::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton)) {
        cur_point = event->pos();

        switch (which_ship) {
            case FOUR_DECKER_RECT:
                MoveRects(four_decker_rect);
                break;
            case THREE_DECKER_RECT_1:
                MoveRects(three_decker_rect_1);
                break;
            case THREE_DECKER_RECT_2:
                MoveRects(three_decker_rect_2);
                break;
            case TWO_DECKER_RECT_1:
                MoveRects(two_decker_rect_1);
                break;
            case TWO_DECKER_RECT_2:
                MoveRects(two_decker_rect_2);
                break;
            case TWO_DECKER_RECT_3:
                MoveRects(two_decker_rect_3);
                break;
            case ONE_DECKER_RECT_1:
                MoveRects(one_decker_rect_1);
                break;
            case ONE_DECKER_RECT_2:
                MoveRects(one_decker_rect_2);
                break;
            case ONE_DECKER_RECT_3:
                MoveRects(one_decker_rect_3);
                break;
            case ONE_DECKER_RECT_4:
                MoveRects(one_decker_rect_4);
                break;
            default:
                break;
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


