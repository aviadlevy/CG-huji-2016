//
// Created by aviadle on 1/5/16.
//

#include "camera.h"
#include "bimage.h"
#include "ray.h"
#include "scene.h"

Camera::Camera() {
    _position = Point3d();
    _coi = Point3d(0, 0, 1);
    _up = Point3d(0, 1, 0);
    _fov_h = 45;
    _samples_per_pixel = 1;
}

Camera::Camera(Point3d &pos, Point3d &coi, Vector3d &up, double fov, size_t samples_per_pixel) {
    _position = pos;
    _coi = coi;
    _up = up;
    _fov_h = fov;
    _samples_per_pixel = samples_per_pixel;
}

void Camera::render(size_t row_start, size_t number_of_rows, BImage &img, Scene &scene) const {
    const int w = img.getWidth();
    const int h = img.getHeight();
    const float aspectRatio = float(img.getHeight()) / img.getWidth();
    Vector3d cor = (_coi - _position).normalize();
    Vector3d horizon = (cor % _up).normalize();
    Vector3d vert = (cor % horizon).normalize();
    Vector3d dx = horizon * 2 * tan(_fov_h) / img.getWidth();
    Vector3d dy = vert * 2 * aspectRatio * tan(_fov_h) / img.getHeight();
    for (size_t samples = 0; samples < _samples_per_pixel; samples++) {
        // if number of samples == 1: shoot a ray through the pixel center, else shoot random rays
        float sx, sy;
        if (_samples_per_pixel == 1) {
            sx = 0.5;
            sy = 0.5;
        }
        else {
            sx = float(rand()) / RAND_MAX;
            sy = float(rand()) / RAND_MAX;
        }
        for (size_t i = row_start; i < row_start + number_of_rows; ++i) {
            for (int j = 0; j < w; ++j) {
                Vector3d dir = (j + sx - w * 0.5) * dx + (i + sy - h * 0.5) * dy + cor;
                Ray r(_position, dir);
                Color3d color = scene.trace_ray(r, 1.0);
                for (int k = 0; k < 3; ++k)
                    color[k] = std::min(color[k], 1.0);
                color *= 255;
                img(j, i) = Bpixel(color[0], color[1], color[2]);

            }
        }

    }//end samples
}

void Camera::setSamplesPerPixel(size_t samples_per_pixel) {
    _samples_per_pixel = samples_per_pixel;
}
