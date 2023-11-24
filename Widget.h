//
// Created by TianKai Ma on 2023/11/24.
//

#ifndef QT_EXAMPLE_PROJ_WIDGET_H
#define QT_EXAMPLE_PROJ_WIDGET_H

#include "iostream"
#include "QApplication"
#include "QClipboard"
#include "QLabel"
#include "QPushButton"
#include "QPainter"
#include "QPainterPath"
#include "QRandomGenerator"
#include "QMessageBox"
#include "QMouseEvent"
#include "QWidget"
#include "ToolPath-lib/Path.h"
#include "ToolPath-lib/Raster.h"
#include "ToolPath-lib/ZigZag.h"

class Widget : public QWidget {
Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

    void init();

    void paintEvent(QPaintEvent *event) override;
};

#endif //QT_EXAMPLE_PROJ_WIDGET_H
