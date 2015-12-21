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

struct face_indices
{
	GLuint x, y, z;
};

#define SHADERS_DIR "shaders/"

#define OBJECT_DEPTH 3.0f
#define OBJECT_B_RAD 2.0f

/**
 * Constructor
 *
 * initiate values
 */
Model::Model(int height, int width) :
_vaoCircle(0), _vboCircle(0), _vaoModel(0), _vboModel(0), _vaoModelFaces(0), _vboModelFace(0), _elementBo(0),
			_width(width), _height(height), _radius(0.75), _numPointsCircle(100), _isFull(true),
			_isPrespective(true), _zoomYStart(.0f), _zoomYEnd(1.0f), _tempZoom(1.0f), _moveXStart(0.f), _moveYStart(0.f),
			_moveXEnd(0.f), _moveYEnd(0.f), _moveXTemp(0.f), _moveYTemp(0.f), _shadingMode(PHONG_MODE), _normalMode(VERT_NORMAL_MODE),
			_spec(200)
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
			.createProgram("RGB",
						   SHADERS_DIR "RGBShader.vert",
						   SHADERS_DIR "RGBShader.frag");

	programManager::sharedInstance()
			.createProgram("Phong",
						   SHADERS_DIR "PhongShader.vert",
						   SHADERS_DIR "PhongShader.frag");

	programManager::sharedInstance()
			.createProgram("Gouraud",
						   SHADERS_DIR "GouraudShader.vert",
						   SHADERS_DIR "GouraudShader.frag");

	GLuint circleProgram = programManager::sharedInstance().programWithID("Circle");
	_phongProgram = programManager::sharedInstance().programWithID("Phong");
	_gouraudProgram = programManager::sharedInstance().programWithID("Gouraud");
	_rgbProgram = programManager::sharedInstance().programWithID("RGB");

	setShading(PHONG_MODE);

	// Obtain uniform variable handles:
	_fillColorUV  = glGetUniformLocation(circleProgram, "fillColor");
	_transformCircleUV = glGetUniformLocation(circleProgram, "circleTransform");
	_transformModelUV = glGetUniformLocation(_program, "modelTransform");
	_specModelUV = glGetUniformLocation(_program, "specExp");

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


		readPointsOfModelVert();
		readPointsOfModelFaces();

 		// Obtain attribute handles:
		_posAttribModel = glGetAttribLocation(_program, "positionModel");
		glEnableVertexAttribArray(_posAttribModel);
		glVertexAttribPointer(_posAttribModel, // attribute handle
							  4,          // number of scalars per vertex
							  GL_FLOAT,   // scalar type
							  GL_FALSE,
							  0,
							  0);

	}

}

/**
 * responsible for drawing the 3D model inside mesh
 */
void Model::draw3dModel()
{

	// Set the program to be used in subsequent lines:
	glUseProgram(_program);

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
	glUniform1f(_specModelUV, _spec);
	// choose fill or wire
	GLenum polygonMode = _isFull ? GL_FILL : GL_LINE;
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	if(_normalMode == FACE_NORMAL_MODE)
	{
		glBindVertexArray(_vaoModelFaces);
		glDrawArrays(GL_TRIANGLES, 0, _mesh.n_faces() * 3);
	}
	else
	{
		glBindVertexArray(_vaoModel);
		glDrawElements(GL_TRIANGLES, _mesh.n_faces() * 3, GL_UNSIGNED_INT, NULL);
	}
	glBindVertexArray(0);
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
void Model::readPointsOfModelFaces()
{

	_mesh.request_face_normals();
	_mesh.request_vertex_normals();
	_mesh.update_normals();

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
	size_t vNum = _mesh.n_faces();
	_meshFaces.reserve(vNum * 3 * 2);
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
			}
		}
	}
	// find the size on each axis, and the center
	float center[3], size[3];
	for(int i = 0; i < 3; ++i)
	{
		center[i] = (_upperRight[i] + _lowerLeft[i]) / 2;
		size[i] = (_upperRight[i] - _lowerLeft[i]) / 2;
	}

	for(faceIter = _mesh.faces_begin(); faceIter != _mesh.faces_end(); ++faceIter) {
		faceHandle = faceIter.handle();
		for (faceVertexIter = _mesh.fv_iter(faceHandle); faceVertexIter; ++faceVertexIter) {
			// This is how to get the point associated with the vertex:
			p = _mesh.point(faceVertexIter);
			glm::vec4 cord((p[0] - center[0]) / size[0], (p[1] - center[1]) / size[1], (p[2] - center[2]) / size[2], 1.0f);
			_meshFaces.push_back(cord);

			Mesh::Normal normPoint = _mesh.normal(faceIter);
			glm::vec4 normal(normPoint[0], normPoint[1], normPoint[2], 0.0f);
			_meshFaces.push_back(normal);
		}
	}
	_mesh.release_face_normals();
	_mesh.release_vertex_normals();

	float radius =	glm::sqrt(size[0] * size[0] +  size[1] * size[1] +  size[2] * size[2]);
	// save the ratio
	_scaleMatModel = glm::scale(glm::mat4(), glm::vec3(size[0] / radius, size[1] / radius, size[2] / radius));

	glGenVertexArrays(1, &_vaoModelFaces);
	glBindVertexArray(_vaoModelFaces);

	// Create and load vertex data into a Vertex Buffer Object:
	glGenBuffers(1, &_vboModelFace);
	glBindBuffer(GL_ARRAY_BUFFER, _vboModelFace);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * _meshFaces.size(), _meshFaces.data(), GL_STATIC_DRAW);

	setShading(PHONG_MODE);
	glBindVertexArray(0);

}

