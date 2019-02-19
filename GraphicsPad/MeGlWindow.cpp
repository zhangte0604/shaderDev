#include <gl\glew.h>
#include <iostream>
#include <fstream>
#include <QtGui/qmouseevent>
#include <QtGui/qkeyevent>
#include <Qt\qapplication.h>
#include <QtGui\qvboxlayout>
#include <QtGui\qhboxlayout>
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
const uint NUM_FLOATS_PER_VERTICE = 14;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

GLuint programID;
GLuint planeProgramID;

GLuint teapotNumIndices;
GLuint teapotNormalsNumIndices;

GLuint cubeNumIndices;
GLuint cubeNormalsNumIndices;

GLuint planeNumIndices;
GLuint planeNormalsNumIndices;

Camera camera;
Camera renderCamera;
GLuint fullTransformationUniformLocation;

GLuint theBufferID;
GLuint framebuffer;
GLuint framebufferTexture;

GLuint teapotVertexArrayObjectID;
GLuint cubeVertexArrayObjectID;
GLuint planeVertexArrayObjectID;

GLuint cubeIndexDataByteOffset;
GLuint teapotIndexDataByteOffset;
GLuint planeIndexDataByteOffset;

GLuint teapotNormalsVertexArrayObjectID;
GLuint cubeNormalsVertexArrayObjectID;
GLuint planeNormalsVertexArrayObjectID;
GLuint teapotNormalsIndexDataByteOffset;
GLuint cubeNormalsIndexDataByteOffset;
GLuint planeNormalsIndexDataByteOffset;

void MeGlWindow::setupFrameBuffer()
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// The texture we are going to render to
	glGenTextures(1, &framebufferTexture);
	glActiveTexture(GL_TEXTURE2); // Use texture unit 2	
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	// Give an empty image to opengl
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0); //bind back to default
	
	// Bind the texture to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);
	
	// Create the depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width(), height());
	// Bind the depth buffer to the FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);


	// Set the target for the fragment shader outputs
	GLenum drawBufs[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBufs);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	// Unbind the framebuffer, and revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glActiveTexture(GL_TEXTURE0); // Bind back to default slot
}

QImage MeGlWindow::loadTexture(const char * texName)
{
	return QGLWidget::convertToGLFormat(QImage(texName, "PNG"));
}

void MeGlWindow::textureSetup()

