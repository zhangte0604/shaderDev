#version 430

out vec4 daColor;

//in vec3 theColor;
in vec3 reflectedVectorWorld;
in vec3 refractedVectorWorld;
in vec4 vertexPositionWorld;
in vec3 normalWorld;
in vec3 eyeVectorWorld;

uniform vec3 lightPositionWorld;
uniform vec4 ambientLight;
//uniform vec3 eyePositionWorld;
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
	//daColor = mix(daColor, reflectedColor, 0.6);


	//Specular Light in world space
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld.xyz);
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 halfwayVectorWorld = normalize(lightVectorWorld + eyeVectorWorld);

	//Diffuse Light in world space
	float brightness = dot(lightVectorWorld, normalize(normalWorld));
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
	
	
	//-------------------phong---------------------
	float specIntensity = dot(reflectedLightVectorWorld, eyeVectorWorld);
	specIntensity = pow(specIntensity, 100);
	vec4 specularLight = vec4(specIntensity, specIntensity, specIntensity, 1);
	
	/*
	//------------------blinn----------------------
	float specIntensity = pow(max(dot(normalWorld, halfwayVectorWorld), 0.0), 50);
	vec4 specularLight = vec4(0.3, 0.3, 0.3, 1.0) * specIntensity;
	*/

	vec4 solidColor = vec4(0.0,0.6,1.0, 1.0);

	//reflection
	vec4 reflectedColor = texture(cubeMapTex, reflectedVectorWorld);
	//reflectedColor = mix(reflectedColor, solidColor, 0.5);
	//daColor = vec4(reflectedColor.rgb, 1.0);
	//refraction
	vec4 refractedColor = texture(cubeMapTex, refractedVectorWorld);

	vec4 blinnColor = vec4(ambientLight) + clamp(specularLight, 0, 1) + clamp(diffuseLight, 0, 1);
	//daColor = vec4(refractedColor.rgb, 0.0);

	//daColor = mix(reflectedColor, refractedColor, 0.8) +  clamp(specularLight, 0, 1);
	//!!!!!!!!!!!!!!!!!!!!!!!Remeber to add specular light?????????????????????????????????????????????

	//daColor = reflectedColor +  clamp(specularLight, 0, 1);
	daColor = mix(reflectedColor, blinnColor, 0.4);
	

	


	
	
	
}