//
//  Model.cpp
//  cg-projects
//
//  Created by aviadle and mosheperetz.
//

#include "ShaderIO.h"
#include "Model.h"
#include "Ball.h"
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

#define SHADERS_DIR "shaders/"
#define RAND_F  (((float) rand() / RAND_MAX)*2 - 1)

Model::Model(float height, float width) :
		 _vao(0), _vbo(0), _height(height), _width(width), _numPoints(70)
{

}

Model::~Model()
{
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
}

void Model::init()
{
	programManager::sharedInstance()
	.createProgram("default",
				   SHADERS_DIR "SimpleShader.vert",
				   SHADERS_DIR "SimpleShader.frag");

	GLuint program = programManager::sharedInstance().programWithID("default");

	// Obtain uniform variable handles:
	_fillColorUV  = glGetUniformLocation(program, "fillColor");

	_transformLocUV = glGetUniformLocation(program, "transform");

	_radiusUV = glGetUniformLocation(program, "radius");

	_lightSourceUV = glGetUniformLocation(program, "lightSource");

	_centerUV = glGetUniformLocation(program, "center");

	_windowSizeUV = glGetUniformLocation(program, "windowSize");

	// Initialize vertices buffer and transfer it to OpenGL
	{

		// For this example we create a single triangle:
		float vertices[_numPoints * 4];
		int i = 0;
		for(i = 0; i < _numPoints * 4; i += 4)
		{
			float alpha = 2.0f * M_PI * (i / 4) / _numPoints;
			vertices[i] = cosf(alpha) * DEFAULT_RADIUS;
			vertices[i+1] = sinf(alpha) * DEFAULT_RADIUS;
			vertices[i+2] = 0.0f;
			vertices[i+3] = 1.0f;
		}
		
		// Create and bind the object's Vertex Array Object:
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);
		
		// Create and load vertex data into a Vertex Buffer Object:
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:

		// Obtain attribute handles:
		_posAttrib = glGetAttribLocation(program, "position");
		glEnableVertexAttribArray(_posAttrib);
		glVertexAttribPointer(_posAttrib, // attribute handle
							  4,          // number of scalars per vertex
							  GL_FLOAT,   // scalar type
							  GL_FALSE,
							  0,
							  0);
		
		// Unbind vertex array:
		glBindVertexArray(0);
	}
	createBall(glm::vec2(0, 0));
}

void Model::updateLocation()
{
	//check for wall collision
	for(unsigned int i = 0; i < _balls.size(); ++i)
	{
		_balls.at(i).updateBall();
	}
	//check for balls collision
	std::map<int, GLfloat> radiuses;
	for(unsigned int i = 0; i < _balls.size(); ++i)
	{
		float tempRadius = _balls.at(i)._radius;
		for(unsigned int j = 0; j < _balls.size(); ++j)
		{
			float tempRadius2 = tempRadius;
			if(i == j)
			{
				continue;
			}
			GLfloat dist = glm::distance(_balls.at(i)._pos, _balls.at(j)._pos);
			if (dist < _balls.at(i)._radius + _balls.at(j)._radius)
			{
				tempRadius2 = (dist - _balls.at(j)._shrinkRadius + _balls.at(i)._shrinkRadius) / 2;
			}
			if(tempRadius > tempRadius2)
			{
				tempRadius = tempRadius2;
			}
		}
		radiuses.insert(std::pair<int,GLfloat>(i, tempRadius));
	}
	for(unsigned int i = 0; i < _balls.size(); ++i)
	{
		_balls.at(i)._shrinkRadius = radiuses.at(i);
	}
}


void Model::draw()
{
	// Set the program to be used in subsequent lines:
	GLuint program = programManager::sharedInstance().programWithID("default");
	glUseProgram(program);

	GLenum polygonMode = GL_FILL;   // Also try using GL_FILL and GL_POINT
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	for(unsigned int i = 0; i < _balls.size(); ++i)
	{
		// Set uniform variable with RGB values:
		float red = _balls.at(i)._color.r; float green = _balls.at(i)._color.g; float blue = _balls.at(i)._color.b;
		glUniform4f(_fillColorUV, red, green, blue, 1.0);

		// set uniform variable of transform matrix
		glm::mat4 sc = glm::scale(glm::mat4(), glm::vec3(_balls.at(i)._shrinkRadius / DEFAULT_RADIUS, _balls.at(i)._shrinkRadius / DEFAULT_RADIUS, 0));
		_balls.at(i)._pos += _balls.at(i)._direction;
		glm::mat4 tr = glm::translate(glm::mat4(), glm::vec3(_balls.at(i)._pos.x, _balls.at(i)._pos.y , 0.0f));
		glm::mat4 trans = tr * sc;
		glUniformMatrix4fv(_transformLocUV, 1, GL_FALSE, glm::value_ptr(trans));

		//Send the radius to shader
		glUniform1f(_radiusUV, _balls.at(i)._shrinkRadius);

		//send light source
		glUniform2f(_lightSourceUV,2.0f,2.0f);

		//Send the radius to shader
		glUniform2f(_centerUV, _balls.at(i)._pos.x, _balls.at(i)._pos.y);

		//Send the WS
		glUniform2f(_windowSizeUV, _width, _height);

		// Draw using the state stored in the Vertex Array object:

		glBindVertexArray(_vao);
		size_t numberOfVertices = _numPoints;
		glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	}

	
	// Unbind the Vertex Array object
	glBindVertexArray(0);


	
	// Cleanup, not strictly necessary
	glUseProgram(0);
}

void Model::resize(int width, int height)
{
    _width	= width;
    _height = height;
    _offsetX = 0;
    _offsetY = 0;

	_numPoints = std::min(70, std::min(width, height));

	float vertices[_numPoints * 4];
	int i = 0;
	for(i = 0; i < _numPoints * 4; i += 4)
	{
		float alpha = 2.0f * M_PI * (i / 4) / _numPoints;
		vertices[i] = cosf(alpha) * DEFAULT_RADIUS;
		vertices[i+1] = sinf(alpha) * DEFAULT_RADIUS;
		vertices[i+2] = 0.0f;
		vertices[i+3] = 1.0f;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Model::createBall(glm::vec2 pos)
{

	//check if we clicked on another ball
	for (unsigned int i = 0; i < _balls.size(); ++i) {
		if (glm::distance(pos, _balls.at(i)._pos) <= _balls.at(i)._shrinkRadius) {
			return;
		}
	}

	//create the color to the ball. make the color bright enough so we'll see it..
	glm::vec3 randColor;
	do {
		randColor = glm::vec3(fabs(RAND_F), fabs(RAND_F), fabs(RAND_F));
		if (randColor.r < 0.07 || randColor.g < 0.07 || randColor.b < 0.07) {
			break;
		}
	}
	while (true);

	_balls.push_back(Ball(randColor, pos, glm::normalize(glm::vec2(RAND_F, RAND_F)) * 0.005f, this));
}
