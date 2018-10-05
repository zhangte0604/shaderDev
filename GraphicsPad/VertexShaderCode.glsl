
#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;



uniform float xMove;
uniform float yMove;

uniform vec3 dominatingColor;

out vec3 theColor;

void main()
{
	gl_Position = vec4(position, 1.0);

	gl_Position.x = gl_Position.x + xMove;
	gl_Position.y = gl_Position.y + yMove;

	theColor = dominatingColor;
}

