#include <gl\glew.h>
#include <fstream>
#include "MeGLWindow.h"
#include <glm/glm.hpp>
using glm::vec3;
#include <Vertex.h>
#include <cassert>

#include <Vector2D.h>
using Math::Vector2D;

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

GLuint TriVertexBufferID;
GLuint boundaryVertexBufferID;
GLuint programID;

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

void sendTriAndDiamondToOpenGL()
{
	
}




void sendTriToOpenGL()
{

	Vertex thisTri[] =
	{
		glm::vec3(+0.0f, +0.1f, +0.0f),
		glm::vec3(-0.1f, -0.1f, +0.0f),
		glm::vec3(+0.1f, -0.1f, +0.0f),
	};

	//generate the buffer, give me an inter
	glGenBuffers(1, &TriVertexBufferID);
	//bind my bufferID, bind the buffer to the binding point 
	glBindBuffer(GL_ARRAY_BUFFER, TriVertexBufferID);
	//send the data(vertices info) down to the openGL
	//declare me some memory on graphics card
	glBufferData(GL_ARRAY_BUFFER, sizeof(thisTri),
		thisTri, GL_STATIC_DRAW);
	
	//describe position data to openGL

}

void sendDiamondToOpenGL()
{
	Vertex boundaryVerts[] =
	{
		glm::vec3(+0.0f, +1.0f, +0.0f),
		glm::vec3(-1.0f, +0.0f, +0.0f),
		glm::vec3(-1.0f, +0.0f, +0.0f),
		glm::vec3(+0.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, -1.0f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),
		glm::vec3(+0.0f, +1.0f, +0.0f),
	};

	glGenBuffers(1, &boundaryVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, boundaryVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boundaryVerts),
		boundaryVerts, GL_STATIC_DRAW);
}

//run everytime you draw
void MeGLWindow::paintGL()
{
	//set the depth buffer to 1 & clear the previous tri
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//render the triangle on the full screen
	glViewport(0, 0, width(), height());

	vec3 dominatingColor(0.0f, 1.0f, 0.4f);
	GLint dominatingColorUniformLocation = glGetUniformLocation(programID, "dominatingColor");
	

	// draw the tri
	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	glBindBuffer(GL_ARRAY_BUFFER, TriVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	//draw diamond
	dominatingColor.r = 1;
	dominatingColor.g = 0;
	dominatingColor.b = 0;
	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	glBindBuffer(GL_ARRAY_BUFFER, boundaryVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, 8);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
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
		cout << buffer << endl;
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
	sendDiamondToOpenGL();
	sendTriToOpenGL();
	installShaders();
	//enable the vertex attribute (position)
	glEnableVertexAttribArray(0);

	connect(&myTimer, SIGNAL(timeout()),
		this, SLOT(myUpdate()));
	myTimer.start(0);
}

//int debugCount = 0;

void MeGLWindow::myUpdate()
{
	/*++debugCount;
	if(debugCount % 10000 == 0)
		cout << "frame!" << debugCount << endl;*/
}

