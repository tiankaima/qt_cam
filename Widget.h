//
// Created by TianKai Ma on 2023/11/24.
//

#ifndef QT_EXAMPLE_PROJ_WIDGET_H
#define QT_EXAMPLE_PROJ_WIDGET_H

#include "iostream"
#include "QApplication"
#include "QClipboard"
#include "QDateTime"
#include "QLabel"
#include "QMessageBox"
#include "QMouseEvent"
#include "QPushButton"
#include "QPainter"
#include "QPainterPath"
#include "QRandomGenerator"
#include "QTimer"
#include "QWidget"
#include "ToolPath-lib/Path.h"
#include "ToolPath-lib/Raster.h"
#include "ToolPath-lib/ZigZag.h"
#include "ToolPath-lib/Contour_Parallel.h"

enum class METHOD {
    RASTER, ZIGZAG, CONTOUR_PARALLEL
};

class Widget : public QWidget {
Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

    void init();

    void paintSingle(const METHOD &m, QPainter &painter);

    void paintEvent(QPaintEvent *event) override;
};

#endif //QT_EXAMPLE_PROJ_WIDGET_H
