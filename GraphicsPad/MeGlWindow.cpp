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
#include "Light.h"

//move "tex" from textureSetup() to paintGL() -> correct texture ?????????why
//what is the chronological order of functions(paintGL & initialize)

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat4;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 14;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

GLuint programID;
GLuint cubemapProgramID;

GLuint teapotNumIndices;
GLuint teapotNormalsNumIndices;

GLuint cubeNumIndices;
GLuint cubeNormalsNumIndices;

GLuint planeNumIndices;
GLuint planeNormalsNumIndices;

Camera camera;
Light light;

GLuint fullTransformationUniformLocation;

GLuint theBufferID;

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

glm::vec3 lightPositionWorld(0.0f, 10.0f, 0.0f);

const char* MeGlWindow::TexFile[] = { "Texture/right.png","Texture/left.png","Texture/down.png","Texture/up.png","Texture/back.png","Texture/front.png" };

void MeGlWindow::cubeMapSetup()
{
	glActiveTexture(GL_TEXTURE0);
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	const char * suffixes[] = { "posx", "negx", "posy",
	 "negy", "posz", "negz" };
	GLuint targets[] = {
	 GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	 GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	 GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	 GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	 GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	 GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < 6; ++i)
	{
		//string texName = string(baseFileName) +
		//	"_" + suffixes[i] + ".png";
		QImage img = QGLWidget::convertToGLFormat(QImage(TexFile[i], "PNG"));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
			img.width(), img.height(),0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
	}

	// Typical cube map settings
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Set the CubeMapTex uniform to texture unit 0
	int uniloc = glGetUniformLocation(cubemapProgramID, "cubeMapTex");
	if (uniloc >= 0)
		glUniform1i(uniloc, 0);
}
void MeGlWindow::textureSetup()
{
	// Brick and Moss
	GLuint texIDs[3];
	glGenTextures(3, texIDs);
	// Load cat texture file
	const char * texName = "Texture/brick.png";
	QImage brickImg =
		QGLWidget::convertToGLFormat(QImage(texName, "PNG"));
	// Copy cat texture to OpenGL
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texIDs[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, brickImg.width(),
		brickImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		brickImg.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);

	// Set the catTex sampler uniform to texture unit0
	int uniloc = glGetUniformLocation(programID, "brickTex");
	if (uniloc >= 0)
		glUniform1i(uniloc, 0);

	// Load moss texture file
	texName = "Texture/moss2.png";
	QImage mossImg =
		QGLWidget::convertToGLFormat(QImage(texName, "PNG"));

	// Copy moss texture to OpenGL
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texIDs[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mossImg.width(),
		mossImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		mossImg.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);

	// Set the MossTex sampler uniform to texture unit 1
	uniloc = glGetUniformLocation(programID, "mossTex");
	if (uniloc >= 0)
		glUniform1i(uniloc, 1);

	// load normal map file
	texName = "Texture/brickNormal.png";
	QImage normalImg =
		QGLWidget::convertToGLFormat(QImage(texName, "PNG"));

	// Copy normal texture to OpenGL
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texIDs[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, normalImg.width(),
		normalImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		normalImg.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);

	// Set the MossTex sampler uniform to texture unit 2
	uniloc = glGetUniformLocation(programID, "normalMapTex");
	if (uniloc >= 0)
		glUniform1i(uniloc, 2);
}

