#version 430

out vec4 daColor;

in vec3 vertexPositionWorld;
in vec2 uvFrag;
//in vec3 normalWorld;
in mat3 tangentToModelMatrix;

uniform vec3 lightPositionWorld;
uniform mat4 modelToWorldMatrix; 
uniform vec3 eyePositionWorld;
uniform vec4 ambientLight;
uniform sampler2D normalMapTex;
uniform sampler2D brickTex;
uniform sampler2D mossTex;

void main()
{
	//The color texture is used as the diffuse reflectivity
	vec4 brickTexColor = texture(brickTex, uvFrag);
	vec4 mossTexColor = texture(mossTex, uvFrag);
	vec4 texColor = mix(brickTexColor, mossTexColor, mossTexColor.a);

	
	vec2 flipUV = vec2(uvFrag.x, -uvFrag.y); //flip UV because the uv info from normal map texture is upside down
	vec4 normalTexture = texture(normalMapTex, uvFrag) * 2 - 1; //the normal info from normal map texture is from 0 to 1, change it to -1 to 1
	
	//Get normalTexture in world space
	vec3 normalTextureInModel = tangentToModelMatrix * normalTexture.xyz;
	vec3 normalTextureInWorld = vec3(modelToWorldMatrix * vec4(normalTextureInModel, 1.0));

	//Diffuse Light in world space
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(-lightVectorWorld, normalize(normalTextureInWorld));
	//float brightness = dot(lightVectorWorld, uvFrag.xyz);
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);

	//Specular Light in world space
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalTextureInWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float specIntensity = dot(reflectedLightVectorWorld, -eyeVectorWorld);
	specIntensity = pow(specIntensity, 100);
	vec4 specularLight = vec4(specIntensity, specIntensity, specIntensity, 1);

	daColor = clamp(diffuseLight, 0, 1) * texColor + ambientLight * texColor;
	//daColor = clamp(diffuseLight, 0, 1) * texColor + clamp(specularLight, 0, 1) + ambientLight * texColor;
}