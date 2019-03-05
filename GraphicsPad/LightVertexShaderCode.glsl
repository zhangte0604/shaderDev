#version 430

in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normalModel;
in layout(location = 3) vec2 uvModel;
in layout(location = 4) vec3 tangentModel;

uniform mat4 modelToProjectionMatrix;


void main()
{
	gl_Position = modelToProjectionMatrix * vertexPositionModel;
}