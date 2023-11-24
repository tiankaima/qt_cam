//
// Created by TianKai Ma on 2023/11/24.
//

#ifndef QT_EXAMPLE_PROJ_CONTOUR_PARALLEL_H
#define QT_EXAMPLE_PROJ_CONTOUR_PARALLEL_H

#include "iostream"
#include "vector"
#include "algorithm"
#include "Path.h"
#include "Raster.h"

Paths Contour_Parallel(const Path &contour, const Paths &holes, double distance);

#endif //QT_EXAMPLE_PROJ_CONTOUR_PARALLEL_H
