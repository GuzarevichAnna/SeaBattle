
#include <QApplication>
#include "PositioningWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    PositioningWindow pw;
    pw.resize(1300, 700);
    pw.show();
    return a.exec();
}
