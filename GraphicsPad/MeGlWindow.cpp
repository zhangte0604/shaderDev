#include <gl\glew.h>
#include <iostream>
#include <fstream>
#include <QtGui/qmouseevent>
#include <QtGui/qkeyevent>
#include <MeGlWindow.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <Vertex.h>
#include <ShapeGenerator.h>
#include "Camera.h"

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat4;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 11;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

GLuint programID;
GLuint programPlaneTextureID;

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

void MeGlWindow::textureSetup()

{
	// Load texture file
	const char * texName = "Texture/turkey.png";
	QImage timg =
		QGLWidget::convertToGLFormat(QImage(texName, "PNG"));
	
	// Copy file to OpenGL
	glActiveTexture(GL_TEXTURE0);
	GLuint tid;
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg.width(),
		timg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		timg.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);

	//// Set the Tex1 sampler uniform to refer to texture unit 0
	//int loc = glGetUniformLocation(programHandle, "Tex1");
	//if (loc >= 0)
	//	glUniform1i(loc, 0);
	//else
	//	fprintf(stderr, "Uniform variable Tex1 not found!\n");
}

void MeGlWindow::sendDataToOpenGL()
{
	//Teapot + Arrow
	ShapeData teapot = ShapeGenerator::makeTeapot();
	//ShapeData teapotNormals = ShapeGenerator::generateNormals(teapot);
	ShapeData arrow = ShapeGenerator::makeArrow();
	//ShapeData arrowNormals = ShapeGenerator::generateNormals(arrow);
	ShapeData plane = ShapeGenerator::makePlane(20);
	//ShapeData planeNormals = ShapeGenerator::generateNormals(plane);

	
	//put data on the graphics card
	//create buffer
	glGenBuffers(1, &theBufferID);
	//bind the buffer to the binding point 
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	//send the data down to the openGL
	glBufferData(GL_ARRAY_BUFFER, 
		teapot.vertexBufferSize() + teapot.indexBufferSize() +
		arrow.vertexBufferSize() + arrow.indexBufferSize() +
		plane.vertexBufferSize() + plane.indexBufferSize(), 0, GL_STATIC_DRAW);
	
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
	

	teapotNumIndices = teapot.numIndices;
	//teapotNormalsNumIndices = teapotNormals.numIndices;
	arrowNumIndices = arrow.numIndices;
	//arrowNormalsNumIndices = arrowNormals.numIndices;
	planeNumIndices = plane.numIndices;
	//planeNormalsNumIndices = planeNormals.numIndices;

	
	glGenVertexArrays(1, &teapotVertexArrayObjectID);
	glGenVertexArrays(1, &arrowVertexArrayObjectID);
	glGenVertexArrays(1, &planeVertexArrayObjectID);

	/*glGenVertexArrays(1, &teapotNormalsVertexArrayObjectID);
	glGenVertexArrays(1, &arrowNormalsVertexArrayObjectID);
	glGenVertexArrays(1, &planeNormalsVertexArrayObjectID);
	*/

	//teapot
	//where array object is activated
	glBindVertexArray(teapotVertexArrayObjectID);
	//enable the vertex attribute (position)
	glEnableVertexAttribArray(0);
	//enable the vertex attribute (color)
	glEnableVertexAttribArray(1);
	//enable the normal attribute (normal)
	glEnableVertexAttribArray(2);
	//enable the uv attribute (uv)
	glEnableVertexAttribArray(3);
	//rebind teapot vertex bufferID back
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	//describe position data to openGL
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	//describe color data to openGL
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	//describe normal data to openGL
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));
	//describe uv data to openGL
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 9));
	//rebind teapot index bufferID back
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	//arrow
	//where array object is activated
	glBindVertexArray(arrowVertexArrayObjectID);
	//enable the vertex attribute (position)
	glEnableVertexAttribArray(0);
	//enable the vertex attribute (color)
	glEnableVertexAttribArray(1);
	//enable the normal attribute (color)
	glEnableVertexAttribArray(2);
	//enable the uv attribute (uv)
	glEnableVertexAttribArray(3);
	//rebind arrow vertex bufferID back
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint arrowByteOffset = teapot.vertexBufferSize() + teapot.indexBufferSize();
	//describe position data to openGL
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)arrowByteOffset);
	//describe color data to openGL
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowByteOffset + sizeof(float) * 3));
	//describe normal data to openGL
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowByteOffset + sizeof(float) * 6));
	//describe uv data to openGL
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowByteOffset + sizeof(float) * 9));
	//rebind arrow index bufferID back
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	//plane
	//where array object is activated
	glBindVertexArray(planeVertexArrayObjectID);
	//enable the vertex attribute (position)
	glEnableVertexAttribArray(0);
	//enable the vertex attribute (color)
	glEnableVertexAttribArray(1);
	//enable the normal attribute (color)
	glEnableVertexAttribArray(2);
	//enable the uv attribute (uv)
	glEnableVertexAttribArray(3);
	//rebind arrow vertex bufferID back
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint planeByteOffset = arrowByteOffset + arrow.vertexBufferSize() + arrow.indexBufferSize();
	//describe position data to openGL
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)planeByteOffset);
	//describe color data to openGL
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 3));
	//describe normal data to openGL
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 6));
	//describe uv data to openGL
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 9));

	//rebind arrow index bufferID back
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	teapotIndexDataByteOffset = teapot.vertexBufferSize();
	arrowIndexDataByteOffset = arrowByteOffset + arrow.vertexBufferSize();
	planeIndexDataByteOffset = planeByteOffset + plane.vertexBufferSize();


	teapot.cleanup();
	arrow.cleanup();
	plane.cleanup();
}

void MeGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

void  MeGlWindow::keyPressEvent(QKeyEvent* e)
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
void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	mat4 modelToProjectionMatrix;
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	GLint modelToWorldMatrixUniformLocation =
		glGetUniformLocation(programID, "modelToWorldMatrix");
	
	//glUseProgram(programID);

	////Ambient Light
	//GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	//vec4 ambientLight(0.01f, 0.01f, 0.01f, 1.0f);
	//glUniform4fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	////Specular Light
	//GLint eyePositionUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");
	//glm::vec3 eyePosition = camera.getPosition();
	//glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);

	////Light position
	//GLint lightPositionWorldUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	//glm::vec3 lightPositionWorld(3.0f, 5.0f, 0.0f);
	//glUniform3fv(lightPositionWorldUniformLocation, 1, &lightPositionWorld[0]);

	////Teapot
	//glBindVertexArray(teapotVertexArrayObjectID);
	//mat4 teapot1ModelToWorldMatrix =
	//	glm::translate(vec3(0.0f, -1.0f, -1.0f)) *
	//	glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
	//modelToProjectionMatrix = worldToProjectionMatrix * teapot1ModelToWorldMatrix;
	//glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	//glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void*)teapotIndexDataByteOffset);
	///*glBindVertexArray(teapotNormalsVertexArrayObjectID);
	//glDrawElements(GL_LINES, teapotNormalsNumIndices, GL_UNSIGNED_SHORT, (void*)teapotNormalsIndexDataByteOffset);*/

	//glBindVertexArray(teapotVertexArrayObjectID);
	//mat4 teapot2ModelToWorldMatrix =
	//	glm::translate(vec3(3.0f, 0.0f, -6.75f)) *
	//	glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
	//modelToProjectionMatrix = worldToProjectionMatrix * teapot2ModelToWorldMatrix;
	////glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	////glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void*)teapotIndexDataByteOffset);
	///*glBindVertexArray(teapotNormalsVertexArrayObjectID);
	//glDrawElements(GL_LINES, teapotNormalsNumIndices, GL_UNSIGNED_SHORT, (void*)teapotNormalsIndexDataByteOffset);*/


	//

	////Arrow translated
	//glBindVertexArray(arrowVertexArrayObjectID);
	//mat4 arrowModelToWorldMatrix = 
	//	glm::translate(0.0f, 2.0f, -8.0f) *
	//	//glm::rotate(-70.0f, 1.0f, 0.0f, 0glDeleteProgram(programID);.0f);
	//
	//modelToProjectionMatrix = worldToProjectionMatrix * arrowModelToWorldMatrix;
	//glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	//glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, 
	//	&arrowModelToWorldMatrix[0][0]);
	//glDrawElements(GL_TRIANGLES, arrowNumIndices, GL_UNSIGNED_SHORT, (void*)arrowIndexDataByteOffset);
	///*glBindVertexArray(arrowNormalsVertexArrayObjectID);
	//glDrawElements(GL_LINES, arrowNormalsNumIndices, GL_UNSIGNED_SHORT, (void*)arrowNormalsIndexDataByteOffset);*/

	////arrow centered
	//arrowModelToWorldMatrix = mat4();
	//modelToProjectionMatrix = worldToProjectionMatrix * arrowModelToWorldMatrix;
	//glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	//glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
	//	&arrowModelToWorldMatrix[0][0]);
	////glDrawElements(GL_TRIANGLES, arrowNumIndices, GL_UNSIGNED_SHORT, (void*)arrowIndexDataByteOffset);
	//
	//
	
	glUseProgram(programPlaneTextureID);

	//Plane
	glBindVertexArray(arrowVertexArrayObjectID);
	mat4 planeModelToWorldMatrix;
	modelToProjectionMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&planeModelToWorldMatrix[0][0]);
	
	//texture
	GLint TextureUniformLocation = glGetUniformLocation(programPlaneTextureID, "Tex1");
	glUniform1i(TextureUniformLocation, 0);

	glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, 0);
	/*glBindVertexArray(planeNormalsVertexArrayObjectID);
	glDrawElements(GL_LINES, planeNormalsNumIndices, GL_UNSIGNED_SHORT, (void*)planeNormalsIndexDataByteOffset);*/
}


