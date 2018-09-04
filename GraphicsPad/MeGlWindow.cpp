#include <gl\glew.h>
#include <MeGLWindow.h>

//#define SCREEN_WIDTH 480
//#define SCREEN_HEIGHT 480

extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;

//void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
//	int i;
//	int triangleAmount = 20; //# of triangles used to draw circle
//
//							 //GLfloat radius = 0.8f; //radius
//	GLfloat twicePi = 2.0f * 3.14;
//
//	glBegin(GL_TRIANGLE_FAN);
//	glVertex2f(x, y); // center of circle
//	for (i = 0; i <= triangleAmount; i++) {
//		glVertex2f(
//			x + (radius * cos(i *  twicePi / triangleAmount)),
//			y + (radius * sin(i * twicePi / triangleAmount))
//		);
//	}
//	glEnd();
//}

void sendDataToOpenGL()
{
	GLfloat verts[] =
	{
		//Triangle's location
		//and color

		-1.0f, +0.4f, //vert 0
		+1.0f, +0.5f, +0.0f,

		+1.0f, +0.4f, //1
		+1.0f, +0.5f, +0.0f,

		+0.0f, -1.0f, //2
		+1.0f, +0.5f, +0.0f,

		-0.5f, +1.0f, //3
		+1.0f, +0.2f, +0.0f,

		+0.0f, +0.4f, //4
		+1.0f, +0.2f, +0.0f,

		+0.5f, +1.0f, //5
		+1.0f, +0.2f, +0.0f,

		-0.5f, +0.2f, //6
		+0.0f, +0.0f, +0.0f,

		-0.3f, +0.2f, //7
		+0.0f, +0.0f, +0.0f,

		-0.4f, +0.1f, //8
		+0.0f, +0.0f, +0.0f,

		+0.5f, +0.2f, //6
		+0.0f, +0.0f, +0.0f,

		+0.3f, +0.2f, //7
		+0.0f, +0.0f, +0.0f,

		+0.4f, +0.1f, //8
		+0.0f, +0.0f, +0.0f,

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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

	//enable the vertex attribute (color)
	glEnableVertexAttribArray(1);
	//describe color data to openGL
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2));

	//how the vertices connected
	GLushort indices[] = { 0,1,2, 0,3,4, 4,5,1, 6,7,8, 9,10,11 };
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);

}


void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//define array of character pointers
	const char* adapter[1];
	adapter[0] = vertexShaderCode;
	glShaderSource(vertexShaderID, 1, adapter, 0);
	adapter[0] = fragmentShaderCode;
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	glUseProgram(programID);
}

void MeGLWindow::initializeGL()
{
	glewInit();
	sendDataToOpenGL();
	installShaders();

}

//run everytime you call
void MeGLWindow::paintGL()
{
	//make screen red
	//glClearColor(1, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);


	//render the triangle on the full screen
	glViewport(0, 0, width(), height());
	//draw a triangle using the verts
	
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//using dices to draw TWO triangles
	glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_SHORT, 0);

}
