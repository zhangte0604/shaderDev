#include <gl\glew.h>
#include <iostream>
#include <fstream>
#include <MeGLWindow.h>
#include <glm/glm.hpp>
#include <Vertex.h>
#include <QtGui/QKeyEvent>

using namespace std;
using glm::vec3;

GLuint programID;
GLfloat yPositionShape01 = 0.0;
GLfloat xPositionShape01 = 1.0;
GLfloat yPositionShape02 = -1.0;
GLfloat xPositionShape02 = 0.0;

//extern const char* vertexShaderCode;
//extern const char* fragmentShaderCode;


void sendDataToOpenGL()
{
	Vertex thisTri[] =
	{
		//first tri
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, 0.5f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-0.5, 0.5f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

	};

	GLuint vertexBufferID;
	//put data on the graphics card
	//create buffer
	glGenBuffers(1, &vertexBufferID);
	//bind the buffer to the binding point 
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	//send the data down to the openGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(thisTri), thisTri, GL_STATIC_DRAW);

	//enable the vertex attribute (position)
	glEnableVertexAttribArray(0);
	//describe position data to openGL
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	//enable the vertex attribute (color)
	glEnableVertexAttribArray(1);
	//describe color data to openGL
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

}

//run everytime you call
void MeGLWindow::paintGL()
{
	//make screen red
	//glClearColor(1, 0, 0, 1);

	//set the depth buffer to 1 & clear the previous tri
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//render the triangle on the full screen
	glViewport(0, 0, width(), height());

	vec3 TriColor(1.0f, 1.0f, 0.0f);

	GLint xMoveUniformLocation = glGetUniformLocation(programID, "xMove");
	GLint yMoveUniformLocation = glGetUniformLocation(programID, "yMove");
	GLint dominatingColorUniformLocation = glGetUniformLocation(programID, "dominatingColor");

	glUniform3fv(dominatingColorUniformLocation, 1, &TriColor[0]);
	glUniform1f(xMoveUniformLocation, xPositionShape01);
	glUniform1f(yMoveUniformLocation, yPositionShape01);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUniform3fv(dominatingColorUniformLocation, 1, &TriColor[1]);
	glUniform1f(xMoveUniformLocation, xPositionShape02);
	glUniform1f(yMoveUniformLocation, yPositionShape02);
	glDrawArrays(GL_TRIANGLES, 0, 3);

}

void MeGLWindow::keyPressEvent(QKeyEvent * e)
{
	switch (e->key())
	{
		//WASD 
	case Qt::Key::Key_W:
		printf("pressed W\n");
		yPositionShape02 += 0.1f;
		break;
	case Qt::Key::Key_S:
		printf("pressed S\n");
		yPositionShape02 -= 0.1f;
		break;
	case Qt::Key::Key_A:
		printf("pressed A\n");
		xPositionShape02 -= 0.1f;
		break;
	case Qt::Key::Key_D:
		printf("pressed D\n");
		xPositionShape02 += 0.1f;
		break;

		//Arrow buttons
	case Qt::Key::Key_Up:
		printf("pressed Up\n");
		yPositionShape01 += 0.1f;
		break;
	case Qt::Key::Key_Down:
		printf("pressed Down\n");
		yPositionShape01 -= 0.1f;
		break;
	case Qt::Key::Key_Left:
		printf("pressed Left\n");
		xPositionShape01 -= 0.1f;
		break;
	case Qt::Key::Key_Right:
		printf("pressed Right\n");
		xPositionShape01 += 0.1f;
		break;
	}
	repaint();
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

void MeGLWindow::initializeGL()
{
	glewInit();
	//enable the buffer
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();

}

