//
// Created by TianKai Ma on 2023/11/24.
//

#include "Contour_Parallel.h"
#include "clipper2/clipper.h"

Paths OffsetClipper(const double* x, const double* y, double dis, int length/) {
    double xmax = x[0];
    double xmin = x[0];
    double ymax = y[0];
    double ymin = y[0];
    double delta_x = 0.0;
    double delta_y = 0.0;
    for (int i = 1; i < length; ++i) {
        xmax = (std::max)(xmax, x[i]);
        xmin = (std::min)(xmin, x[i]);
        ymax = (std::max)(ymax, y[i]);
        ymin = (std::min)(ymin, y[i]);
    }
    delta_x = (xmax + xmin) * 0.5;
    delta_y = (ymax + ymin) * 0.5;
    double scale = CLIPPER_BOUND / std::max(xmax - xmin, ymax - ymin);

    IntPath contour = Path_to_IntPath(x, y, length, scale, delta_x, delta_y);

    Paths solution;
    ClipperOffset co;
    co.AddPath(contour, jtRound, etClosedPolygon);
    co.Execute(solution, -dis * scale);

    Paths ps;
    
    return ps;
}

double TotalLength(const std::vector<double> &x, const std::vector<double> &y, ) {
    int length = x.size();
    double L = 0.0;
    for (int i = 0; i < length - 1; ++i) {
        L += sqrt((x[i + 1] - x[i]) * (x[i + 1] - x[i]) + (y[i + 1] - y[i]) * (y[i + 1] - y[i]));
    }

    return L;
}

Paths do1offset(const Path& p) {
    Paths ps;
    double Length = TotalLength(p.x, p.y);
    Paths ps1 = ContourParallel::OffsetClipper(p.x, p.y, opts.delta * 1.001, p.length, opts.wash, min(opts.washdis, Length * 1.0 / opts.num_least));
    if (ps1.size()) {
        for (int i_Path = 0; i_Path < ps1.size(); ++i_Path) {
            Length = Curve::TotalLength(ps1[i_Path].x, ps1[i_Path].y, ps1[i_Path].length);
            Paths ps2 = ContourParallel::OffsetClipper(ps1[i_Path].x, ps1[i_Path].y, -opts.delta * 1.001, ps1[i_Path].length, opts.wash, min(opts.washdis, Length * 1.0 / opts.num_least));
            for (int j = 0; j < ps2.size(); ++j) {
                double lambda = 0.1;
                double* deltas = Optimize_QSL(ps2[j], opts);
                if (deltas) {
                    double max_deltas = deltas[0];
                    for (register int i = 1; i < ps2[j].length; ++i) {
                        max_deltas = (std::max)(max_deltas, deltas[i]);
                    }
                    for (register int i = 0; i < ps2[j].length; ++i) {
                        deltas[i] += opts.delta - max_deltas;
                    }
                    ps.push_back(Path());
                    ps[ps.size() - 1].length = ps2[j].length;
                    Curve::OffsetNaive(ps2[j].x, ps2[j].y, deltas, ps2[j].length, ps[ps.size() - 1].x, ps[ps.size() - 1].y);
                    delete[] deltas;
                }
                else {
                    Paths ps3 = ContourParallel::OffsetClipper(ps2[j], Paths(), opts.delta, opts.wash, opts.washdis, opts.num_least);
                    ps.push_back(ps3[0]);
                }
            }
        }
    }
    return ps;
}

PathNode* root_offset(const Path& contour, const Paths& holes) {
    PathNode* root = new PathNode(contour);
    root->parent = NULL;
    std::stack<PathNode*> S;
    S.push(root);

    while (!S.empty()) {
        PathNode* pn_parent = S.top();
        S.pop();
        Paths ps_offset = do1offset(pn_parent->data);
        for (int i_offset = 0; i_offset < ps_offset.size(); ++i_offset) {
            Paths ps_clip = ps_offset[i_offset];
            for (int i_clip = 0; i_clip < ps_clip.size(); ++i_clip) {
                Path p = ps_clip[i_clip];
                PathNode* pn_child = new PathNode(p);
                pn_parent->children.push_back(pn_child);
                pn_child->parent = pn_parent;
                S.push(pn_child);
            }
        }
    }
    return root;
}

Paths Contour_Parallel(const Path& contour, const Paths& holes, double dis) {
    PathNode* root = root_offset(contour, holes, dis, wash, wash_distance);
    std::vector<PathNode*> * dfs = PathNode::DFS_root(root);
    Paths solution;
    for (int i = 0; i < dfs->size(); ++i) {
        solution.emplace_back();
        solution[i].x = (*dfs)[i]->data.x;
        solution[i].y = (*dfs)[i]->data.y;
        for (auto & j : (*dfs)[i]->children) {
            j = nullptr;
        }
    }

    return solution;
}