#include <gl\glew.h>
#include <iostream>
#include <fstream>
#include <QtGui/qmouseevent>
#include <QtGui/qkeyevent>
#include <MeGLWindow.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <Vertex.h>
#include <ShapeGenerator.h>
#include "Camera.h"

using namespace std;
using glm::vec3;
using glm::mat4;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 9;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

GLuint programID;

GLuint teapotNumIndices;
GLuint teapotNormalsNumIndices;

GLuint arrowNumIndices;
GLuint arrowNormalsNumIndices;

GLuint planeNumIndices;
GLuint planeNormalsNumIndices;

Camera camera;
GLuint fullTransformationUniformLocation;

GLuint theBufferID;

GLuint teapotVertexArrayObjectID;
GLuint arrowVertexArrayObjectID;
GLuint planeVertexArrayObjectID;

GLuint arrowIndexDataByteOffset;
GLuint teapotIndexDataByteOffset;
GLuint planeIndexDataByteOffset;

GLuint teapotNormalsVertexArrayObjectID;
GLuint arrowNormalsVertexArrayObjectID;
GLuint planeNormalsVertexArrayObjectID;
GLuint teapotNormalsIndexDataByteOffset;
GLuint arrowNormalsIndexDataByteOffset;
GLuint planeNormalsIndexDataByteOffset;