{
	// Load texture file
	
	//diffuse map
	const char * diffuseName = "Texture/brick.png";
	QImage timg = loadTexture(diffuseName);

	// Copy file to OpenGL
	glActiveTexture(GL_TEXTURE0);
	GLuint diffuseID;
	glGenTextures(1, &diffuseID);
	glBindTexture(GL_TEXTURE_2D, diffuseID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg.width(),
		timg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		timg.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);



	//specular map
	const char * specularName = "Texture/brick-specular.png";
	QImage timg1 = loadTexture(specularName);

	// Copy file to OpenGL
	glActiveTexture(GL_TEXTURE1);
	GLuint specularID;
	glGenTextures(1, &specularID);
	glBindTexture(GL_TEXTURE_2D, specularID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg1.width(),
		timg1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		timg1.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
}

void MeGlWindow::sendDataToOpenGL()
{
	textureSetup();

	//Teapot + cube
	ShapeData teapot = ShapeGenerator::makeTeapot();
	//ShapeData teapotNormals = ShapeGenerator::generateNormals(teapot);
	ShapeData cube = ShapeGenerator::makeCube();
	//ShapeData cubeNormals = ShapeGenerator::generateNormals(cube);
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
		cube.vertexBufferSize() + cube.indexBufferSize() +
		plane.vertexBufferSize() + plane.indexBufferSize(), 0, GL_STATIC_DRAW);
	
	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.vertexBufferSize(), teapot.vertices);
	currentOffset += teapot.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.indexBufferSize(), teapot.indices);
	currentOffset += teapot.indexBufferSize();
	
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);
	currentOffset += plane.indexBufferSize();
	

	teapotNumIndices = teapot.numIndices;
	//teapotNormalsNumIndices = teapotNormals.numIndices;
	cubeNumIndices = cube.numIndices;
	//cubeNormalsNumIndices = cubeNormals.numIndices;
	planeNumIndices = plane.numIndices;
	//planeNormalsNumIndices = planeNormals.numIndices;

	
	glGenVertexArrays(1, &teapotVertexArrayObjectID);
	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	glGenVertexArrays(1, &planeVertexArrayObjectID);


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
	//enable the tangent attribute (tangent)
	glEnableVertexAttribArray(4);
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
	//describe tangent data to openGL
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 11));
	//rebind teapot index bufferID back
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	//cube
	//where array object is activated
	glBindVertexArray(cubeVertexArrayObjectID);
	//enable the vertex attribute (position)
	glEnableVertexAttribArray(0);
	//enable the vertex attribute (color)
	glEnableVertexAttribArray(1);
	//enable the normal attribute (color)
	glEnableVertexAttribArray(2);
	//enable the uv attribute (uv)
	glEnableVertexAttribArray(3);
	//enable the tangent attribute (tangent)
	glEnableVertexAttribArray(4);
	//rebind cube vertex bufferID back
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint cubeByteOffset = teapot.vertexBufferSize() + teapot.indexBufferSize();
	//describe position data to openGL
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)cubeByteOffset);
	//describe color data to openGL
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(cubeByteOffset + sizeof(float) * 3));
	//describe normal data to openGL
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(cubeByteOffset + sizeof(float) * 6));
	//describe uv data to openGL
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(cubeByteOffset + sizeof(float) * 9));
	//describe uv data to openGL
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(cubeByteOffset + sizeof(float) * 11));
	//rebind cube index bufferID back
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	//plane
	//where array object is activated
	glBindVertexArray(planeVertexArrayObjectID);
	//enable the vertex attribute (position)
	glEnableVertexAttribArray(0);
	//enable the vertex attribute (color)
	glEnableVertexAttribArray(1);
	//enable the normal attribute (normal)
	glEnableVertexAttribArray(2);
	//enable the uv attribute (uv)
	glEnableVertexAttribArray(3);
	//enable the tangent attribute (tangent)
	glEnableVertexAttribArray(4);
	
	//rebind cube vertex bufferID back
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint planeByteOffset = cubeByteOffset + cube.vertexBufferSize() + cube.indexBufferSize();
	//describe position data to openGL
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)planeByteOffset);
	//describe color data to openGL
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 3));
	//describe normal data to openGL
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 6));
	//describe uv data to openGL
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 9));
	//describe uv data to openGL
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 11));

	//rebind cube index bufferID back
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	teapotIndexDataByteOffset = teapot.vertexBufferSize();
	cubeIndexDataByteOffset = cubeByteOffset + cube.vertexBufferSize();
	planeIndexDataByteOffset = planeByteOffset + plane.vertexBufferSize();

	

	teapot.cleanup();
	cube.cleanup();
	plane.cleanup();
}

//Alt: control teapot
void MeGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	if (IsAltPressing == true)
	{
		camera.mouseUpdate(glm::vec2(e->x(), e->y()));
		repaint();
	}
	else if (IsAltPressing == false)
	{
		renderCamera.mouseUpdate(glm::vec2(e->x(), e->y()));
		repaint();
	}
	
}

void  MeGlWindow::keyPressEvent(QKeyEvent* event)
{
	
	switch (event->key())
	{

		case Qt::Key::Key_W:
			if (IsAltPressing)
			{
				camera.moveForward();
			}
			else
			{
				renderCamera.moveForward();
			}
			
			break;

		case Qt::Key::Key_S:
			if (IsAltPressing)
			{
				camera.moveBackward();
			}
			else
			{
				renderCamera.moveBackward();
			}
			
			break;
		case Qt::Key::Key_A:
			if (IsAltPressing)
			{
				camera.strafeLeft();
			}
			else
			{
				renderCamera.strafeLeft();
			}
	
			break;
		case Qt::Key::Key_D:
			if (IsAltPressing)
			{
				camera.strafeRight();
			}
			else
			{
				renderCamera.strafeRight();
			}
			
			break;
		case Qt::Key::Key_R:
			if (IsAltPressing)
			{
				camera.moveUp();
			}
			else
			{
				renderCamera.moveUp();
			}
			
			break;
		case Qt::Key::Key_F:
			if (IsAltPressing)
			{
				camera.moveDown();
			}
			else
			{
				renderCamera.moveDown();
			}
			break;
		case Qt::Key::Key_Escape:
			printf("esc");
			QCoreApplication::quit();
			break;
	
		case Qt::Key::Key_Alt:
			printf("Alt Press");
			IsAltPressing = true;
			break;
	}
	repaint();
}

