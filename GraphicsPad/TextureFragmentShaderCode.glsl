#version 430

out vec4 daColor;

in vec3 vertexPositionWorld;
in vec2 texCoord;
in vec3 normalWorld;
in mat3 tangentToModelMatrix;

uniform vec3 lightPositionWorld;
uniform sampler2D normalMapTex;
uniform mat4 modelToWorldMatrix; //??????? where to define this matrix
uniform vec3 eyePositionWorld;
uniform vec4 ambientLight;

void main()
{
	//Get normalTexture in world space
	vec2 flipUV = vec2(texCoord.x, -texCoord.y); //flip UV because the uv info from normal map texture is upside down
	vec4 normalTexture = texture(normalMapTex, flipUV) * 2 - 1; //the normal info from normal map texture is from 0 to 1, change it to -1 to 1
	vec3 normalTextureInModel = tangentToModelMatrix * normalTexture.xyz;
	vec3 normalTextureInWorld = vec3(modelToWorldMatrix * vec4(normalTextureInModel, 1.0));

	//Diffuse Light in world space
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalTextureInWorld));
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);

	//Specular Light in world space
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalTextureInWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float specIntensity = dot(reflectedLightVectorWorld, eyeVectorWorld);
	specIntensity = pow(specIntensity, 50);
	vec4 specularLight = vec4(0, 0, specIntensity, 1);

	daColor = clamp(diffuseLight, 0, 1) + clamp(specularLight, 0, 1) + ambientLight;
}