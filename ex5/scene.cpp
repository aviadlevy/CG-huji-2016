//
// Created by aviadle on 1/11/16.
//

#include "scene.h"

Scene::Scene(Color3d &color, AmbientLight &light, double cutoffAngle) : _ambientLight(light), _background(color),
                                                                        _cutoffAngle(cutoffAngle) {
}

Color3d Scene::trace_ray(Ray ray, double vis) const {
    Color3d ret = _background, reflect = COLOR_BLACK, refract = COLOR_BLACK;
    ray = Ray(ray(EPS * 10), ray.D());
    if (vis > MINIMAL_VIS) {
        const Object *obj = 0;
        double t;
        Point3d P;
        Vector3d N;
        Color3d texColor;
        if (findNearestObject(ray, &obj, t, P, N, texColor)) {
            if (obj->isRefractive()) {
                Vector3d tempRefracted;
                double m = 1 / obj->getIndex();
                Vector3d refractNorm = N;
                Vector3d dir = ray.D();
                dir = -dir;
                dir.normalize();
                refractNorm.normalize();
                double NDir = (refractNorm | dir);
                double det = 1 - m * m * (1 - NDir * NDir);
                if (det > EPS) {
                    tempRefracted = N * (m * NDir - sqrt(det)) - m * dir;
                }
                else {
                    tempRefracted = (dir - (2 * (dir | refractNorm) * refractNorm)).normalize();
                }
                Ray refracted(P, tempRefracted);
                for (size_t s = 0; s < _numberOfRefRays; s++) {
                    if (_numberOfRefRays > 1) {
                        refracted = perturbateRay(refracted);
                    }
                    refract += obj->getTransparency() * trace_ray(refracted, vis * RECURSION_FACTOR);
                }
                refract /= _numberOfRefRays;
            }

            if (obj->getReflection() != COLOR_BLACK) {
                Vector3d dir = ray.D();
                Vector3d refractedNorm = N;
                dir.normalize();
                refractedNorm.normalize();
                Ray reflected(P, (dir - (2 * (dir | refractedNorm) * refractedNorm)).normalize());

                for (size_t s = 0; s < _numberOfRefRays; s++) {
                    if (_numberOfRefRays > 1) {
                        reflected = perturbateRay(reflected);
                    }
                    reflect += (obj->getReflection() * trace_ray(reflected, vis * RECURSION_FACTOR));
                }
                reflect /= _numberOfRefRays;
            }

            Color3d lights = obj->getAmbient() * _ambientLight._color;
            for (size_t i = 0; i < _lights.size(); i++) {
                bool isShadow = true;
                Vector3d L;
                for (size_t s = 0; s < _lights[i]->_shadows.size(); s++) {
                    Vector3d lightPos = _lights[i]->_position + _lights[i]->_shadows[s] * _lights[i]->_radius;
                    L = (lightPos - P);
                    double maxT = L.length();
                    L.normalize();
                    isShadow = findNearestObject(Ray(lightPos, -L), obj, maxT);
                    if (!isShadow) {
                        break;
                    }
                }
                if (isShadow) {
                    continue;
                }
                if ((L | N) > EPS) {
                    Color3d diffuse = texColor * _lights[i]->_color * (L | N);
                    lights += diffuse;
                    L.normalize();
                    N.normalize();
                    Vector3d R = (L - (2 * (L | N) * N)).normalize();
                    Color3d specular = obj->getSpecular() * _lights[i]->_color *
                                       pow(std::max(ray.D() | R, EPS), obj->getShining());
                    lights += specular;
                }
            }
            ret = lights;
            ret += reflect;
            ret += refract;
        }
    }
    return ret;
}

void Scene::add_object(Object *obj) {
    _objects.push_back(obj);
}

void Scene::add_light(PointLight *light) {
    _lights.push_back(light);
}

Ray Scene::perturbateRay(const Ray &r) const {
    Vector3d pert(rand(), rand(), rand());
    pert.normalize();
    pert *= sin(M_PI * _cutoffAngle / 180);
    return Ray(r.O(), r.D() + pert);
}

bool Scene::findNearestObject(Ray ray, const Object* object, double t) const
{
    for (size_t i=0; i < _objects.size(); i++)
    {
        const Object* it = _objects[i];
        double t2;
        Point3d P;			//unused
        Vector3d N;			//unused
        Color3d texColor;	//unused

        int isIntersect = it->intersect(ray, INF, t2, P, N, texColor);
        if (isIntersect == 1 && t2 > EPS && t2 < t - EPS)
        {
            return true;
        }
    }

    return false;

}

bool Scene::findNearestObject(Ray ray, const Object **object, double &t, Point3d &P, Vector3d &N, Color3d &texColor) const {
    bool ret = false;
    double close = INF;
    for (size_t i=0; i < _objects.size(); i++)
    {
        Object* it = _objects[i];
        int isIntersect = it->intersect(ray, close, t, P, N, texColor);
        if (isIntersect == 1 && t < close)
        {
            close = t;
            *object = _objects[i];
            ret = true;
        }
    }
    return ret;
}
