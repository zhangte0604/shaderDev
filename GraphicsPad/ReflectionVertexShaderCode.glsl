#version 430


in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normalModel;
in layout(location = 3) vec2 uvModel;
in layout(location = 4) vec3 tangentModel;

uniform mat4 modelToProjectionMatrix2;
uniform mat4 modelToWorldMatrix2;
uniform vec3 eyePositionWorld;

//out vec3 normalWorld;
//out vec3 vertexPositionWorld;
out vec3 theColor;
out vec3 reflectedVectorWorld;

void main()
{
	
	

	//Smooth Surface Normals: the vertex of intersection uses the same normal which is the vertexPositionModel normal
	vec3 normalWorld = vec3(mat4(transpose(inverse(modelToWorldMatrix2))) * vec4(normalModel, 0));
	//normalWorld = mat3(transpose(inverse(modelToWorldMatrix2))) * normalModel;
	vec4 vertexPositionWorld = modelToWorldMatrix2 * vertexPositionModel;
	gl_Position = modelToProjectionMatrix2 * vertexPositionModel;

	theColor = vertexColor;

	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld.xyz);
	reflectedVectorWorld = reflect(eyeVectorWorld, normalize(normalWorld));


	
}