//
// Created by TianKai Ma on 2023/11/24.
//

#ifndef QT_EXAMPLE_PROJ_WIDGET_H
#define QT_EXAMPLE_PROJ_WIDGET_H

#include <QWidget>
#include <QPainter>

class Widget : public QWidget {
Q_OBJECT

    QVector<QPoint> points;
    int selectedPointIndex = -1;

    bool isHoldingMouse = false;

public:
    explicit Widget(QWidget *parent = nullptr);

    void init();

    void paintEditPanel();

    void paintCanvas();

    void paintEvent(QPaintEvent *event) override;

    void addRandomNewPoint();

    void addNewPoint(QPoint point);

    // track mouse events:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    // dump as string
    QString dumpAsString();

    // load from string
    void loadFromString(QString str);

    // save to clipboard
    void saveToClipboard();

    // load from clipboard
    void loadFromClipboard();
};

#endif //QT_EXAMPLE_PROJ_WIDGET_H