void MeGLWindow::sendDataToOpenGL()
{
	//Teapot + Arrow
	ShapeData teapot = ShapeGenerator::makeTeapot();
	ShapeData teapotNormals = ShapeGenerator::generateNormals(teapot);
	ShapeData arrow = ShapeGenerator::makeArrow();
	ShapeData arrowNormals = ShapeGenerator::generateNormals(arrow);
	ShapeData plane = ShapeGenerator::makePlane(20);
	ShapeData planeNormals = ShapeGenerator::generateNormals(plane);

	
	//put data on the graphics card
	//create buffer
	glGenBuffers(1, &theBufferID);
	//bind the buffer to the binding point 
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	//send the data down to the openGL
	glBufferData(GL_ARRAY_BUFFER, 
		teapot.vertexBufferSize() + teapot.indexBufferSize() +
		arrow.vertexBufferSize() + arrow.indexBufferSize() +
		plane.vertexBufferSize() + plane.indexBufferSize() +
		teapotNormals.vertexBufferSize() + teapotNormals.indexBufferSize() +
		arrowNormals.vertexBufferSize() + arrowNormals.indexBufferSize() +
		planeNormals.vertexBufferSize() + planeNormals.indexBufferSize(), 0, GL_STATIC_DRAW);
	
	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.vertexBufferSize(), teapot.vertices);
	currentOffset += teapot.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.indexBufferSize(), teapot.indices);
	currentOffset += teapot.indexBufferSize();
	
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.vertexBufferSize(), arrow.vertices);
	currentOffset += arrow.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.indexBufferSize(), arrow.indices);
	currentOffset += arrow.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);
	currentOffset += plane.indexBufferSize();
	
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapotNormals.vertexBufferSize(), teapotNormals.vertices);
	currentOffset += teapotNormals.vertexBufferSize();
	teapotNormalsIndexDataByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapotNormals.indexBufferSize(), teapotNormals.indices);
	currentOffset += teapotNormals.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrowNormals.vertexBufferSize(), arrowNormals.vertices);
	currentOffset += arrowNormals.vertexBufferSize();
	arrowNormalsIndexDataByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrowNormals.indexBufferSize(), arrowNormals.indices);
	currentOffset += arrowNormals.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, planeNormals.vertexBufferSize(), planeNormals.vertices);
	currentOffset += planeNormals.vertexBufferSize();
	planeNormalsIndexDataByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, planeNormals.indexBufferSize(), planeNormals.indices);
	currentOffset += planeNormals.indexBufferSize();
	

	teapotNumIndices = teapot.numIndices;
	teapotNormalsNumIndices = teapotNormals.numIndices;
	arrowNumIndices = arrow.numIndices;
	arrowNormalsNumIndices = arrowNormals.numIndices;
	planeNumIndices = plane.numIndices;
	planeNormalsNumIndices = planeNormals.numIndices;

	/*
	//instancing

	GLuint transformationMatrixBufferID;
	glGenBuffers(1, &transformationMatrixBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, transformationMatrixBufferID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * 2, 0, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 0));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 4));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 8));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 12));
	//enable the vertex attribute (TransformMatrix)
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	*/
	
	glGenVertexArrays(1, &teapotVertexArrayObjectID);
	glGenVertexArrays(1, &arrowVertexArrayObjectID);
	glGenVertexArrays(1, &planeVertexArrayObjectID);

	glGenVertexArrays(1, &teapotNormalsVertexArrayObjectID);
	glGenVertexArrays(1, &arrowNormalsVertexArrayObjectID);
	glGenVertexArrays(1, &planeNormalsVertexArrayObjectID);
	
	//teapot
	//where array object is activated
	glBindVertexArray(teapotVertexArrayObjectID);
	//enable the vertex attribute (position)
	glEnableVertexAttribArray(0);
	//enable the vertex attribute (color)
	glEnableVertexAttribArray(1);
	//rebind teapot vertex bufferID back
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	//describe position data to openGL
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	//describe color data to openGL
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	//rebind teapot index bufferID back
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	//arrow
	//where array object is activated
	glBindVertexArray(arrowVertexArrayObjectID);
	//enable the vertex attribute (position)
	glEnableVertexAttribArray(0);
	//enable the vertex attribute (color)
	glEnableVertexAttribArray(1);
	//rebind arrow vertex bufferID back
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint arrowByteOffset = teapot.vertexBufferSize() + teapot.indexBufferSize();
	//describe position data to openGL
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)arrowByteOffset);
	//describe color data to openGL
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowByteOffset + sizeof(float) * 3));
	//rebind arrow index bufferID back
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	//plane
	//where array object is activated
	glBindVertexArray(planeVertexArrayObjectID);
	//enable the vertex attribute (position)
	glEnableVertexAttribArray(0);
	//enable the vertex attribute (color)
	glEnableVertexAttribArray(1);
	//rebind arrow vertex bufferID back
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint planeByteOffset = arrowByteOffset + arrow.vertexBufferSize() + arrow.indexBufferSize();
	//describe position data to openGL
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)planeByteOffset);
	//describe color data to openGL
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 3));
	//rebind arrow index bufferID back
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	teapotIndexDataByteOffset = teapot.vertexBufferSize();
	arrowIndexDataByteOffset = arrowByteOffset + arrow.vertexBufferSize();
	planeIndexDataByteOffset = planeByteOffset + plane.vertexBufferSize();

	glBindVertexArray(teapotNormalsVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint teapotNormalsByteOffset = planeByteOffset + plane.vertexBufferSize() + plane.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)teapotNormalsByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(teapotNormalsByteOffset + sizeof(float) * 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(arrowNormalsVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint arrowNormalsByteOffset = teapotNormalsByteOffset + teapotNormals.vertexBufferSize() + teapotNormals.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)arrowNormalsByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowNormalsByteOffset + sizeof(float) * 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(planeNormalsVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint planeNormalsByteOffset = arrowNormalsByteOffset + arrowNormals.vertexBufferSize() + arrowNormals.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)planeNormalsByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeNormalsByteOffset + sizeof(float) * 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	teapot.cleanup();
	arrow.cleanup();
	plane.cleanup();
}

void MeGLWindow::mouseMoveEvent(QMouseEvent* e)
{
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

void  MeGLWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		camera.moveForward();
		break;
	case Qt::Key::Key_S:
		camera.moveBackward();
		break;
	case Qt::Key::Key_A:
		camera.strafeLeft();
		break;
	case Qt::Key::Key_D:
		camera.strafeRight();
		break;
	case Qt::Key::Key_R:
		camera.moveUp();
		break;
	case Qt::Key::Key_F:
		camera.moveDown();
		break;
	}
	repaint();
}


