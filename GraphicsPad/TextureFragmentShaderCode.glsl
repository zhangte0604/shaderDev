#version 430


out vec4 daColor;

in vec3 normalWorld;
in vec3 vertexPositionWorld;
in vec2 TexCoord;
in vec3 tangentToModelTransform;

uniform vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;
uniform vec4 ambientLight;
uniform sampler2D Tex0;
uniform sampler2D Tex1;


void main()
{
	vec4 diffuseColor = texture(Tex0, TexCoord);
	vec4 specularColor = texture(Tex1, TexCoord);

	// Diffuse
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	//float brightness = dot(lightVectorWorld, normalize(normalWorld));
	float brightness = dot(lightVectorWorld, diffuseColor.xyz);
	
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
	//vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0) * diffuseColor;


	// Specular
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float s = dot(reflectedLightVectorWorld, eyeVectorWorld);
	s = pow(s, 10);
	vec4 specularLight = vec4(s, s, s, 1);



	//Clamp: Don't wanna distract from diffuse light when adding ambient light
	daColor = clamp(diffuseLight, 0, 1) * diffuseColor + ambientLight * diffuseColor + clamp(specularLight, 0, 1);
	
	//daColor = ambientLight + clamp(specularLight, 0, 1);
	//daColor = vec4(specularColor.x,0.0,0.0,0.0);

}