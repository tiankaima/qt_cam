//
// Created by TianKai Ma on 2023/11/24.
//

#include "Widget.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define EDIT_PANEL_WIDTH 250
#define EDIT_PANEL_HEIGHT WINDOW_HEIGHT
#define CANVAS_WIDTH (WINDOW_WIDTH - EDIT_PANEL_WIDTH)
#define CANVAS_HEIGHT WINDOW_HEIGHT

Widget::Widget(QWidget *parent) : QWidget(parent) {
//    this->setStyleSheet("background-color: white;");
    this->setFixedSize(1200, 800);
    this->setWindowTitle("Bezier Curve - TianKai Ma 2023/11/24");

    this->init();
}

void Widget::init() {
    // add default points
    points.append(QPoint(100, 100));
    points.append(QPoint(300, 100));
    points.append(QPoint(200, 400));
    points.append(QPoint(400, 400));
}

void Widget::paintEditPanel() {
    // show a edit panel on the left, allowing user to edit points
    auto editPanel = new QWidget(this);
    editPanel->setGeometry(0, 0, EDIT_PANEL_WIDTH, EDIT_PANEL_HEIGHT);
    editPanel->setStyleSheet("background-color: lightgray;");

    // add button to add new point
    auto addPointButton = new QPushButton(editPanel);
    addPointButton->setGeometry(10, 0, 230, 50);
    addPointButton->setText("Add new point");
    connect(addPointButton, &QPushButton::clicked, this, &Widget::addRandomNewPoint);

    // add button to save to clipboard
    auto saveToClipboardButton = new QPushButton(editPanel);
    saveToClipboardButton->setGeometry(10, 50, 230, 50);
    saveToClipboardButton->setText("Save to clipboard");
    connect(saveToClipboardButton, &QPushButton::clicked, this, &Widget::saveToClipboard);

    // add button to load from clipboard
    auto loadFromClipboardButton = new QPushButton(editPanel);
    loadFromClipboardButton->setGeometry(10, 100, 230, 50);
    loadFromClipboardButton->setText("Load from clipboard");
    connect(loadFromClipboardButton, &QPushButton::clicked, this, &Widget::loadFromClipboard);

    editPanel->show();
}

void Widget::paintCanvas() {

    // do not overlap with edit panel
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.translate(EDIT_PANEL_WIDTH, 0);

    // print current point count on left top
    painter.drawText(50, 50, "Points: " + QString::number(points.size()));
    painter.drawText(50, 65, "Control points: " + QString::number((points.size() + 2) / 3));


    //    pointInfo->setFont(QFont("Monospace", 20));
    if (selectedPointIndex == -1) {
        painter.drawText(50, 750, "No point selected");
    } else {
        painter.drawText(50, 750, "x: " + QString::number(points[selectedPointIndex].x()) + "\ny: " +
                                  QString::number(points[selectedPointIndex].y()));
    }


    // draw filled circles at control points
    QPen controlPointPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(controlPointPen);
    for (int i = 0; i < points.size(); i += 3) {
        painter.drawPoint(points[i]);
    }

    // draw unfilled squares at end points
    QPen endPointPen(Qt::red, 1);
    painter.setPen(endPointPen);
    for (int i = 0; i < points.size(); i += 1) {
        if (i % 3 == 0) continue;
        auto path = QPainterPath();
        path.addRect(points[i].x() - 5, points[i].y() - 5, 10, 10);
        painter.drawPath(path);

    }

    // draw a unfilled circle around selected point
    if (selectedPointIndex != -1) {
        QPen selectedPointPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap);
        auto path = QPainterPath();
        path.addEllipse(points[selectedPointIndex].x() - 15, points[selectedPointIndex].y() - 15, 30, 30);
        painter.setPen(selectedPointPen);
        painter.drawPath(path);
    }

    // draw lines (to connect control points)
    QPen linePen = QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(linePen);
    for (int i = 0; i < points.size() - 1; i++) {
        painter.drawLine(points[i], points[i + 1]);
    }

    // draw bezier curve
    QPen bezierPen = QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(bezierPen);
    for (int i = 0; i < points.size() - 3; i += 3) {
        QPainterPath path;
        path.moveTo(points[i]);
        path.cubicTo(points[i + 1], points[i + 2], points[i + 3]);
        painter.drawPath(path);
    }

    painter.restore();
}

