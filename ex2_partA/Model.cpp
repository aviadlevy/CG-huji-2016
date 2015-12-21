//
//  Model.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Model.h"
#include <cmath>
#include <vector>
#include "stdlib.h"

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif



#define SHADERS_DIR "shaders/"

#define OBJECT_DEPTH 3.0f
#define OBJECT_B_RAD 2.0f

/**
 * Constructor
 *
 * initiate values
 */
Model::Model(int height, int width) :
_vaoCircle(0), _vboCircle(0), _vaoModel(0), _vboModel(0), _width(width), _height(height), _radius(0.75), _numPointsCircle(100), _isFull(true),
			_isPrespective(true), _zoomYStart(.0f), _zoomYEnd(1.0f), _tempZoom(1.0f), _moveXStart(0.f), _moveYStart(0.f),
			_moveXEnd(0.f), _moveYEnd(0.f), _moveXTemp(0.f), _moveYTemp(0.f)
{

}

/**
 * Destructor
 */
Model::~Model()
{
	if (_vaoCircle != 0)
		glDeleteVertexArrays(1, &_vaoCircle);
	if (_vboCircle != 0)
		glDeleteBuffers(1, &_vboCircle);
	if (_vaoModel != 0)
		glDeleteVertexArrays(1, &_vaoModel);
	if (_vboModel != 0)
		glDeleteBuffers(1, &_vboModel);
}

/**
 * init model programs
 */
void Model::init(char* filePath)
{
	programManager::sharedInstance()
	.createProgram("Circle",
				   SHADERS_DIR "CircleShader.vert",
				   SHADERS_DIR "CircleShader.frag");

	programManager::sharedInstance()
			.createProgram("Model",
						   SHADERS_DIR "ModelShader.vert",
						   SHADERS_DIR "ModelShader.frag");

	GLuint circleProgram = programManager::sharedInstance().programWithID("Circle");
	GLuint modelProgram = programManager::sharedInstance().programWithID("Model");
		
	// Obtain uniform variable handles:
	_fillColorUV  = glGetUniformLocation(circleProgram, "fillColor");
	_transformModelUV = glGetUniformLocation(modelProgram, "modelTransform");
	_transformCircleUV = glGetUniformLocation(circleProgram, "circleTransform");

	// Initialize vertices buffer and transfer it to OpenGL
	{
		// For this example we create a single triangle:
		float vertices[_numPointsCircle * 4];
		unsigned int i = 0;
		for(i = 0; i < _numPointsCircle * 4; i += 4)
		{
			float alpha = 2.0f * M_PI * (i / 4) / _numPointsCircle;
			vertices[i] = cosf(alpha) * _radius;
			vertices[i+1] = sinf(alpha) * _radius;
			vertices[i+2] = 0.0f;
			vertices[i+3] = 1.0f;
		}
		
		// Create and bind the object's Vertex Array Object:
		glGenVertexArrays(1, &_vaoCircle);
		glBindVertexArray(_vaoCircle);
		
		// Create and load vertex data into a Vertex Buffer Object:
		glGenBuffers(1, &_vboCircle);
		glBindBuffer(GL_ARRAY_BUFFER, _vboCircle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:

		// Obtain attribute handles:
		_posAttrib = glGetAttribLocation(circleProgram, "position");
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

	//draw model from mesh
	{
		if (!OpenMesh::IO::read_mesh(_mesh, filePath)) {
			// if we didn't make it, exit...
			std::cerr << "Error loading mesh, Aborting.\n";
			return;
		}


		glGenVertexArrays(1, &_vaoModel);
		glBindVertexArray(_vaoModel);

		// Create and load vertex data into a Vertex Buffer Object:
		glGenBuffers(1, &_vboModel);
		glBindBuffer(GL_ARRAY_BUFFER, _vboModel);

		readPointsOfModel();

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _meshVertices.size(), _meshVertices.data(), GL_STATIC_DRAW);

		// Obtain attribute handles:
		_posAttribModel = glGetAttribLocation(modelProgram, "positionModel");
		glEnableVertexAttribArray(_posAttribModel);
		glVertexAttribPointer(_posAttribModel, // attribute handle
							  4,          // number of scalars per vertex
							  GL_FLOAT,   // scalar type
							  GL_FALSE,
							  0,
							  0);

		// Unbind vertex array:
		glBindVertexArray(0);
	}

}

/**
 * responsible for drawing the 3D model inside mesh
 */
void Model::draw3dModel()
{

	// Set the program to be used in subsequent lines:
	GLuint modelProgram = programManager::sharedInstance().programWithID("Model");
	glUseProgram(modelProgram);

	glm::mat4 projectMat, translateMat, scaleMat, rotationMat;
	if(_isPrespective)
	{
		projectMat = glm::perspective(glm::radians(50.0f) / _tempZoom, 1.0f, OBJECT_DEPTH - OBJECT_B_RAD, OBJECT_DEPTH + OBJECT_B_RAD);
		translateMat = glm::translate(glm::mat4(), glm::vec3(_moveXTemp, _moveYTemp, -OBJECT_DEPTH));
	}
	else
	{
		projectMat = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
		translateMat = glm::translate(glm::mat4(), glm::vec3(_moveXTemp, _moveYTemp, .0f));
		scaleMat = glm::scale(glm::mat4(), glm::vec3(_tempZoom, _tempZoom, 1.0f));
	}
	rotationMat = _rotateOpMat * _rotationMat;
	// calculate the transform matrix to send to shader
	glm::mat4 transform = _screenRatio * scaleMat * projectMat * translateMat * rotationMat * _scaleMatModel;
	glUniformMatrix4fv(_transformModelUV, 1, GL_FALSE, glm::value_ptr(transform));
	// choose fill or wire
	GLenum polygonMode = _isFull ? GL_FILL : GL_LINE;
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	// Unbind the Vertex Array object
	glBindVertexArray(_vaoModel);
	_numOfPointsModel = _meshVertices.size();
	glDrawArrays(GL_TRIANGLES, 0, _numOfPointsModel);
	// Cleanup, not strictly necessary
	glUseProgram(0);
}

/**
 * responsible for the pink circle
 */
void Model::draw2dCircle() {
	// Set the program to be used in subsequent lines:
	GLuint circleProgram = programManager::sharedInstance().programWithID("Circle");
	glUseProgram(circleProgram);
	glUniformMatrix4fv(_transformCircleUV, 1, GL_FALSE, glm::value_ptr(_screenRatio));
	GLenum polygonMode = GL_FILL;   // Also try using GL_FILL and GL_POINT
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	// Set uniform variable with RGB values:
	float red = 1.0f; float green = 0.4f; float blue = 0.7f;
	glUniform4f(_fillColorUV, red, green, blue, 1.0);
	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vaoCircle);
	glDrawArrays(GL_LINE_LOOP, 0, _numPointsCircle);
	// Unbind the Vertex Array object
	glBindVertexArray(0);
	// Cleanup, not strictly necessary
	glUseProgram(0);
}