//check error !!!!!!!not finish
bool MeGlWindow::checkStatus(
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
bool MeGlWindow::checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

//check linker error function
bool MeGlWindow::checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

//
string MeGlWindow::readShaderCode(const char* fileName)
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


void MeGlWindow::installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint vertexShaderPlaneID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderPlaneID = glCreateShader(GL_FRAGMENT_SHADER);

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

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	//Linker decideds where the attributes are
	glLinkProgram(programID);

	//glUseProgram(programID);

	//  Plane Texture Shader
	temp = readShaderCode("VertexShaderCodePlane.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderPlaneID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCodePlane.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderPlaneID, 1, adapter, 0);

	glCompileShader(vertexShaderPlaneID);
	glCompileShader(fragmentShaderPlaneID);

	programPlaneTextureID = glCreateProgram();
	glAttachShader(programPlaneTextureID, vertexShaderPlaneID);
	glAttachShader(programPlaneTextureID, fragmentShaderPlaneID);
	//Linker decideds where the attributes are
	glLinkProgram(programPlaneTextureID);


	//check if compile has error
	if (!checkShaderStatus(vertexShaderID) 
		|| !checkShaderStatus(fragmentShaderID) 
		|| !checkShaderStatus(vertexShaderPlaneID) 
		|| !checkShaderStatus(fragmentShaderPlaneID))
		return;

	//check if linker has error
	if (!checkProgramStatus(programID) || !checkProgramStatus(programPlaneTextureID))
		return;

	//delete shader
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteShader(vertexShaderPlaneID);
	glDeleteShader(fragmentShaderPlaneID);

	//glUseProgram(programPlaneTextureID);
}

void MeGlWindow::initializeGL()
{
	setMinimumSize(1200, 500);
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

	textureSetup();

	sendDataToOpenGL();
	installShaders();
	

	fullTransformationUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix");

}

MeGlWindow::~MeGlWindow()
{
	glDeleteBuffers(1, &theBufferID);
	glUseProgram(0);
	glDeleteProgram(programID);
	glDeleteProgram(programPlaneTextureID);
}

