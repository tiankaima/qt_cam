//
// Created by TianKai Ma on 2023/11/24.
//

#ifndef QT_EXAMPLE_PROJ_ZIGZAG_H
#define QT_EXAMPLE_PROJ_ZIGZAG_H

#include "iostream"
#include "vector"
#include "algorithm"
#include "Path.h"
#include "Raster.h"

IntPaths ZigZag(const IntPath& contour, const IntPaths& holes, double distance, double scale);

Paths ZigZag(const Path& contour, const Paths& holes, double distance);

#endif //QT_EXAMPLE_PROJ_ZIGZAG_H