void MeGlWindow::sendDataToOpenGL()
{
	
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

void MeGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

void  MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	// Camera
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

	
	// Light
	case Qt::Key::Key_2:
		lightPositionWorld += glm::vec3(0.0, 0.0, -0.2);
		break;
	case Qt::Key::Key_0:
		lightPositionWorld += glm::vec3(0.0, 0.0, 0.2);
		break;
	case Qt::Key::Key_Right:
		lightPositionWorld += glm::vec3(0.2, 0.0, 0.0);
		break;
	case Qt::Key::Key_Left:
		lightPositionWorld += glm::vec3(-0.2, 0.0, 0.0);
		break;
	case Qt::Key::Key_Up:
		lightPositionWorld += glm::vec3(0.0, 0.2, 0.0);
		break;
	case Qt::Key::Key_Down:
		lightPositionWorld += glm::vec3(0.0, -0.2, 0.0);
		break;

	/*case Qt::Key::Key_Up:
		light.moveUp();
		break;
	case Qt::Key::Key_Down:
		light.moveDown();
		break;
	case Qt::Key::Key_Left:
		light.strafeLeft();
		break;
	case Qt::Key::Key_Right:
		light.strafeRight();
		break;*/
	}
	repaint();
}


//run everytime you call
void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	
	//Matrix setup
	mat4 modelToProjectionMatrix;
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 200.0f);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	fullTransformationUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix");
	//mat4 modelToViewMatrix = worldToViewMatrix * modelToWorldMatrix;

	glUseProgram(programID); //this must call before texture
	//initializeGL() is before paintGL()

	// Set the catTex sampler uniform to texture unit0
	int uniloc = glGetUniformLocation(programID, "brickTex");
	if (uniloc >= 0)
		glUniform1i(uniloc, 0);
	uniloc = glGetUniformLocation(programID, "mossTex");
	if (uniloc >= 0)
		glUniform1i(uniloc, 1);
	uniloc = glGetUniformLocation(programID, "normalMapTex");
	if (uniloc >= 0)
		glUniform1i(uniloc, 2);

	//// brick tex
	//GLint brickTextureUniformLocation = glGetUniformLocation(programID, "brickTex");
	//glUniform1i(brickTextureUniformLocation, 0);
	////moss tex
	//GLint mossTextureUniformLocation = glGetUniformLocation(programID, "mossTex");
	//glUniform1i(mossTextureUniformLocation, 1);
	//// Normal Setup
	//GLint normalmapUniformLocation = glGetUniformLocation(programID, "normalMapTex");
	//glUniform1i(normalmapUniformLocation, 2);

	GLint modelToWorldMatrixUniformLocation =
		glGetUniformLocation(programID, "modelToWorldMatrix");
	//glUniform1i(modelToWorldMatrixUniformLocation, 1);

	//Ambient Light
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	vec4 ambientLight(0.55f, 0.55f, 0.55f, 1.0f);
	glUniform4fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	//Specular Light
	GLint eyePositionUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");
	glm::vec3 eyePosition = camera.getPosition();
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);

	//Light position
	GLint lightPositionWorldUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	//glm::vec3 lightPositionWorld = light.getPosition();
	glUniform3fv(lightPositionWorldUniformLocation, 1, &lightPositionWorld[0]);

	//Teapot
	glBindVertexArray(teapotVertexArrayObjectID);
	mat4 teapot1ModelToWorldMatrix =
		glm::translate(vec3(0.0f, -1.0f, -1.0f)) *
		glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
	modelToProjectionMatrix = worldToProjectionMatrix * teapot1ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	//glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void*)teapotIndexDataByteOffset);
	/*glBindVertexArray(teapotNormalsVertexArrayObjectID);
	glDrawElements(GL_LINES, teapotNormalsNumIndices, GL_UNSIGNED_SHORT, (void*)teapotNormalsIndexDataByteOffset);*/

	glBindVertexArray(teapotVertexArrayObjectID);
	mat4 teapot2ModelToWorldMatrix =
		glm::translate(vec3(3.0f, 0.0f, -6.75f)) *
		glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
	modelToProjectionMatrix = worldToProjectionMatrix * teapot2ModelToWorldMatrix;
	//glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	//glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void*)teapotIndexDataByteOffset);
	/*glBindVertexArray(teapotNormalsVertexArrayObjectID);
	glDrawElements(GL_LINES, teapotNormalsNumIndices, GL_UNSIGNED_SHORT, (void*)teapotNormalsIndexDataByteOffset);*/



	//Plane
	glBindVertexArray(planeVertexArrayObjectID);
	mat4 planeModelToWorldMatrix = 
	
		glm::rotate(0.0f, 1.0f, 0.0f, 0.0f);

	modelToProjectionMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&planeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexDataByteOffset);
	/*glBindVertexArray(planeNormalsVertexArrayObjectID);
	glDrawElements(GL_LINES, planeNormalsNumIndices, GL_UNSIGNED_SHORT, (void*)planeNormalsIndexDataByteOffset);*/

	////cube translated
	//glBindVertexArray(cubeVertexArrayObjectID);
	//mat4 cubeModelToWorldMatrix = 
	//	glm::translate(0.0f, 2.0f, -3.0f) *
	//	glm::rotate(-70.0f, 1.0f, 0.0f, 0.0f);
	//
	//modelToProjectionMatrix = worldToProjectionMatrix * cubeModelToWorldMatrix;
	//glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	//glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, 
	//	&cubeModelToWorldMatrix[0][0]);
	////glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexDataByteOffset);


	
	
	//CubeMap
	glUseProgram(cubemapProgramID);

	int cubeuniloc = glGetUniformLocation(cubemapProgramID, "cubeMapTex");
	if (cubeuniloc >= 0)
		glUniform1i(cubeuniloc, 0);
	
	glBindVertexArray(cubeVertexArrayObjectID);
	mat4 cubeModelToWorldMatrix =
		glm::scale(5.0f, 5.0f, 5.0f);

	////CubeMap doesn't move with camera
	//worldToViewMatrix[3][0] = 0.0;
	//worldToViewMatrix[3][1] = 0.0;
	//worldToViewMatrix[3][2] = 0.0;
	GLuint skyboxTransformMatrixUniformLocation = glGetUniformLocation(cubemapProgramID, "skyboxTransformMatrix");
	mat4 skyboxTransformMatrix = worldToProjectionMatrix * cubeModelToWorldMatrix;
	//modelToProjectionMatrix = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUniformMatrix4fv(skyboxTransformMatrixUniformLocation, 1, GL_FALSE, &skyboxTransformMatrix[0][0]);
	//glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
	//	&cubeModelToWorldMatrix[0][0]); 
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexDataByteOffset);
	

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

	//texture shader
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

	////check if compile has error
	//if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
	//	return;
	
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	//Linker decideds where the attributes are
	glLinkProgram(programID);

	

	////check if linker has error
	//if (!checkProgramStatus(programID))
	//	return;

	//delete shader
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	//glUseProgram(programID);


	GLuint cubemapVertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint cubemapFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	
	//Cubemap Shader
	temp = readShaderCode("CubeMapVertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(cubemapVertexShaderID, 1, adapter, 0);
	temp = readShaderCode("CubeMapFragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(cubemapFragmentShaderID, 1, adapter, 0);

	glCompileShader(cubemapVertexShaderID);
	glCompileShader(cubemapFragmentShaderID);

	////check if compile has error
	//if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
	//	return;

	cubemapProgramID = glCreateProgram();
	glAttachShader(cubemapProgramID, cubemapVertexShaderID);
	glAttachShader(cubemapProgramID, cubemapFragmentShaderID);
	//Linker decideds where the attributes are
	glLinkProgram(cubemapProgramID);

	

	////check if linker has error
	//if (!checkProgramStatus(cubemapProgramID))
	//	return;

	//delete shader
	glDeleteShader(cubemapVertexShaderID);
	glDeleteShader(cubemapFragmentShaderID);

	//glUseProgram(cubemapProgramID);
	
}

void MeGlWindow::initializeGL()
{
	setMinimumSize(1200, 900);
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

	//textureSetup();
	installShaders();
	textureSetup();
	sendDataToOpenGL();
	cubeMapSetup();

}

MeGlWindow::~MeGlWindow()
{
	glDeleteBuffers(1, &theBufferID);
	glUseProgram(0);
	glDeleteProgram(programID);
}

