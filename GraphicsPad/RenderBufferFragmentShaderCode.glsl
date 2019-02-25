#version 430

out vec4 daColor;

in vec2 fragmentUV;

//in vec3 normalWorld;
//in vec3 vertexPositionWorld;

//uniform vec3 lightPositionWorld;
//uniform vec3 eyePositionWorld;
//uniform vec4 ambientLight;

uniform sampler2D frameBufferTexture;


void main()
{
	
	// Diffuse
	//vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	//float brightness = dot(lightVectorWorld, normalize(normalWorld));
	//vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
	//vec4 diffuseLight = vec4(brightness, brightness, brightness, 1);
	

	// Specular
	//vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	//vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	//float s = dot(reflectedLightVectorWorld, eyeVectorWorld);
	//s = pow(s, 10);
	//vec4 specularLight = vec4(0, 0, s, 1);
	
	
	
	
	//Clamp: Don't wanna distract from diffuse light when adding ambient light
	//daColor = clamp(diffuseLight, 0, 1) + ambientLight + clamp(specularLight, 0, 1);
	//daColor = ambientLight + clamp(specularLight, 0, 1);
	//daColor = vec4(1.0, 0.0, 0.0, 1.0);
	daColor = vec4(vec3(texture(frameBufferTexture, fragmentUV)), 1.0);

}