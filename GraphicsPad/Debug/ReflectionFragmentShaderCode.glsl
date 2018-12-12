#version 430

out vec4 daColor;

in vec3 theColor;
in vec3 reflectedVectorWorld;

//uniform vec3 lightPositionWorld;
//uniform vec4 ambientLight;
uniform vec3 eyePositionWorld;
uniform samplerCube cubeMapTex;


void main()
{
	//vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	//float brightness = dot(lightVectorWorld, normalize(normalWorld));
	//vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
	
	//Clamp: Don't wanna distract from diffuse light when adding ambient light
	//daColor = clamp(diffuseLight, 0, 1) + ambientLight;

	//eye direction in world space
	//vec3 eyeVectorWorld = normalize(vertexPositionWorld - eyePositionWorld);
	//reflection direction in world space
	//vec3 reflectedVectorWorld = reflect(eyeVectorWorld, normalize(normalWorld);//??????????????why not work with this line??????????????

	//daColor = vec4(1.0,0.0,0.0, 1.0);

	vec4 reflectedColor = texture(cubeMapTex, reflectedVectorWorld);

	//daColor = mix(daColor, reflectedColor, 0.6);

	daColor = vec4(reflectedColor.rgb, 1.0);
	
	
}