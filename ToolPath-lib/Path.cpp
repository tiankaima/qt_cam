//
// Created by TianKai Ma on 2023/11/24.
//

#include "Path.h"

#include <utility>

PathNode::PathNode() : data(Path()), parent(nullptr) {}

PathNode::PathNode(Path p) : data(std::move(p)), parent(nullptr) {}

PathNode::PathNode(const PathNode& pn) : data(pn.data), parent(pn.parent) {
    for (auto i : pn.children) {
        children.push_back(i);
    }
}

// DFS the depth tree and delete the tree
// root is the root of the depth tree.
// The output is all Paths on the tree in the order of DFS.
std::vector<PathNode*>* PathNode::DFS_root(PathNode* root) {
    auto* dfs = new std::vector<PathNode*>();
    std::stack<PathNode*> S;
    S.push(root);
    while (!S.empty()) {
        PathNode* Path_now = S.top();
        S.pop();
        dfs->push_back(Path_now);
        for (int i_child = (int)Path_now->children.size() - 1; i_child >= 0; --i_child) {
            S.push(Path_now->children[i_child]);
        }
    }
    return dfs;
}


int double_to_int(const double& d, double scale, double delta_pos) {
    return (int)((d - delta_pos) * scale);
}

double int_to_double(const int &c, double scale, double delta_pos) {
    return c / scale + delta_pos;
}

Path IntPath_to_Path(const IntPath& P, double scale, double delta_x, double delta_y) {
    Path p;
    p.x = std::vector<double>(P.size());
    p.y = std::vector<double>(P.size());
    for (int i = 0; i < P.size(); ++i) {
        p.x[i] = int_to_double(P[i].x, scale, delta_x);
        p.y[i] = int_to_double(P[i].y, scale, delta_y);
    }
    return p;
}

Paths IntPaths_to_Paths(const IntPaths& Ps, double scale, double delta_x, double delta_y) {
    Paths ps;
    for (const auto & P : Ps) {
        ps.push_back(IntPath_to_Path(P, scale, delta_x, delta_y));
    }
    return ps;
}

IntPath Path_to_IntPath(const Path& P, double scale, double delta_x, double delta_y) {
    IntPath p = IntPath(P.x.size());
    for (int i = 0; i < P.x.size(); ++i) {
        p[i].x = double_to_int(P.x[i], scale, delta_x);
        p[i].y = double_to_int(P.y[i], scale, delta_y);
    }
    return p;
}

IntPaths Paths_to_IntPaths(const Paths& Ps, double scale, double delta_x, double delta_y) {
    IntPaths ps;
    for (const auto & P : Ps) {
        ps.push_back(Path_to_IntPath(P, scale, delta_x, delta_y));
    }
    return ps;
}