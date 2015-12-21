//
// Created by aviadle and mosheperetz.
//


#ifndef EX1_BALL_H
#define EX1_BALL_H

#include "ShaderIO.h"
#include <cmath>

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#define DEFAULT_RADIUS 0.1f

class Model;

class Ball {
public:
    Ball(glm::vec3 color, glm::vec2 pos, glm::vec2 direction, const Model* model);

    void updateBall();
    glm::vec3 _color;
    glm::vec2 _pos;
    glm::vec2 _direction;
    const Model* _model;
    GLfloat _radius;
    GLfloat _shrinkRadius;
};

#endif //EX1_BALL_H
