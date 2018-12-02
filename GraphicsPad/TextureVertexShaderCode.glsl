#version 430


in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normalModel;
in layout(location = 3) vec2 vertexTexCoord;
in layout(location = 4) vec3 vertexTangent;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 normalMatrix;

out vec3 vertexPositionWorld;
out vec3 normalWorld;
out vec2 texCoord;
//out mat4 tangentToModelTransform;


void main()
{
	//vec4 v = vec4(vertexPositionModel, 1.0);
	
	gl_Position = modelToProjectionMatrix * vertexPositionModel;

	//Smooth Surface Normals: the vertex of intersection uses the same normal which is the vertexPositionModel normal
	//normalWorld = vec3(modelToWorldMatrix * normalize(vertexPositionModel));
	normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 0));
	vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);

	TexCoord = VertexTexCoord;

	//Transform normal and tangent to eye space
	vec3 norm = normalize(normalMatrix * normalModel);
	vec3 tang = normalize(normalMatrix * vertexTangent);
	//Compute the binormal
	vec3 binormal = normalize(cross(norm, tang)) * 1;

	//Matrix for transformation to tangent space
	mat3 toObjectLocal = mat3(
		tang.x, binormal.x, norm.x,
		tang.y, binormal.y, norm.y,
		tang.z, binormal.z, norm.z);

	//

	
	//vec3 bitangentVector = normalize(cross(normalModel, tangentModel));
	//tangentToModelTransform = mat4(vec4(tangentModel, 0.0), vec4(bitangentVector, 0.0), vec4(normalModel, 0.0), vec4(vertexPositionModel, 1.0));
	
}
