#include "Raster.h"

bool cmp_Raster(const IntPoint &a, const IntPoint &b) {
    if (a.y == b.y) {
        return a.x < b.x;
    }
    return a.y < b.y;
}

IntPaths Raster(const IntPath &contour, const IntPaths &holes, double distance, double scale) {
    IntPaths ps;
    IntPaths contour_now;

    contour_now.push_back(contour);
    for (const auto &hole: holes) {
        contour_now.push_back(hole);
    }

    int y_max = contour_now[0][0].x, y_min = contour_now[0][0].y;
    for (auto &i: contour_now) {
        for (auto &j: i) {
            y_max = std::max(y_max, j.y);
            y_min = std::min(y_min, j.y);
        }
    }
    int num = (int) ceil((y_max - y_min) / (scale * distance)) - 1;
    int _distance = (y_max - y_min) / (num + 1);
    int y0 = y_min + _distance / 2;

    std::vector<IntPoint> intersection;
    for (auto &i: contour_now) {
        for (int j = 0; j < i.size(); ++j) {
            int I = ceil(1.0 * (i[j].y - y0) / _distance);
            int II = ceil(1.0 * (i[(j + 1) % i.size()].y - y0) / _distance);
            for (int k = std::min(I, II); k < std::max(I, II); ++k) {
                double lambda = 1.0 * (y0 + k * _distance - i[j].y) / (i[(j + 1) % i.size()].y - i[j].y);
                intersection.push_back(IntPoint(
                        {int(i[j].x * (1.0 - lambda) + i[(j + 1) % i.size()].x * lambda), y0 + k * _distance}));
            }
        }
    }

    std::sort(intersection.data(), intersection.data() + intersection.size(), cmp_Raster);
    for (int i = 0; i < intersection.size(); i += 2) {
        IntPath p;
        p.push_back(intersection[i]);
        p.push_back(intersection[i + 1]);
        ps.push_back(p);
    }

    return ps;
}

Paths Raster(const Path &contour, const Paths &holes, double distance) {
    if (contour.x.empty()) return {};

    double x_max = contour.x[0];
    double x_min = contour.x[0];
    double y_max = contour.y[0];
    double y_min = contour.y[0];
    for (int i = 1; i < contour.x.size(); ++i) {
        x_max = std::max(x_max, contour.x[i]);
        x_min = std::min(x_min, contour.x[i]);
        y_max = std::max(y_max, contour.y[i]);
        y_min = std::min(y_min, contour.y[i]);
    }
    double delta_x = 0;
    double delta_y = 0;
    double scale = CLIPPER_BOUND / std::max(x_max - x_min, y_max - y_min);

    IntPath Contour = Path_to_IntPath(contour, scale, delta_x, delta_y);
    IntPaths Holes = Paths_to_IntPaths(holes, scale, delta_x, delta_y);

    IntPaths Solution = Raster(Contour, Holes, distance, scale);

    Paths solution = IntPaths_to_Paths(Solution, scale, delta_x, delta_y);
    return solution;
}

