//
// Created by aviadle on 1/12/16.
//

#ifndef EX5_SPAROID_H
#define EX5_SPAROID_H


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "object.h"


//////////////////////////////
// Class Decleration        //
//////////////////////////////

class Ellipsoid : public Object
{
public:

    // Constructor - create a Ellipsoid with the given parameters  //
    Ellipsoid(Point3d C = POINT_ORIGIN, double r = 1, Vector3d _scale = Vector3d(1,1,1));

    // Destructor - delete an Ellipsoid  //
    virtual ~Ellipsoid();

    // Ray Ellipsoid intersection //
    virtual int intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor) const;

    Color3d textureDiffuse(const Point3d& P) const;

    bool roots(const Ray &ray, double *t0 = NULL, double *t1 = NULL) const;

    Vector3d getCenter() const {return _C;}

protected:

    Point3d   _C; // The Ellipsoid's center  //
    double    _r; // The Ellipsoid's underlying sphere radius  //

    //scale factor for x,y,z axis, respectively
    Vector3d _scale;
    Vector3d _scaleInv; //optimization


};

#endif //EX5_SPAROID_H
