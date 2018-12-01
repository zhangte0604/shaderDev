#version 430


in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normalModel;
in layout(location = 3) vec2 VertexTexCoord;
in layout(location = 4) vec3 tangentModel;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;

out vec3 vertexPositionWorld;
out vec3 normalWorld;
out vec2 TexCoord;
out mat4 tangentToModelTransform;


void main()
{
	//vec4 v = vec4(vertexPositionModel, 1.0);
	
	gl_Position = modelToProjectionMatrix * vertexPositionModel;

	//Smooth Surface Normals: the vertex of intersection uses the same normal which is the vertexPositionModel normal
	//normalWorld = vec3(modelToWorldMatrix * normalize(vertexPositionModel));
	normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 0));
	vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);

	TexCoord = VertexTexCoord;
	
	//vec3 bitangentVector = normalize(cross(normalModel, tangentModel));
	//tangentToModelTransform = mat4(vec4(tangentModel, 0.0), vec4(bitangentVector, 0.0), vec4(normalModel, 0.0), vec4(vertexPositionModel, 1.0));
	
}
