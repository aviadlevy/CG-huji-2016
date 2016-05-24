//
// Created by aviadle on 1/14/16.
//

#include "Sparoid.h"

Sparoid::Sparoid(Point3d C, double r, double xyVal, double z) : Ellipsoid(C, r, Vector3d(xyVal, z, xyVal)) {

}