void  MeGlWindow::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Alt)
	{
		printf("Alt Release");
		IsAltPressing = false;
	}
	repaint();
}


//run everytime you call
void MeGlWindow::paintGL()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.5f, 0.0f, 0.0f, 1.0f); //color of plane
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	glEnable(GL_DEPTH_TEST);

	//Teapot
	glUseProgram(programID);

	mat4 modelToProjectionMatrix;
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	//Ambient Light
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	vec4 ambientLight(0.01f, 0.01f, 0.01f, 1.0f);
	glUniform4fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	//Specular Light
	GLint eyePositionUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");
	glm::vec3 eyePosition = camera.getPosition();
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);

	//Light position
	GLint lightPositionWorldUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	glm::vec3 lightPositionWorld(0.0f, 5.0f, 0.0f);
	glUniform3fv(lightPositionWorldUniformLocation, 1, &lightPositionWorld[0]);

	glBindVertexArray(teapotVertexArrayObjectID);
	mat4 teapot1ModelToWorldMatrix =
		glm::translate(vec3(0.0f, -1.0f, 0.0f)) *
		glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));

	fullTransformationUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix");
	GLint modelToWorldMatrixUniformLocation = glGetUniformLocation(programID, "modelToWorldMatrix");
	modelToProjectionMatrix = worldToProjectionMatrix * teapot1ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &teapot1ModelToWorldMatrix[0][0]);


	GLuint diffuseMapUniformLocation = glGetUniformLocation(programID, "Tex0");
	glUniform1i(diffuseMapUniformLocation, 0);
	GLuint speculareMapUniformLocation = glGetUniformLocation(programID, "Tex1");
	glUniform1i(speculareMapUniformLocation, 1);


	glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void*)teapotIndexDataByteOffset);
	

	//Now use a simple plane to display the texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width(), height());
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // the color of viewport
	glClear(GL_COLOR_BUFFER_BIT);
	

	//Plane
	glUseProgram(planeProgramID);

	worldToViewMatrix = renderCamera.getWorldToViewMatrix();
	worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	glBindVertexArray(planeVertexArrayObjectID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);

	mat4 planeModelToWorldMatrix = 
		glm::translate(0.0f, -3.0f, 0.0f) *
		glm::rotate(0.0f, 1.0f, 0.0f, 0.0f) *
		glm::scale(0.5f, 0.5f, 0.5f);

	GLuint framebufferTextureUniformLoc = glGetUniformLocation(planeProgramID, "frameBufferTexture");
	glUniform1i(framebufferTextureUniformLoc, 2);

	fullTransformationUniformLocation = glGetUniformLocation(planeProgramID, "modelToProjectionMatrix");
	modelToWorldMatrixUniformLocation = glGetUniformLocation(planeProgramID, "modelToWorldMatrix");
	modelToProjectionMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
	
	glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexDataByteOffset);
	
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
	
	// Teapot shader
	//define array of character pointers
	const GLchar* adapter[1];
	string temp = readShaderCode("TextureVertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("TextureFragmentShaderCode.glsl");	
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
	GLint transformLocation = glGetAttribLocation(programID, "modelToProjectionMatrix");
	*/

	//delete shader
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	
	// plane shader
	temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	////check if compile has error
	//if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
	//	return;

	planeProgramID = glCreateProgram();
	glAttachShader(planeProgramID, vertexShaderID);
	glAttachShader(planeProgramID, fragmentShaderID);
	//Linker decideds where the attributes are
	glLinkProgram(planeProgramID);

	////check if linker has error
	//if (!checkProgramStatus(cubemapProgramID))
	//	return;

	//delete shader
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void MeGlWindow::initializeGL()
{
	setMinimumSize(600, 600);
	setMouseTracking(true);
	glewInit();
	//enable the buffer
	glEnable(GL_DEPTH_TEST);
	//disable rendering tris not facing the cam
	//glEnable(GL_CULL_FACE);

	//Face Culling: choose which face you wanna cull
	//glCullFace(GL_FRONT); //default is GL_BACK

	//Winding Order: make front face to be clockwise rather than counter clockwise
	//glFrontFace(GL_CW); //default is GL_CCW

	//textureSetup();
	sendDataToOpenGL();
	setupFrameBuffer();
	installShaders();

	

}

MeGlWindow::~MeGlWindow()
{
	glDeleteBuffers(1, &theBufferID);
	glUseProgram(0);
	glDeleteProgram(programID);
}

