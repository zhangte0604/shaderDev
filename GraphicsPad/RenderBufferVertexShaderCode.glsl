#version 430

in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normalModel;
in layout(location = 3) vec2 VertexTexCoord;
in layout(location = 4) vec3 tangentModel;

uniform mat4 viewToProjectionMatrix;
uniform mat4 worldToViewMatrix;
uniform mat4 modelToWorldMatrix;

//uniform mat4 modelToProjectionMatrix;

out vec4 clipSpaceCoordinate;
//out vec2 fragmentUV;



void main()
{
	clipSpaceCoordinate = viewToProjectionMatrix * worldToViewMatrix * modelToWorldMatrix * vertexPositionModel;
	

	gl_Position = clipSpaceCoordinate;

	//fragmentUV = VertexTexCoord;

	//Smooth Surface Normals: the vertex of intersection uses the same normal which is the vertexPositionModel normal
	//normalWorld = vec3(modelToWorldMatrix * normalize(vertexPositionModel));
	//normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 0));
	//vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);
}