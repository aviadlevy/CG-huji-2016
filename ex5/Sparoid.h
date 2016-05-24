//
// Created by aviadle on 1/14/16.
//

#include "Ellipsoid.h"

#ifndef SPAROID_H
#define SPAROID_H

class Sparoid : public Ellipsoid
{
 public:
    Sparoid(Point3d C = POINT_ORIGIN, double r = 1, double xyVal = 1, double zVal = 1);

    virtual ~Sparoid() {}

};

#endif //EX5_SPAROID_H
