//
//  Model.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__Model__
#define __ex0__Model__

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bits/stl_algo.h>
#include "glm/gtc/matrix_transform.hpp"

#include "OpenMesh/Core/IO/MeshIO.hh"
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"
typedef OpenMesh::PolyMesh_ArrayKernelT<> Mesh;

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#define PHONG_MODE 1
#define GOURAUD_MODE 2
#define RGB_MODE 3
#define FACE_NORMAL_MODE 1
#define VERT_NORMAL_MODE 2
#define INCREASE_MODE '+'
#define DECREASE_MODE '-'
#define TEXTURE_NORMAL 4
#define TEXTURE_MARBEL 0
#define TEXTURE_WOOD 1
#define TEXTURE_MIRROR 2
#define TEXTURE_BRICK 3


class Model {
	
	GLuint _vaoCircle, _vboCircle, _vaoModel, _vboModel, _vaoModelFaces, _vboModelFace, _elementBo, _posAttrib, _posAttribModel,
			_fillColorUV, _transformModelUV, _transformCircleUV, _specModelUV, _normalModelUV, _textureScaleUV, _turbMagnUV, _textureModeUV;
	GLuint _program, _rgbProgram, _gouraudProgram, _phongProgram;
	float _width, _height, _radius;
	size_t _numPointsCircle, _numOfPointsModel;
	bool _isFull, _isPrespective, _isOutOfCircle;
	Mesh _mesh;
	std::vector<glm::vec4> _meshVertices, _meshFaces;
	glm::mat4 _scaleMatModel, _screenRatio, _rotationMat, _rotateOpMat;
	Mesh::Point _lowerLeft, _upperRight;
	float _zoomYStart, _zoomYEnd, _tempZoom;
	float _moveXStart, _moveYStart, _moveXEnd, _moveYEnd, _moveXTemp, _moveYTemp;
	float _rotateXStart, _rotateYStart;
	int _shadingMode, _normalMode, _textureMode;
	float _spec, _textureScale, _turbMag;
	const char * _sphereFile;
	const char * _brickFile;
	const char * _brickBumpFile;
	GLuint _sphereText, _brickText, _brickBumpText;


public:
	Model(int height, int width);
	virtual ~Model();
	void init(char* filePath);
	void draw3dModel();
	void draw2dCircle();
	void resize(int width, int height);
	void readPointsOfModelFaces();
	void readPointsOfModelVert();
	void changeWire();
	void changePerspective();
	void rotate(int x, int y);
	void reset();
	void startRotate(int x, int y);
	void stopRotate(int x, int y);
	void startMove(int x, int y);
	void stopMove(int x, int y);
	void startZoom(int y);
	void stopZoom(int y);
	void move(int x, int y);
	void zoom(int y);
	glm::vec3 getArcBallVec(float x, float y);
	void setShading(int type);
	void setNormal();
	void bindProgram(GLuint program);
	void setSpec(char sign);
	void switchTexture();
	void decreaseScaleTexture();
	void increaseScaleTexture();
	void decreaseTurb();
	void increaseTurb();
	void loadTexture(const char *fileName, GLuint *texture);
};

#endif /* defined(__ex0__Model__) */
