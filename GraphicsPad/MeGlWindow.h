#pragma once
#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW
#include <GL/glew.h>
#include <QtOpenGL\qglwidget>
#include <string>

class MeGLWindow : public QGLWidget
{
	void sendDataToOpenGL();
	bool checkStatus(GLuint objectID,
		PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
		PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
		GLenum statusType);
	bool checkShaderStatus(GLuint shaderID);
	bool checkProgramStatus(GLuint programID);
	std::string readShaderCode(const char* fileName);
	void installShaders();
protected:
	void initializeGL();
	void paintGL();
public:
	~MeGLWindow();

};

#endif

