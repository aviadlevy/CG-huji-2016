//
// Created by aviadle on 1/11/16.
//

#include "lights.h"

PointLight::PointLight(Point3d position, Color3d color, double radius) :
        _position(position),_color(color), _radius(radius)
{

    int numRays = ceil(std::max(_radius, 1.0));
    for(int i = 0; i < numRays; i++)
    {
        Vector3d randVec = Vector3d(rand(), rand(), rand()).normalize();
        _shadows.push_back(randVec);
    }

    if (_radius > EPS)
    {
        _shadows.push_back(Vector3d(0, 0, 1));
        _shadows.push_back(Vector3d(0, 0, -1));
        _shadows.push_back(Vector3d(0, 1, 0));
        _shadows.push_back(Vector3d(0, -1, 0));
        _shadows.push_back(Vector3d(1, 0, 0));
        _shadows.push_back(Vector3d(-1, 0, 0));
    }
}
