
#version 430

in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normalModel;
in layout(location = 3) vec2 VertexTexCoord;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;

out vec3 vertexPositionWorld;
out vec3 normalWorld;
out vec2 TexCoord;


void main()
{
	//vec4 v = vec4(vertexPositionModel, 1.0);
	gl_Position = modelToProjectionMatrix * vertexPositionModel;

	//Smooth Surface Normals: the vertex of intersection uses the same normal which is the vertexPositionModel normal
	//normalWorld = vec3(modelToWorldMatrix * normalize(vertexPositionModel));
	normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 0));
	vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);

	TexCoord = VertexTexCoord;
	
}

