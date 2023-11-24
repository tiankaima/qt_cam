#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QPainter>
#include <QRandomGenerator>
#include "Widget.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Widget widget;
    widget.show();

    return app.exec();
}