/**
 * resize screen
 */
void Model::resize(int width, int height)
{
    _width	= width;
    _height = height;
	// keep screen ratio so the drawing will not spread
	_screenRatio = glm::scale(glm::mat4(), glm::vec3(glm::max(_height / _width, 1.0f), glm::max(_width /_height, 1.0f), 1.0f));
}

/**
 * read the points of mesh and match them to our world ([-1, 1] axis)
 */
void Model::readPointsOfModel()
{
	// Vertex iterator is an iterator which goes over all the vertices of the mesh:
	Mesh::VertexIter vertexIter;
	Mesh::FaceIter faceIter;
	Mesh::FaceVertexIter faceVertexIter;
	Mesh::FaceHandle faceHandle;
	// This is used to store the geometrical position of the vertices of the mesh:
	Mesh::Point p;
	const float fm = std::numeric_limits<float>::max();
	_lowerLeft = Mesh::Point(fm, fm, fm);
	_upperRight = Mesh::Point(0,0,0);
	// number of vertices in the mesh:
	size_t vNum = _mesh.n_vertices();
	_meshVertices.reserve(vNum);
	vertexIter = _mesh.vertices_begin();
	_lowerLeft = _upperRight = _mesh.point(vertexIter);
	// This is how to go over all the vertices in the mesh:
	for(faceIter = _mesh.faces_begin(); faceIter != _mesh.faces_end(); ++faceIter) {
		faceHandle = faceIter.handle();
		for (faceVertexIter = _mesh.fv_iter(faceHandle); faceVertexIter; ++faceVertexIter) {
			// This is how to get the point associated with the vertex:
			p = _mesh.point(faceVertexIter);
			for (int i = 0; i < 3; i++) {
				_lowerLeft[i] = std::min(_lowerLeft[i], p[i]);
				_upperRight[i] = std::max(_upperRight[i], p[i]);
				_meshVertices.push_back(p[i]);
			}
			_meshVertices.push_back(1.0f);
		}
	}

	// find the size on each axis, and the center
	float xSize, ySize, zSize, xCenter, yCenter, zCenter;
	xSize = (_upperRight[0] - _lowerLeft[0]) / 2;
	ySize = (_upperRight[1] - _lowerLeft[1]) / 2;
	zSize = (_upperRight[2] - _lowerLeft[2]) / 2;
	xCenter = (_upperRight[0] + _lowerLeft[0]) / 2;
	yCenter = (_upperRight[1] + _lowerLeft[1]) / 2;
	zCenter = (_upperRight[2] + _lowerLeft[2]) / 2;

	// resizing
	for(std::vector<float>::iterator iter = _meshVertices.begin(); iter != _meshVertices.end(); iter += 4)
	{
		iter[0] -= xCenter;
		iter[0] /= xSize;
		iter[1] -= yCenter;
		iter[1] /= ySize;
		iter[2] -= zCenter;
		iter[2] /= zSize;
	}
	float radius =	glm::sqrt(xSize * xSize +  ySize * ySize +  zSize * zSize);
	// save the ratio
	_scaleMatModel = glm::scale(glm::mat4(), glm::vec3(xSize / radius, ySize / radius, zSize / radius));
}

