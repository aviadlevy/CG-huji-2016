//
//  lights.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : Declerae and implement classes representing ambient and
//            point light sources.
//

#ifndef __LIGHTS__
#define __LIGHTS__


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "MyVecs.h"
#include <vector>


//////////////////////////////
// Class Decleration        //
//////////////////////////////

class AmbientLight {
public:
    AmbientLight(Color3d color = COLOR_BLACK) : _color(color) { }

public:
    Color3d _color;

};


class PointLight {
public:
    PointLight(Point3d position = POINT_ORIGIN, Color3d color = COLOR_BLACK, double radius = EPS);

    Point3d _position;
    Color3d _color;
    double _radius;
    std::vector<Vector3d> _shadows;
};


#endif
