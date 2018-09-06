#include <gl\glew.h>
#include <iostream>
#include <fstream>
#include <MeGLWindow.h>
using namespace std;


//extern const char* vertexShaderCode;
//extern const char* fragmentShaderCode;


void sendDataToOpenGL()
{
	const float RED_TRIANGLE_Z = 0.5f;
	const float BLUE_TRIANGLE_Z = -0.5f;

	GLfloat verts[] =
	{
		//Triangle's location
		//and color
		-1.0f, -1.0f, RED_TRIANGLE_Z,
		+1.0f, +0.0f, +0.0f,
		+0.0f, +1.0f, RED_TRIANGLE_Z,
		+1.0f, +0.0f, +0.0f,
		+1.0f, -1.0f, RED_TRIANGLE_Z,
		+1.0f, +0.0f, +0.0f,

		-1.0f, +1.0f, BLUE_TRIANGLE_Z,
		+0.0f, +0.0f, +1.0f,
		+0.0f, -1.0f, BLUE_TRIANGLE_Z,
		+0.0f, +0.0f, +1.0f,
		+1.0f, +1.0f, BLUE_TRIANGLE_Z,
		+0.0f, +0.0f, +1.0f,

	};

	GLuint vertexBufferID;
	//put data on the graphics card
	//create buffer
	glGenBuffers(1, &vertexBufferID);
	//bind the buffer to the binding point 
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	//send the data down to the openGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts),
		verts, GL_STATIC_DRAW);

	//enable the vertex attribute (position)
	glEnableVertexAttribArray(0);
	//describe position data to openGL
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	//enable the vertex attribute (color)
	glEnableVertexAttribArray(1);
	//describe color data to openGL
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	//how the vertices connected
	GLushort indices[] = { 3,4,5, 0,1,2 };
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);

}

//run everytime you call
void MeGLWindow::paintGL()
{
	//make screen red
	//glClearColor(1, 0, 0, 1);

	//sert the depth buffer to 1
	glClear(GL_DEPTH_BUFFER_BIT);


	//render the triangle on the full screen
	glViewport(0, 0, width(), height());
	//draw a triangle using the verts

	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//using dices to draw TWO triangles
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

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

	
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	//check if linker has error
	if (!checkProgramStatus(programID))
		return;

	glUseProgram(programID);
}

void MeGLWindow::initializeGL()
{
	glewInit();
	//enable the buffer
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();

}

