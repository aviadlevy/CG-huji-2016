//
// Created by aviadle on 1/11/16.
//

#include "MyMeshObject.h"

MyMeshObject::MyMeshObject(MyMesh &mesh) {
    _mesh = mesh;
    readingMesh();
    calculateBoundingSphere();
}

MyMeshObject::~MyMeshObject() {
    if (_boundingSphere)
    {
        delete(_boundingSphere);
    }
}

void MyMeshObject::readingMesh() {
    _mesh.request_face_normals();
    _mesh.update_normals();
    _mesh.request_vertex_texcoords2D();
    bool isTex = _mesh.has_vertex_texcoords2D();
    bool isNormal =_mesh.has_face_normals();
    for (MyMesh::FaceIter h_it=_mesh.faces_begin(); h_it!=_mesh.faces_end(); ++h_it)
    {
        vector<Point3d> vertices;
        vector<Point2d> textices;
        for (MyMesh::FaceVertexIter fv_it = _mesh.fv_iter(h_it); fv_it; ++fv_it)
        {
            MyMesh::Point p = _mesh.point(fv_it.handle());
            vertices.push_back(p);

            if (isTex)
            {
                MyMesh::TexCoord2D uv = _mesh.texcoord2D(fv_it);
                textices.push_back(uv);
            }
        }
        if (isTex)
        {
            if (isNormal)
            {
                Vector3d n = _mesh.normal(h_it);
                _polygons.push_back(Polygon(vertices, textices, n));
            }
            else
            {
                _polygons.push_back(Polygon(vertices, textices));
            }
        }
        else
        {
            if (isNormal)
            {
                Vector3d n = _mesh.normal(h_it);
                _polygons.push_back(Polygon(vertices, n));
            }
            else
            {
                _polygons.push_back(Polygon(vertices));
            }
        }
        _polygons.back().setParent(this);
    }
    _mesh.release_face_normals();
}

void MyMeshObject::calculateBoundingSphere() {
    Point3d center = Point3d(0, 0, 0);
    double r = 0.0;
    MyMesh::VertexIter vertexIter;
    int vNum = _mesh.n_vertices();
    for (vertexIter = _mesh.vertices_begin(); vertexIter != _mesh.vertices_end(); ++vertexIter)
    {
        center += _mesh.point(vertexIter);
    }
    center /= (double)vNum;
    for (vertexIter = _mesh.vertices_begin(); vertexIter != _mesh.vertices_end(); ++vertexIter)
    {
        Point3d p = center - _mesh.point(vertexIter);
        r = std::max(r, p.length());
    }
    _boundingSphere = new Sphere(center, r + EPS);
}

void MyMeshObject::set_texture_map(BImage *image) {
    _diffuseTexture = image;
    for (size_t i=0; i<_polygons.size(); i++)
    {
        _polygons[i].set_texture_map(image);
    }
}

int MyMeshObject::intersect(Ray &ray, double tMax, double &t, Point3d &P, Vector3d &N, Color3d &texColor) const{
    int ret = 0;
    if (!_boundingSphere->roots(ray))
    {
        return 0;
    }
    for (size_t i = 0; i < _polygons.size(); i++)
    {
        int intersectPolygon = _polygons[i].intersect(ray, tMax, t, P, N, texColor);
        if (intersectPolygon == 1)
        {
            tMax = t;
            ret = 1;
        }
    }
    return ret;
}

