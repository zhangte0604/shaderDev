#pragma once
#include <GL/glew.h>
#include <ShapeGenerator.h>

class Vertex;

struct ShapeData
{
	Vertex* vertices;
	GLuint numVertices;
	GLushort* indices;
	GLuint numIndices;
};
