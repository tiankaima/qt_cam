//
// Created by TianKai Ma on 2023/11/24.
//

#include "Widget.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define PI 3.14159265358979323846
#define CONTOUR_SIZE 100

#define DELTA_X 250.0
#define DELTA_Y 300.0
#define SCALE 10.0

Widget::Widget(QWidget *parent) : QWidget(parent) {
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    this->setWindowTitle("CAM - TianKai Ma 2023/11/24");

    this->init();
}

void Widget::init() {
    // set timer, refresh every 100ms
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Widget::update));
    timer->start(100);
}

void Widget::paintSingle(const METHOD &m, QPainter &painter) {
    auto contour = Path();
    contour.x = std::vector<double>(CONTOUR_SIZE);
    contour.y = std::vector<double>(CONTOUR_SIZE);
    for (int i = 0; i < CONTOUR_SIZE; ++i) {
        auto theta = 2.0 * PI * i / CONTOUR_SIZE;
        auto r = 15.0 * (1.0 + 0.1 * cos(6.0 * theta));
        contour.x[i] = r * cos(theta);
        contour.y[i] = r * sin(theta);
    }
    auto holes = Paths();
    auto solution = Paths();

    switch (m) {
        case METHOD::RASTER:
            solution = Raster(contour, holes, 1.0);
            break;
        case METHOD::ZIGZAG:
            solution = ZigZag(contour, holes, 1.0);
            break;
        case METHOD::CONTOUR_PARALLEL:
            solution = Contour_Parallel(contour, holes, 1.0);
            break;
    }


    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::red, 1));
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));

    auto delta_x = 0;
    auto delta_y = 0;

    switch (m) {
        case METHOD::RASTER:
            delta_x = 250;
            delta_y = 300;
            break;
        case METHOD::ZIGZAG:
            delta_x = 750;
            delta_y = 300;
            break;
        case METHOD::CONTOUR_PARALLEL:
            delta_x = 500;
            delta_y = 500;
            break;
    }

    // the contour
    QPainterPath painter_path;
    painter_path.moveTo(contour.x[0] * SCALE + delta_x, contour.y[0] * SCALE + delta_y);
    for (int i = 1; i < contour.x.size(); ++i) {
        painter_path.lineTo(contour.x[i] * SCALE + delta_x, contour.y[i] * SCALE + delta_y);
    }
    painter_path.closeSubpath();
    painter.drawPath(painter_path);

    int counter = 0;
    for (const auto &path: solution) {
        painter_path.moveTo(path.x[0] * SCALE + delta_x, path.y[0] * SCALE + delta_y);
        for (int i = 1; i < path.x.size(); ++i) {
//            painter_path.lineTo(path.x[i] * SCALE + DELTA_X, path.y[i] * SCALE + DELTA_Y);
            counter++;
        }

//        painter_path.closeSubpath();
//        painter.drawPath(painter_path);
    }

    // based on current time, mod the counter to get a max value, then plot only to that value (in blue)
//    auto max = (int) QDateTime::currentSecsSinceEpoch() % counter;
    auto max = (int) QDateTime::currentMSecsSinceEpoch() % counter;

//    auto max = QRandomGenerator::global()->bounded(counter);
//    auto max = counter;
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    int new_conter = 0;
    for (const auto &path: solution) {
        painter_path.moveTo(path.x[0] * SCALE + delta_x, path.y[0] * SCALE + delta_y);
        for (int i = 1; i < path.x.size(); ++i) {
            painter_path.lineTo(path.x[i] * SCALE + delta_x, path.y[i] * SCALE + delta_y);
            new_conter++;
            if (new_conter > max) {
                break;
            }
        }

//        painter_path.closeSubpath();
        painter.drawPath(painter_path);
        if (new_conter > max) {
            break;
        }
    }
}

void Widget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    QPainter painter(this);
    painter.save();

    this->paintSingle(METHOD::RASTER, painter);
    this->paintSingle(METHOD::ZIGZAG, painter);
    this->paintSingle(METHOD::CONTOUR_PARALLEL, painter);


    // draw text for each method
    painter.setPen(QPen(Qt::white, 1, Qt::SolidLine));
    painter.drawText(230, 500, "Raster");
    painter.drawText(730, 500, "ZigZag");
    painter.drawText(450, 300, "Contour Parallel");
    painter.restore();
}