void Widget::addRandomNewPoint() {
    // add a new point at random position
    points.append(QPoint(
            QRandomGenerator::global()->bounded(0, CANVAS_WIDTH),
            QRandomGenerator::global()->bounded(0, CANVAS_HEIGHT)
    ));

    points.append(QPoint(
            QRandomGenerator::global()->bounded(0, CANVAS_WIDTH),
            QRandomGenerator::global()->bounded(0, CANVAS_HEIGHT)
    ));

    points.append(QPoint(
            QRandomGenerator::global()->bounded(0, CANVAS_WIDTH),
            QRandomGenerator::global()->bounded(0, CANVAS_HEIGHT)
    ));

    this->update();
}

void Widget::addNewPoint(QPoint point) {
    // calc (2 * lastPoint + point) / 3, (lastPoint + 2 * point) / 3

    auto lastPoint = points[points.size() - 1];
    auto point1 = QPoint((2 * lastPoint.x() + point.x()) / 3, (2 * lastPoint.y() + point.y()) / 3);
    auto point2 = QPoint((lastPoint.x() + 2 * point.x()) / 3, (lastPoint.y() + 2 * point.y()) / 3);

    points.append(point1);
    points.append(point2);
    points.append(point);
}

void Widget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    // Check if points.size is valid
    if (points.size() < 4 || points.size() % 3 != 1) {
        QMessageBox::warning(this, "Warning", "Invalid number of points, reset triggered");
        points.clear();
        this->init();
//        throw std::invalid_argument("Invalid number of points");
    }

    this->paintEditPanel();
    this->paintCanvas();

    this->update();
}

// mouse events
void Widget::mousePressEvent(QMouseEvent *event) {
    auto mouse_x = event->pos().x() - EDIT_PANEL_WIDTH;
    auto mouse_y = event->pos().y();



    // check if mouse is pressed on a point
    for (int i = 0; i < points.size(); i++) {
        if (abs(points[i].x() - mouse_x) < 10 && abs(points[i].y() - mouse_y) < 10) {
            if (event->button() == Qt::RightButton) {
                // if points < 4, do not allow delete, pop up a message box
                if (points.size() <= 4) {
                    QMessageBox::warning(this, "Warning", "Cannot delete point, at least 4 points are required");
                    return;
                }

                auto s = points.size();

                if (i == 0 || i == 1) {
                    points.remove(0);
                    points.remove(0);
                    points.remove(0);
                } else if (i == s - 1 || i == s - 2) {
                    points.remove(s - 1);
                    points.remove(s - 2);
                    points.remove(s - 3);
                } else if (i % 3 == 0) {
                    points.remove(i);
                    points.remove(i);
                    points.remove(i - 1);
                } else if (i % 3 == 1) {
                    points.remove(i);
                    points.remove(i - 1);
                    points.remove(i - 2);
                } else {
                    points.remove(i);
                    points.remove(i);
                    points.remove(i);
                }

                selectedPointIndex = -1;
                return;
            } else {
                selectedPointIndex = i;
                this->isHoldingMouse = true;
                return;
            }
        }
    }

    // if right click, add a new point
    if (event->button() == Qt::RightButton) {
        this->addNewPoint(QPoint(mouse_x, mouse_y));
        return;
    } else {
        selectedPointIndex = -1;
        this->isHoldingMouse = true;
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    auto mouse_x = event->pos().x() - EDIT_PANEL_WIDTH;
    auto mouse_y = event->pos().y();

    // move selected point
    if (selectedPointIndex != -1 && isHoldingMouse) {
        points[selectedPointIndex] = QPoint(mouse_x, mouse_y);
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event) {
    this->isHoldingMouse = false;
}

QString Widget::dumpAsString() {
    // save as:
    // P1.x P1.y
    // P2.x P2.y
    // ...

    QString str;
    for (auto point: points) {
        str += QString::number(point.x()) + " " + QString::number(point.y()) + "\n";
    }
    return str;
}

void Widget::loadFromString(QString str) {
    // load from string
    // P1.x P1.y
    // P2.x P2.y
    // ...

    auto backup = points;

    try {
        points.clear();
        auto lines = str.split("\n");
        for (auto line: lines) {
            auto nums = line.split(" ");
            if (nums.size() != 2) continue;
            points.append(QPoint(nums[0].toInt(), nums[1].toInt()));
        }
    } catch (int e) {
        points = backup;
        QMessageBox::warning(this, "Warning", "Invalid string");
    }
}

void Widget::saveToClipboard() {
    // save to clipboard
    QApplication::clipboard()->setText(this->dumpAsString());
}

void Widget::loadFromClipboard() {
    // load from clipboard
    this->loadFromString(QApplication::clipboard()->text());
}
