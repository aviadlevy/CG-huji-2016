//
// Created by aviadle on 1/12/16.
//

#include "Ellipsoid.h"

Ellipsoid::Ellipsoid(Point3d C, double r, Vector3d scale)  : _C(C), _r(r), _scale(scale)
{
    _scaleInv = Vector3d(1.0/_scale[0], 1.0/_scale[1], 1.0/_scale[2]);
}

Ellipsoid::~Ellipsoid()
{
}


bool Ellipsoid::roots(const Ray &ray, double *t0, double *t1) const
{
    //Compute A, B and C coefficients
    using OpenMesh::dot;
    double a = dot(ray.D(), ray.D()); //should be 1
    double b = 2 * dot(ray.D(), ray.O() - _C);
    double c = dot(ray.O() - _C, ray.O() - _C) - (_r * _r);

    //Find discriminant
    double disc = b * b - 4 * a * c;

    // if discriminant is negative there are no real roots, so return
    // false as ray misses Ellipsoid
    if (disc < -EPS)
        return false;

    if (t0 && t1) //enable null input for intersection test only
    {
        // compute q as described above
        double distSqrt = sqrt(disc);
        *t0 = (-b - distSqrt) * 0.5;
        *t1 = (-b + distSqrt) * 0.5;

        if (t0 > t1)
        {
            std::swap(t0, t1);
        }
    }

    return true;
}

int Ellipsoid::intersect(Ray& inRay, double tMax, double& t, Point3d& P, Vector3d& N,
                         Color3d& texColor) const
{
    int retVal = 0;

    //transform ray and ellipsoid to canonical represntation
    Ray ray((inRay.O() - _C) * _scaleInv + _C, inRay.D() * _scaleInv);


    double t0, t1;
    if (!roots(ray, &t0, &t1))
        return 0;

    if (t0 > EPS)
    {
        retVal = 1;
        t = t0;
    }
    else if (t1 > EPS)
    {
        retVal = 1;
        t = t1;
    }

    if (t > tMax) //avoid further computation aince we look for the closest object
    {
        retVal = 0;
    }

    if (retVal == 1)
    {

        P = ray(t);

        //tranform back to original cartesian representation
        P -= _C;
        P *= _scale;
        P += _C;

        N = (P - _C);
        N *= _scaleInv;
        N.normalize();

        texColor = _diffuse;
        if (_diffuseTexture)
        {
            texColor *= textureDiffuse(N);
        }



    }

    return retVal;
}

Color3d Ellipsoid::textureDiffuse(const Point3d& P) const
{
    //sample texture from the given sphere
    float theta = atan(P[X] / P[Z]);
    float phi   = atan(P[Y] / Point2d(P[X],P[Z]).length());

    float u = (theta + M_PI) / (2 * M_PI);
    float v = (phi + M_PI/2)  / M_PI;

    u *= _diffuseTexture->width();
    v *= _diffuseTexture->height();

    Bpixel pix = (*_diffuseTexture)(u, v);
    return Color3d(pix.r, pix.g, pix.b) * COLOR_NORMALIZE;

}