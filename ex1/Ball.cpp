//
// Created by aviadle and mosheperetz.
//

#include "Ball.h"
#include "Model.h"

Ball::Ball(glm::vec3 color, glm::vec2 pos, glm::vec2 direction, const Model *model) : _color(color), _pos(pos),
                                                                                               _direction(direction),
                                                                                               _model(model)
{
    //check for walls boundary and decrease radius
    _shrinkRadius = _radius = DEFAULT_RADIUS;
    _shrinkRadius = _radius = std::min(_radius,1 - pos.x);
    _shrinkRadius = _radius = std::min(_radius,1 - pos.y);
    _shrinkRadius = _radius = std::min(_radius,pos.x + 1);
    _shrinkRadius = _radius = std::min(_radius,pos.y + 1);

    //check for balls near us and decrease radius
    for (unsigned int i = 0; i < model->_balls.size(); ++i)
    {
        GLfloat dist = glm::distance(pos, model->_balls.at(i)._pos) - model->_balls.at(i)._shrinkRadius;
        _shrinkRadius = _radius = std::min(_radius, dist);
    }
}

void Ball::updateBall()
{
    //change direction if we're beyond the walls
    if(_pos.x + _radius > 1 || _pos.x - _radius < -1)
    {
        _direction.x *= -1;
    }
    if(_pos.y + _radius > 1 || _pos.y - _radius < -1)
    {
        _direction.y *= -1;
    }
    _pos += glm::vec2(_direction.x, _direction.y);
}
