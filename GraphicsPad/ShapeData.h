#pragma once
#include <GL/glew.h>
#include <ShapeGenerator.h>

class Vertex;

struct shapeData
{
	Vertex* vertices;
	GLuint numVertices;
	GLushort* indices;
	GLuint numIndices;
};