void Model::readPointsOfModelVert()
{
	_mesh.request_face_normals();
	_mesh.request_vertex_normals();
	_mesh.update_normals();

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
	_meshVertices.reserve(vNum * 2);
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
			}
		}
	}
	// find the size on each axis, and the center
	float center[3], size[3];
	for(int i = 0; i < 3; ++i)
	{
		center[i] = (_upperRight[i] + _lowerLeft[i]) / 2;
		size[i] = (_upperRight[i] - _lowerLeft[i]) / 2;
	}

	for(vertexIter = _mesh.vertices_begin(); vertexIter != _mesh.vertices_end(); ++vertexIter) {
		// This is how to get the point associated with the vertex:
		p = _mesh.point(vertexIter.handle());
		glm::vec4 cord((p[0] - center[0]) / size[0], (p[1] - center[1]) / size[1], (p[2] - center[2]) / size[2], 1.0f);
		_meshVertices.push_back(cord);

		Mesh::Normal normPoint = _mesh.normal(vertexIter);
		glm::vec4 normal(normPoint[0], normPoint[1], normPoint[2], 0.0);
		_meshVertices.push_back(normal);
	}
	_mesh.release_face_normals();
	_mesh.release_vertex_normals();

	float radius =	glm::sqrt(size[0] * size[0] +  size[1] * size[1] +  size[2] * size[2]);
	// save the ratio
	_scaleMatModel = glm::scale(glm::mat4(), glm::vec3(size[0] / radius, size[1] / radius, size[2] / radius));

	std::vector<face_indices> faces;
	faces.reserve(_mesh.n_faces());
	for(faceIter = _mesh.faces_begin(); faceIter != _mesh.faces_end(); ++faceIter)
	{
		Mesh::ConstFaceVertexIter cfvlt = _mesh.cfv_iter(faceIter.handle());
		face_indices face;
		face.x = cfvlt.handle().idx();
		++cfvlt;
		face.y = cfvlt.handle().idx();
		++cfvlt;
		face.z = cfvlt.handle().idx();
		faces.push_back(face);
	}

	glGenVertexArrays(1, &_vaoModel);
	glBindVertexArray(_vaoModel);

	glGenBuffers(1, &_elementBo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face_indices) * faces.size(), &(faces[0]), GL_STATIC_DRAW);

	// Create and load vertex data into a Vertex Buffer Object:
	glGenBuffers(1, &_vboModel);
	glBindBuffer(GL_ARRAY_BUFFER, _vboModel);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * _meshVertices.size(), _meshVertices.data(), GL_STATIC_DRAW);

	setShading(PHONG_MODE);
	glBindVertexArray(0);

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
//	we saw that on this school solution, we dont change from wire to full or from one prespective to the other
//  when 'r' key is pressed
//	_isFull = true;
//	_isPrespective = true;
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
	_spec = 200;
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

void Model::bindProgram(GLuint program) {
	_transformModelUV = glGetUniformLocation(program, "modelTransform");
	_specModelUV = glGetUniformLocation(program, "specExp");
	_posAttribModel = glGetAttribLocation(program, "positionModel");
	glEnableVertexAttribArray(_posAttrib);
	glVertexAttribPointer(_posAttrib, // attribute handle
						  4,          // number of scalars per vertex
						  GL_FLOAT,   // scalar type
						  GL_FALSE,
						  sizeof(glm::vec4) * 2,
						  0);
	_normalModelUV = glGetAttribLocation(program, "normalModel");
	glEnableVertexAttribArray(_normalModelUV);
	glVertexAttribPointer(_normalModelUV, // attribute handle
						  4,          // number of scalars per vertex
						  GL_FLOAT,   // scalar type
						  GL_FALSE,
						  sizeof(glm::vec4) * 2,
						  (GLvoid*)(sizeof(glm::vec4)));

	// Unbind vertex array:
//	glBindVertexArray(0);
}

void Model::setShading(int type)
{
	_shadingMode = type;
	switch (type)
	{
		case PHONG_MODE:
			_program = _phongProgram;
			break;
		case GOURAUD_MODE:
			_program = _gouraudProgram;
			break;
		case RGB_MODE:
			_program = _rgbProgram;
			break;
		default:
			break;
	}
	glUseProgram(0);
	bindProgram(_program);
	glUseProgram(_program);
}

void Model::setNormal()
{
	_normalMode = (_normalMode == VERT_NORMAL_MODE) ? FACE_NORMAL_MODE : VERT_NORMAL_MODE;
	if(_program == _rgbProgram)
		_program = _gouraudProgram;
	glUseProgram(0);
	bindProgram(_program);
	glUseProgram(_program);
}

void Model::setSpec(char sign)
{
	switch (sign)
	{
		case INCREASE_MODE:
			if(_spec <= 2000)
				_spec+=10;
			if (_spec > 2000)
				_spec = 2000;
			break;
		case DECREASE_MODE:
			if (_spec >= 0)
				_spec-=10;
			if(_spec < 0)
				_spec = 0;
			break;
	}
}