//run everytime you call
void MeGLWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	mat4 fullTransformMatrix;
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	//Ambient Light
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	vec3 ambientLight(0.5f, 0.5f, 0.5f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	//Teapot
	glBindVertexArray(teapotVertexArrayObjectID);
	mat4 teapot1ModelToWorldMatrix =
		glm::translate(vec3(-3.0f, 0.0f, -6.0f)) *
		glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * teapot1ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void*)teapotIndexDataByteOffset);
	glBindVertexArray(teapotNormalsVertexArrayObjectID);
	glDrawElements(GL_LINES, teapotNormalsNumIndices, GL_UNSIGNED_SHORT, (void*)teapotNormalsIndexDataByteOffset);

	glBindVertexArray(teapotVertexArrayObjectID);
	mat4 teapot2ModelToWorldMatrix =
		glm::translate(vec3(3.0f, 0.0f, -6.75f)) *
		glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * teapot2ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void*)teapotIndexDataByteOffset);
	glBindVertexArray(teapotNormalsVertexArrayObjectID);
	glDrawElements(GL_LINES, teapotNormalsNumIndices, GL_UNSIGNED_SHORT, (void*)teapotNormalsIndexDataByteOffset);

	//Arrow
	glBindVertexArray(arrowVertexArrayObjectID);
	mat4 arrowModelToWorldMatrix = glm::translate(0.0f, 0.0f, -0.3f);
	fullTransformMatrix = worldToProjectionMatrix * arrowModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, arrowNumIndices, GL_UNSIGNED_SHORT, (void*)arrowIndexDataByteOffset);
	glBindVertexArray(arrowNormalsVertexArrayObjectID);
	glDrawElements(GL_LINES, arrowNormalsNumIndices, GL_UNSIGNED_SHORT, (void*)arrowNormalsIndexDataByteOffset);


	//Plane
	glBindVertexArray(planeVertexArrayObjectID);
	mat4 planeModelToWorldMatrix = glm::mat4();
	fullTransformMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexDataByteOffset);
	glBindVertexArray(planeNormalsVertexArrayObjectID);
	glDrawElements(GL_LINES, planeNormalsNumIndices, GL_UNSIGNED_SHORT, (void*)planeNormalsIndexDataByteOffset);
}


//check error !!!!!!!not finish
bool MeGLWindow::checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;
		delete[] buffer;
		return false;
	}
	return true;
}


//check compile error function
bool MeGLWindow::checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

//check linker error function
bool MeGLWindow::checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

//
string MeGLWindow::readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}


void MeGLWindow::installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//define array of character pointers
	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");	
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	//check if compile has error
	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	
	
	//Linker decideds where the attributes are
	glLinkProgram(programID);

	//check if linker has error
	if (!checkProgramStatus(programID))
		return;

	/*
	//Where are these attributes automatically
	GLint positionLocation = glGetAttribLocation(programID, "position");
	GLint colorLocation = glGetAttribLocation(programID, "vertexColor");
	GLint transformLocation = glGetAttribLocation(programID, "fullTransformMatrix");
	*/
	

	//delete shader
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void MeGLWindow::initializeGL()
{
	setMouseTracking(true);
	glewInit();
	//enable the buffer
	glEnable(GL_DEPTH_TEST);
	//disable rendering tris not facing the cam
	glEnable(GL_CULL_FACE);

	//Face Culling: choose which face you wanna cull
	//glCullFace(GL_FRONT); //default is GL_BACK

	//Winding Order: make front face to be clockwise rather than counter clockwise
	//glFrontFace(GL_CW); //default is GL_CCW

	sendDataToOpenGL();
	installShaders();

	fullTransformationUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");

}

MeGLWindow::~MeGLWindow()
{
	glDeleteBuffers(1, &theBufferID);
	glUseProgram(0);
	glDeleteProgram(programID);
}