/**
 * change wire/full state
 */
void Model::changeWire() {
	_isFull = !_isFull;
}
/**
 * change perspective/ortho state
 */
void Model::changePerspective() {
	_isPrespective = !_isPrespective;
}
/**
 * reset to start posiotion
 */
void Model::reset() {
	_isFull = true;
	_isPrespective = true;
	_zoomYStart = .0f;
	_zoomYEnd = 1.0f;
	_tempZoom = 1.0f;
	_moveXStart = 0.f;
	_moveYStart = 0.f;
	_moveXEnd = 0.f;
	_moveYEnd = 0.f;
	_moveXTemp = 0.f;
	_moveYTemp = 0.f;
	_rotateOpMat = glm::mat4();
	_rotationMat = glm::mat4();
}

/**
 * rotate 3D object
 */
void Model::rotate(int x, int y) {
	if(_isOutOfCircle)
	{
		return;
	}
	glm::vec3 vecStart = getArcBallVec(_rotateXStart, _rotateYStart);
	glm::vec3 vecEnd = getArcBallVec(x, y);
	glm::vec3 axisCameraCoord = glm::cross(vecStart, vecEnd);
	float angle = 2 * acos(glm::dot(vecStart, vecEnd));
	_rotateOpMat = glm::rotate(glm::mat4(), angle, axisCameraCoord);
	glutPostRedisplay();
}

/**
 * move 3D object
 */
void Model::move(int x, int y) {
	_moveXTemp = -(_moveXStart - x) / _width + _moveXEnd;
	_moveYTemp = (_moveYStart - y) / _height + _moveYEnd;
	glutPostRedisplay();
}

/**
 * zoom (scale) 3D object
 */
void Model::zoom(int y) {
	_tempZoom = glm::pow(1.004f, y - _zoomYStart) * _zoomYEnd;
	_tempZoom = glm::max(_tempZoom, 0.3f);
	_tempZoom = glm::min(_tempZoom, 12.f);
	glutPostRedisplay();
}

void Model::startRotate(int x, int y) {
	float newX = 1.0*x/_width*2 - 1.0;
	float newY = 1.0*y/_height*2 - 1.0;
	if (newX * newX + newY * newY > _radius * _radius) {
		_isOutOfCircle = true;
		return;
	}
	_isOutOfCircle = false;
	_rotateXStart = x;
	_rotateYStart = y;
}

void Model::stopRotate(int x, int y) {
	_rotationMat = _rotateOpMat * _rotationMat;
	_rotateOpMat = glm::mat4();
}

void Model::startMove(int x, int y) {
	_moveXStart = x;
	_moveYStart = y;
}

void Model::stopMove(int x, int y) {
	_moveXEnd = _moveXTemp;
	_moveYEnd = _moveYTemp;
}

void Model::startZoom(int y) {
	_zoomYStart = y;
}

void Model::stopZoom(int y) {
	_zoomYEnd = _tempZoom;
}

/**
 * create vec that match the arc ball
 */
glm::vec3 Model::getArcBallVec(float x, float y)
{
	glm::vec3 vec = glm::vec3(1.0*x/_width*2 - 1.0,
							  1.0*y/_height*2 - 1.0,
							  0);
	vec.y = -vec.y;
	float OP_squared = vec.x * vec.x + vec.y * vec.y;
	if (OP_squared <= 1*1)
		vec.z = sqrt(1*1 - OP_squared);  // Pythagore
	else
		vec = glm::normalize(vec);  // nearest point
	return vec;
}
