//
// Created by TianKai Ma on 2023/11/24.
//

#ifndef QT_EXAMPLE_PROJ_RASTER_H
#define QT_EXAMPLE_PROJ_RASTER_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "Path.h"

bool cmp_Raster(const IntPoint &a, const IntPoint &b);

IntPaths Raster(const IntPath &contour, const IntPaths &holes, double distance, double scale);

Paths Raster(const Path &contour, const Paths &holes, double distance);

#endif //QT_EXAMPLE_PROJ_RASTER_H
