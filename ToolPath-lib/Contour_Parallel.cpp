//
// Created by TianKai Ma on 2023/11/24.
//

#include "Contour_Parallel.h"
#include "clipper2/clipper.h"

Paths Contour_Parallel(const Path &contour, const Paths &holes, double distance) {
    Clipper2Lib::PathD subject = std::vector<Clipper2Lib::PointD>(contour.x.size());
    // iterate over the contour
    for (int i = 0; i < contour.x.size(); ++i) {
//        subject.push_back(Clipper2Lib::MakePathD({
//            contour.x[i-1], contour.y[i-1], contour.x[i], contour.y[i]
//        }));
        subject[i] = Clipper2Lib::PointD(contour.x[i], contour.y[i]);
    }

//    Clipper2Lib::ClipperD clipper = Clipper2Lib::ClipperD();
//    clipper.AddSubject(subject);

    Paths result;

    for (int count = 0; count <= 100; count++) {
        // {Polygon, Joined, Butt, Square, Round};
        Clipper2Lib::PathsD sol = Clipper2Lib::InflatePaths(Clipper2Lib::PathsD{subject}, -count * distance,
                                                            Clipper2Lib::JoinType::Square,
                                                            Clipper2Lib::EndType::Joined);

//        sol = Clipper2Lib::Intersect(sol, subject, Clipper2Lib::FillRule::EvenOdd);
//        clipper.Execute(Clipper2Lib::ClipType::Union, Clipper2Lib::FillRule::NonZero, sol);

        for (auto &path: sol) {
            Path p;
            for (int i = 0; i < path.size(); ++i) {
                if (Clipper2Lib::PointInPolygon(path[i], subject) == Clipper2Lib::PointInPolygonResult::IsOutside) {
                    continue;
                }

                p.x.push_back(path[i].x);
                p.y.push_back(path[i].y);
            }
            if (p.x.size() < 3) continue;
            result.push_back(p);
        }
    }
    return result;
//    offseter.AddPaths(subject, Clipper2Lib::JoinType::Round, Clipper2Lib::EndType::Polygon);


}