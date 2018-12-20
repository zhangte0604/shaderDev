#version 430


in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normalModel;
in layout(location = 3) vec2 uvModel;
in layout(location = 4) vec3 tangentModel;

uniform mat4 modelToProjectionMatrix2;
uniform mat4 modelToWorldMatrix2;
uniform vec3 eyePositionWorld;

out vec3 normalWorld;
out vec4 vertexPositionWorld;
out vec3 eyeVectorWorld;
//out vec3 theColor;
out vec3 reflectedVectorWorld;
out vec3 refractedVectorWorld;

void main()
{
	
	

	//Smooth Surface Normals: the vertex of intersection uses the same normal which is the vertexPositionModel normal
	normalWorld = vec3(mat4(transpose(inverse(modelToWorldMatrix2))) * vec4(normalModel, 0));
	//normalWorld = vec3(modelToWorldMatrix2 * vec4(normalModel, 0));
	vertexPositionWorld = modelToWorldMatrix2 * vertexPositionModel;

	gl_Position = modelToProjectionMatrix2 * vertexPositionModel;
	
	eyeVectorWorld = normalize(vertexPositionWorld.xyz - eyePositionWorld );

	//reflection
	reflectedVectorWorld = reflect(eyeVectorWorld, normalize(normalWorld)); 

	//refraction
	float refractionRatio = float(1.00 / 1.52);
	refractedVectorWorld = refract(eyeVectorWorld, normalize(normalWorld), refractionRatio);

	


	
}