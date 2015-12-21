//
//  Model.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__Model__
#define __ex0__Model__

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include "Ball.h"
#include <vector>

class Ball;

class Model {
	
	GLuint _vao, _vbo;

	// Attribute handle:
	GLint _posAttrib, _fillColorUV, _transformLocUV, _lightSourceUV, _centerUV, _windowSizeUV;
	GLfloat _radiusUV;

	friend class Ball;
	std::vector<Ball> _balls;




private:
// View port frame:
	float _height, _width, _offsetX, _offsetY;

	int _numPoints;

public:
	Model(float height, float width);
	virtual ~Model();
	void init();
	void draw();
	void resize(int width, int height);

	inline float getWidth() const {
		return _width;
	}

	inline float getHeight() const {
		return _height;
	}
	void updateLocation();
	void createBall(glm::vec2 pos);


};

#endif /* defined(__ex0__Model__) */
