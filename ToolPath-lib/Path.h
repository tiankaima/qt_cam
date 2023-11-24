//
// Created by TianKai Ma on 2023/11/24.
//

#ifndef QT_EXAMPLE_PROJ_PATH_H
#define QT_EXAMPLE_PROJ_PATH_H

#define CLIPPER_BOUND 1e8

#include "vector"
#include "algorithm"
#include "cmath"
#include "stack"
#include "QPoint"

//typedef std::vector<QPoint> Path;
typedef struct {
    std::vector<double> x;
    std::vector<double> y;
} Path;
typedef std::vector<Path> Paths;

typedef struct {
    int x;
    int y;
} IntPoint;
typedef std::vector<IntPoint> IntPath;
typedef std::vector<IntPath> IntPaths;

class PathNode {
public:
    PathNode();

    PathNode(Path p);

    PathNode(const PathNode &pn);

    static std::vector<PathNode *> *DFS_root(PathNode *root); // DFS the depth tree and delete the tree
public:
    Path data; // the path of this node
    std::vector<PathNode *> children; // all children of this node
    PathNode *parent; // the parent of this node (NULL means it is the root)
};


int double_to_int(const double &d, double scale, double delta_pos);

double int_to_double(const int &c, double scale, double delta_pos);

Path IntPath_to_Path(const IntPath &P, double scale, double delta_x, double delta_y);

Paths IntPaths_to_Paths(const IntPaths &Ps, double scale, double delta_x, double delta_y);

IntPath Path_to_IntPath(const Path &P, double scale, double delta_x, double delta_y);

IntPaths Paths_to_IntPaths(const Paths &Ps, double scale, double delta_x, double delta_y);

#endif //QT_EXAMPLE_PROJ_PATH_H
