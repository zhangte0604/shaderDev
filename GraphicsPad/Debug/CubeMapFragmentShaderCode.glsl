#version 430

out vec4 daColor;

in vec3 reflectDir; // The direction of the reflected ray

in vec3 uvFrag;

uniform samplerCube cubeMapTex; 
uniform bool drawSkyBox;
uniform float reflectFactor;  // Amount of reflection
uniform vec4 materialColor; // Color of the obj's "Tint"

void main()
{
	daColor = texture(cubeMapTex, uvFrag);
	
	/*
	// Access the cube map texture
 	vec4 cubeMapColor = texture(cubeMapTex, reflectDir);

	if( drawSkyBox )
 		daColor = cubeMapColor;

	else
		daColor = mix(materialColor, cubeMapColor, reflectFactor);
	*/

	/*
	//The color texture is used as the diffuse reflectivity
	vec4 brickTexColor = texture(brickTex, uvFrag);
	vec4 mossTexColor = texture(mossTex, uvFrag);
	vec4 texColor = mix(brickTexColor, mossTexColor, mossTexColor.a);

	
	vec2 flipUV = vec2(uvFrag.x, -uvFrag.y); //flip UV because the uv info from normal map texture is upside down
	vec4 normalTexture = texture(normalMapTex, uvFrag); //the normal info from normal map texture is from 0 to 1, change it to -1 to 1
	
	//Get normalTexture in world space
	vec3 normalTextureInModel = tangentToModelMatrix * normalTexture.xyz;
	vec3 normalTextureInWorld = vec3(modelToWorldMatrix * vec4(normalTextureInModel, 1.0));

	//Diffuse Light in world space
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalTextureInWorld));
	//float brightness = dot(lightVectorWorld, uvFrag.xyz);
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);

	//Specular Light in world space
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalTextureInWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float specIntensity = dot(reflectedLightVectorWorld, eyeVectorWorld);
	specIntensity = pow(specIntensity, 100);
	vec4 specularLight = vec4(specIntensity, specIntensity, specIntensity, 1);

	daColor = clamp(diffuseLight, 0, 1) * texColor + ambientLight * texColor;
	//daColor = clamp(diffuseLight, 0, 1) * texColor + clamp(specularLight, 0, 1) + ambientLight * texColor;
	*/
}