
#include "PositioningWindow.h"

PositioningWindow::PositioningWindow() {
    corner = new QPoint(70, 70);
    finishButton = new QPushButton("Finish positioning");
    connect(finishButton, SIGNAL(clicked(bool)), SLOT(onFinish()));

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(finishButton, 22);
    mainLayout->setContentsMargins(500, 670, 500, 20);
    setLayout(mainLayout);

//    four_decker_rect = new QRect(*corner_1 + QPoint(0, length+gap), QSize(deck_size*4, deck_size));
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

//    QRect three_decker_1_rect(*corner_1 + QPoint(0, length+deck_size*1.5), QSize(deck_size*3, deck_size));
//    QRect three_decker_2_rect(*corner_1 + QPoint(0, length+gap), QSize(deck_size*4, deck_size));
//    QRect two_decker_rect(*corner_1 + QPoint(0, length+gap), QSize(deck_size*4, deck_size));
//
//    painter.drawRect(*four_decker_rect);

}

void PositioningWindow::PaintEmptyField() {

    QPainter painter(this);
    QPen blackPen(QColorConstants::Svg::black);
    painter.setPen(blackPen);

    QSize field_size(length, length);
    QRect field(*corner, field_size);
    painter.drawRect(field);

    for (int i = 1; i < 10; i++) {
        painter.drawLine(*corner + QPoint(deck_size * i, 0), *corner + QPoint(deck_size * i, length));
        painter.drawLine(*corner + QPoint(0, deck_size * i), *corner + QPoint(length, deck_size * i));
    }
}

//void PositioningWindow::mousePressEvent(QMouseEvent *event) {
//    if (event->buttons() == Qt::LeftButton
//        && four_decker_rect->contains(event->pos())) {
//
//         QDrag *drag = new QDrag(this);
//         //drag->setPixmap()
//
//        Qt::DropAction dropAction = drag->exec();
//    }
//}
