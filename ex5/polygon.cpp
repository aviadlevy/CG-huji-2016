//
// Created by aviadle on 1/11/16.
//

#include "polygon.h"


Polygon::Polygon(vector<Point3d>& vertices) : Object(), _vertices(vertices)
{
    triangulate();
}

Polygon::Polygon(vector<Point3d>& vertices, Vector3d& normal) : Object(), _vertices(vertices), _normal(normal)
{
    triangulate();
}

Polygon::Polygon(vector<Point3d>& vertices, vector<Point2d> textices) : Object(), _textices(textices), _vertices(vertices)
{

    triangulate();
}

Polygon::Polygon(vector<Point3d>& vertices, vector<Point2d> textices,
                 Vector3d& normal) : Object(), _textices(textices), _vertices(vertices),  _normal(normal)
{
    triangulate();
}

void Polygon::triangulate() {
    if (_vertices.size() <= 2) {
        return;
    }
    Point3d p = _vertices[0];
    for (size_t i = 2; i < _vertices.size(); i++)
    {
        Triangle* t;
        if (!_textices.empty())
        {
            t = new Triangle(p, _vertices[i-1], _vertices[i], _textices[0], _textices[i-1], _textices[i]);
        }
        else
        {
            t = new Triangle(p, _vertices[i-1], _vertices[i]);
        }
        _triangles.push_back(t);
    }
}

int Polygon::intersect(Ray &ray, double tMax, double &t, Point3d &P, Vector3d &N, Color3d &texColor) const{
    int ret = 0;
    for (size_t i = 0; i < _triangles.size(); i++)
    {
        Triangle* tri = _triangles[i];
        Ray p = tri->getPlane();
        double c = ray.D() | p.D();
        if (fabs(c) > EPS)
        {
            t = ((p.O() | p.D()) - (p.D() | ray.O())) / (c);
            if (t > 0 && t < tMax)
            {
                Point2d texCoords;
                P = ray(t);
                if (tri->isInside(P, texCoords))
                {
                    ret = 1;
                    N = p.D();
                    texColor = getDiffuse();
                    if (_diffuseTexture)
                    {
                        double u,v;
                        if (_textices.empty()) // Does the polygon have a texture map coordinates  //
                        {
                            u = texCoords[0];
                            v = texCoords[1];
                        }
                        else
                        {
                            u = _triangles[i]->get_t0()[X] * (1 - texCoords[0] - texCoords[1]) + _triangles[i]->get_tu()[X] *texCoords[0] + _triangles[i]->get_tv()[X] * texCoords[1];
                            v = _triangles[i]->get_t0()[Y] * (1 - texCoords[0] - texCoords[1]) + _triangles[i]->get_tu()[Y] *texCoords[0] + _triangles[i]->get_tv()[Y] * texCoords[1];
                        }

                        BImage* texture = const_cast<BImage*>(getDiffuseTexture()); //HACK, BImage doesn't support const operations...

                        u *= texture->width();
                        v *= texture->height();

                        Bpixel pix = (*texture)(u, v);
                        texColor *= Color3d(pix.r, pix.g, pix.b) * COLOR_NORMALIZE;
                    }
                }
            }
        }
        if (ret == 1)
            break;
    }
    return ret;
}

