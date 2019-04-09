#pragma once
#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW
#include <GL/glew.h>
#include <QtOpenGL\qglwidget>
#include <string>

const int TERRAIN_SIZE = 260;

class MeGlWindow : public QGLWidget
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

	//store data of the heightmap image
	GLubyte heightmap[TERRAIN_SIZE * TERRAIN_SIZE];
	//textures of grass and water
	GLuint m_grassTexture;
	GLuint m_waterTexture;

protected:
	void textureSetup();
	void initializeGL();
	void drawTerrain();
	void paintGL();
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	

public:
	~MeGlWindow();

};

#endif

