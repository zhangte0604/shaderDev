#include <gl\glew.h>
#include <Qt\qdebug.h>
#include <fstream>
#include "MeGLWindow.h"
#include <glm/glm.hpp>
using glm::vec3;
#include <Vertex.h>
#include <cassert>

#include <Vector3D.h>
using Math::Vector3D;

#include <iostream>
using std::cout;
using std::endl;

using namespace std;


const float X_DELTA = 0.1f;
const float Y_DELTA = 0.2f;
const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint TIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VERTICE * sizeof(float);
const uint FIRST_MAX_X_SHIFT = 2 / X_DELTA - 1;
const uint FIRST_MAX_Y_SHIFT = 2 / Y_DELTA - 1;

const uint SEC_MAX_X_SHIFT = 2 / X_DELTA - 1;
const uint SEC_MAX_Y_SHIFT = 2 / Y_DELTA - 1;

const uint MAX_TRIS = (FIRST_MAX_X_SHIFT+1) * ( FIRST_MAX_Y_SHIFT+1) * (SEC_MAX_X_SHIFT + 1) * (SEC_MAX_Y_SHIFT + 1);

uint numTris = 0;
uint numSecTris = 0;
uint firstXShift = 0;
uint firstYShift = 0;
int secXShift = 0;
int secYShift = 0;

GLuint programID;
GLuint TriVertexBufferID;
GLuint boundaryVertexBufferID;
GLuint boudaryIndexBufferID;

namespace
{
	//tri data
	Vector3D shipVerts[] =
	{
		Vector3D(+0.0f, +0.1f, 0.0f),
		Vector3D(-0.1f, -0.1f, 0.0f),
		Vector3D(+0.1f, -0.1f, 0.0f),

		/*glm::vec3(+0.0f, +0.1f, +0.0f),
		glm::vec3(-0.1f, -0.1f, +0.0f),
		glm::vec3(+0.1f, -0.1f, +0.0f),*/
	};
	
	//set tri pos
	Vector3D shipPosition(0.0f, 0.0f, 0.0f);

	//diamond data
	Vector3D boundaryVerts[] =
	{
		Vector3D(+0.0f, +1.0f, +0.0f),
		Vector3D(-1.0f, +0.0f, +0.0f),
		Vector3D(-1.0f, +0.0f, +0.0f),
		Vector3D(+0.0f, -1.0f, +0.0f),
		Vector3D(+0.0f, -1.0f, +0.0f),
		Vector3D(+1.0f, +0.0f, +0.0f),
		Vector3D(+1.0f, +0.0f, +0.0f),
		Vector3D(+0.0f, +1.0f, +0.0f),
	};

	GLushort boundaryIndices[] = { 0, 1, 1, 2, 2, 3, 3, 0 };

	const unsigned int NUM_SHIP_VERTS = sizeof(shipVerts) / sizeof(*shipVerts);
	const unsigned int NUM_BOUNDARY_VERTS = sizeof(boundaryVerts) / sizeof(*boundaryVerts);
}

void keyboardControl()
{
	//WASD controls first tri
	if (GetKeyState('D') & 0x8000/*check if high-order bit is set (1 << 15)*/)
	{
		// do stuff
		if (firstXShift < FIRST_MAX_X_SHIFT) 
		{
			firstXShift++;
		}

	}
	else if (GetKeyState('A') & 0x8000) 
	{
		if (firstXShift > 0) 
		{
			firstXShift--;
		}
	}
	else if (GetKeyState('W') & 0x8000) 
	{
		if (firstYShift > 0) 
		{
			firstYShift--;
		}
	}
	else if (GetKeyState('S') & 0x8000) 
	{
		if (firstYShift < FIRST_MAX_Y_SHIFT) 
		{
			firstYShift++;
		}
	}

	//Arrow keys control second tri
	if (GetKeyState(VK_RIGHT) & 0x8000/*check if high-order bit is set (1 << 15)*/)
	{
		// do stuff
		if (secXShift * X_DELTA + X_DELTA < 1)
		{
			secXShift++;
		}

	}
	else if (GetKeyState(VK_LEFT) & 0x8000)
	{
		if (secXShift * X_DELTA > -1)
		{
			secXShift--;
			//printf(secXShift);
		}
	}
	else if (GetKeyState(VK_UP) & 0x8000)
	{
		if (secYShift > 0)
		{
			secYShift--;
		}
	}
	else if (GetKeyState(VK_DOWN) & 0x8000)
	{
		if (secYShift < FIRST_MAX_Y_SHIFT)
		{
			secYShift++;
		}
	}
}


