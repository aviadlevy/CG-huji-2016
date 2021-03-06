//
//  triangle.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : A class that represents a triangle on the 3d space.
//            Implements a method checking if a given point is inside
//            the triangle.
//

#ifndef _TRIANGLE_HH
#define _TRIANGLE_HH


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "MyVecs.h"
#include "ray.h"


//////////////////////////////
// Class Decleration        //
//////////////////////////////

class Triangle {
public:
// Constructor - create a triangle from the given points  //
    Triangle(Point3d p0, Point3d p1, Point3d p2);

    Triangle(Point3d p0, Point3d p1, Point3d p2, Point2d t0, Point2d t1, Point2d t2);


// Destructor - get rid of a triangle //
    ~Triangle() {}

// isInside - return true if the given point is inside the triangle //
    bool isInside(IN const Point3d &p, OUT Point2d &texCoord) const;

private:
    Point3d _p0;  // One of the triangle vertices   //
    Point3d _p1;  // One of the triangle vertices   //
    Point3d _p2;  // One of the triangle vertices   //

    Vector3d _u;  // a vector from p0 to p1         //
    Vector3d _v;  // a vector from p0 to p2         //

    Point2d _t0;  // The texture map coordiate of p0  //
    Point2d _tu;  // The texture map coordinate of p1 //
    Point2d _tv;  // The texture map coordinate of p2 //
    Vector3d _norm;
    bool _textured;

// Used for checking if a point is inside the triangle  //
    double _uu;
    double _uv;
    double _vv;
    double _d;

public:
    Vector3d get_norm() const {
        return _norm;
    }

    void set_norm(Vector3d _norm) {
        Triangle::_norm = _norm;
    }

    Point2d get_tv() const {
        return _tv;
    }

    void set_tv(const Point2d &_tv) {
        Triangle::_tv = _tv;
    }

    Point2d get_tu() const {
        return _tu;
    }

    void set_tu(const Point2d &_tu) {
        Triangle::_tu = _tu;
    }

    Point2d get_t0() const {
        return _t0;
    }

    void set_t0(const Point2d &_t0) {
        Triangle::_t0 = _t0;
    }

    Ray getPlane() {
        return Ray(_p0, _norm);
    }
};

#endif /* _TRIANGLE_HH */
