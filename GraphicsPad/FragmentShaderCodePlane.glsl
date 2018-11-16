#version 430


out vec4 daColor;

in vec3 normalWorld;
in vec3 vertexPositionWorld;
in vec2 TexCoord;

uniform vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;
uniform vec4 ambientLight;
uniform sampler2D Tex1;


void main()
{
	vec4 texColor = texture(Tex1, TexCoord);

	// Diffuse
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalWorld));
	//vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
	
	vec4 diffuseLight = brightness * texColor;
	

	// Specular
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float s = dot(reflectedLightVectorWorld, eyeVectorWorld);
	s = pow(s, 50);
	vec4 specularLight = vec4(0, 0, s, 1);
	
	
	
	//Clamp: Don't wanna distract from diffuse light when adding ambient light
	daColor = clamp(diffuseLight, 0, 1) + ambientLight + clamp(specularLight, 0, 1);
	daColor = vec4(1,0,0,1);
	
	//daColor = ambientLight + clamp(specularLight, 0, 1);
	//daColor = clamp(specularLight, 0, 1);

}