void sendTriToOpenGL()
{
	//generate the buffer, give me an inter
	glGenBuffers(1, &TriVertexBufferID);
	//bind my bufferID, bind the buffer to the binding point 
	glBindBuffer(GL_ARRAY_BUFFER, TriVertexBufferID);
	//send the data(vertices info) down to the openGL
	//declare me some memory on graphics card
	glBufferData(GL_ARRAY_BUFFER, sizeof(shipVerts),
		NULL, GL_DYNAMIC_DRAW);
	
	//describe position data to openGL

}

void sendDiamondToOpenGL()
{
	

	glGenBuffers(1, &boundaryVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, boundaryVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boundaryVerts),
		boundaryVerts, GL_STATIC_DRAW);

	/*glGenBuffers(1, &boudaryIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundaryVertexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boundaryIndices),
		boundaryIndices, GL_STATIC_DRAW);*/
}

//run everytime you draw
void MeGLWindow::paintGL()
{
	//set the depth buffer to 1 & clear the previous tri
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//render the triangle on the full screen
	glViewport(0, 0, width(), height());

	//enable the vertex attribute (position)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	vec3 dominatingColor(0.0f, 1.0f, 0.4f);
	GLint dominatingColorUniformLocation = glGetUniformLocation(programID, "dominatingColor");
	

	// draw the tri(using uniform)
	Vector3D translatedVerts[NUM_SHIP_VERTS]; //NUM_VERTS
	for (unsigned int i = 0; i < NUM_SHIP_VERTS; i++) //3 is NUM_VERTS
	{
		translatedVerts[i] = shipVerts[i] + shipPosition;
	}

	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glBindBuffer(GL_ARRAY_BUFFER, TriVertexBufferID);
	
	glBufferSubData(
		GL_ARRAY_BUFFER, 0, 
		sizeof(translatedVerts),
		translatedVerts);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	//draw diamond(using uniform)
	dominatingColor.r = 1;
	dominatingColor.g = 0;
	dominatingColor.b = 0;
	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glBindBuffer(GL_ARRAY_BUFFER, boundaryVertexBufferID);
	glBufferSubData(
		GL_ARRAY_BUFFER, 0,
		sizeof(boundaryVerts),
		boundaryVerts);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_LINES, 0, 8);
	//glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, 0);
}


//check error !!!!!!!not finish
bool checkStatus(
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
		//cout << buffer << endl;
		delete[] buffer;
		return false;
	}
	return true;
}


//check compile error function
bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

//check linker error function
bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

//
string readShaderCode(const char* fileName)
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


void installShaders()
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

	glLinkProgram(programID);

	//check if linker has error
	if (!checkProgramStatus(programID))
		return;

	glUseProgram(programID);
}

//only runs once
void MeGLWindow::initializeGL()
{
	glewInit();
	//enable the buffer
	glEnable(GL_DEPTH_TEST);
	//shapes data
	sendTriToOpenGL();
	sendDiamondToOpenGL();

	installShaders();


	connect(&myTimer, SIGNAL(timeout()),
		this, SLOT(myUpdate()));
	myTimer.start(0);
}



void MeGLWindow::handleBoundaries()
{
	//tri goes back when it bounce the screen boundary
	if (shipPosition.x < -1 || shipPosition.x > 1)
		shipPosition.x *= -1;
	if (shipPosition.y < -1 || shipPosition.y > 1)
		shipPosition.y *= -1;

	//check if the tri is out of screen
	bool anyCollisions = false;
	for (uint i = 0; i < NUM_BOUNDARY_VERTS; i++)
	{
		const Vector3D& first = boundaryVerts[i];
		const Vector3D& second = boundaryVerts[(i + 1) % NUM_BOUNDARY_VERTS];

		Vector3D wall = second - first;
		Vector3D normal = wall.perpCcwXy().normalized();
		Vector3D respectiveShipPosition = shipPosition - first;
		float dotResult = normal.dot(respectiveShipPosition);
		anyCollisions |= (dotResult < 0);

		if (dotResult < 0)
		{

			shipPosition = shipPosition - 2 * shipPosition.dot(normal) * normal;
			cout << "test!" << endl;
		}
	}
	qDebug() << anyCollisions;
}

//int debugCount = 0;
void MeGLWindow::myUpdate()
{
	/*++debugCount;
	if(debugCount % 10000 == 0)
		cout << "frame!" << debugCount << endl;*/

	handleBoundaries();
	Vector3D velocity(0.01f, 0.01f, 0);
	shipPosition = shipPosition + velocity;
	repaint();
}



