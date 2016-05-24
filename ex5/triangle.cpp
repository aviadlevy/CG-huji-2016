//
// Created by aviadle on 1/11/16.
//

#include "triangle.h"

Triangle::Triangle(Point3d p0, Point3d p1, Point3d p2) : _p0(p0), _p1(p1), _p2(p2), _u(p1 - p0), _v(p2 - p0)
{
    _norm = (_u % _v).normalize();
    _d = (_u % _v).norm();
}

Triangle::Triangle(Point3d p0, Point3d p1, Point3d p2, Point2d t0, Point2d t1, Point2d t2) : _p0(p0), _p1(p1), _p2(p2),
                                                                                             _u(p1 - p0), _v(p2 - p0),
                                                                                             _t0(t0), _tu(t1), _tv(t2)
{
    _norm = (_u % _v).normalize();
    _d = (_u % _v).norm();
}

bool Triangle::isInside(const Point3d &p, Point2d &texCoord) const {
    Vector3d e1 = _p0 - p;
    Vector3d e2 = _p1 - p;
    Vector3d e3 = _p2 - p;
    double w = (e2 % e3).norm() / _d;
    texCoord[X] = (e1 % e3).norm() / _d;
    texCoord[Y] = (e1 % e2).norm() / _d;
    return (texCoord[X] + texCoord[Y] + w - 1 < EPS);
}
