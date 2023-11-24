#include "ZigZag.h"


IntPaths ZigZag(const IntPath& contour, const IntPaths& holes, double distance, double scale) {
    IntPaths ps;
    IntPaths contour_now; 

    contour_now.push_back(contour);
    for (const auto & hole : holes) {
        contour_now.push_back(hole);
    }

    
    int y_max = contour_now[0][0].y, y_min = contour_now[0][0].y;
    for (auto & i : contour_now) {
        for (auto & j : i) {
            y_max = std::max(y_max, j.y);
            y_min = std::min(y_min, j.y);
        }
    }
    int num = (int)ceil((y_max - y_min) / (scale * distance)) - 1;
    int Distance = (y_max - y_min) / (num + 1);
    int y0 = y_min + Distance / 2;


    
    struct InterPoint {
        IntPoint point;
        unsigned int id_path;
        unsigned int id_point;
        InterPoint(int x, int y, unsigned int Path, unsigned int point) :
                point(IntPoint({x, y})), id_path(Path), id_point(point) {}
        bool operator < (const InterPoint& p) const {
            return cmp_Raster(this->point, p.point);
        }
        static bool next_to(const InterPoint& a, const InterPoint& b, int size) {
            
            return (a.id_path == b.id_path) && ((a.id_point == (b.id_point + 1) % size) || (b.id_point == (a.id_point + 1) % size));
        }
    };
    std::vector<InterPoint> intersection; 

    int* size_path_iter = new int[contour_now.size()]();
    for (int i = 0; i < contour_now.size(); ++i) {
        unsigned int id_point = 0;
        for (int j = 0; j < contour_now[i].size(); ++j) {
            int I = ceil(1.0 * (contour_now[i][j].y - y0) / Distance);
            int II = ceil(1.0 * (contour_now[i][(j + 1) % contour_now[i].size()].y - y0) / Distance);
            for (int k = std::min(I, II); k < std::max(I, II); ++k) {
                double lambda = 1.0 * (y0 + k * Distance - contour_now[i][j].y) / (contour_now[i][(j + 1) % contour_now[i].size()].y - contour_now[i][j].y);
                intersection.emplace_back(int(contour_now[i][j].x * (1.0 - lambda) + contour_now[i][(j + 1) % contour_now[i].size()].x * lambda), y0 + k * Distance, i, id_point++);
            }
        }
        size_path_iter[i] = (int)id_point;
    }

    
    std::sort(intersection.data(), intersection.data() + intersection.size());
    std::vector<std::vector<InterPoint>> InterPoint_Paths;
    for (int i = 0; i < intersection.size(); i += 2) {
        bool flag = false;
        for (int j = (int)InterPoint_Paths.size() - 1; j >= 0; --j) {
            if (InterPoint::next_to(intersection[i], InterPoint_Paths[j][InterPoint_Paths[j].size() - 1],
                                    size_path_iter[intersection[i].id_path])) {
                InterPoint_Paths[j].push_back(intersection[i]);
                InterPoint_Paths[j].push_back(intersection[i + 1]);
                flag = true;
                break;
            }
            if (InterPoint::next_to(intersection[i + 1], InterPoint_Paths[j][InterPoint_Paths[j].size() - 1],
                                    size_path_iter[intersection[i + 1].id_path])) {
                InterPoint_Paths[j].push_back(intersection[i + 1]);
                InterPoint_Paths[j].push_back(intersection[i]);
                flag = true;
                break;
            }
        }
        if (!flag) {
            std::vector<InterPoint> Path_now;
            Path_now.push_back(intersection[i]);
            Path_now.push_back(intersection[i + 1]);
            InterPoint_Paths.push_back(Path_now);
        }
    }
    for (auto & InterPoint_Path : InterPoint_Paths) {
        IntPath p;
        for (auto & j : InterPoint_Path) {
            p.push_back(j.point);
        }
        ps.push_back(p);
    }

    return ps;
}



Paths ZigZag(const Path& contour, const Paths& holes, double distance) {
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

    IntPaths Solution = ZigZag(Contour, Holes, distance, scale);

    Paths solution = IntPaths_to_Paths(Solution, scale, delta_x, delta_y);
    return solution;
}