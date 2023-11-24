//
// Created by TianKai Ma on 2023/11/24.
//

#include "Widget.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define PI 3.14159265358979323846
#define CONTOUR_SIZE 100

#define DELTA_X 300.0
#define DELTA_Y 300.0
#define SCALE 10.0

Widget::Widget(QWidget *parent) : QWidget(parent) {
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    this->setWindowTitle("CAM - TianKai Ma 2023/11/24");

    this->init();
}

void Widget::init() {

}

void Widget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    auto contour = Path();
    contour.x = std::vector<double>(CONTOUR_SIZE);
    contour.y = std::vector<double>(CONTOUR_SIZE);
    for (int i = 0; i < CONTOUR_SIZE; ++i) {
        auto theta = 2.0 * PI * i / CONTOUR_SIZE;
        auto r = 15.0 * (1.0 + 0.15 * cos(10.0 * theta));
        contour.x[i] = r * cos(theta);
        contour.y[i] = r * sin(theta);
    }
    auto holes = Paths();
//    auto solution = Raster(contour, holes, 1.0);
    auto solution = ZigZag(contour, holes, 1.0);

    // use QPainter to draw the solution
    QPainter painter(this);
    painter.save();

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));

    // the contour
    QPainterPath painter_path;
    painter_path.moveTo(contour.x[0] * SCALE + DELTA_X, contour.y[0] * SCALE + DELTA_Y);
    for (int i = 1; i < contour.x.size(); ++i) {
        painter_path.lineTo(contour.x[i] * SCALE + DELTA_X, contour.y[i] * SCALE + DELTA_Y);
    }
    painter_path.closeSubpath();
    painter.drawPath(painter_path);

    for (const auto & path : solution) {
        painter_path.moveTo(path.x[0] * SCALE + DELTA_X, path.y[0] * SCALE + DELTA_Y);
        for (int i = 1; i < path.x.size(); ++i) {
            painter_path.lineTo(path.x[i] * SCALE + DELTA_X, path.y[i] * SCALE + DELTA_Y);
        }

        painter_path.closeSubpath();
        painter.drawPath(painter_path);
    }

    painter.restore();
}