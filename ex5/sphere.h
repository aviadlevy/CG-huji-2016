//
//  sphere.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : A class that represents a sphere on the 3d space.
//            Inherits from Object class, implementing the method to test
//            intersection of a given ray with the sphere.
//

#ifndef _SPHERE_HH
#define _SPHERE_HH


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "Ellipsoid.h"


//////////////////////////////
// Class Decleration        //
//////////////////////////////


class Sphere : public Ellipsoid
{
public:
  Sphere(Point3d C = POINT_ORIGIN, double r = 1);

    virtual ~Sphere() {}


    inline bool isInside(Point3d p) const
    {
        return (p-_C).length() < _r;
    }
} ;

#endif /* _SPHERE_HH */
