#version 430

out vec4 daColor;

//in vec3 vertexPositionWorld;
in vec3 vertexPositionView;
//in vec3 vertexPositionTangent;
//in vec3 normalWorld;
in vec2 texCoord;
//in vec3 lightDirTangent;
//in vec3 viewDirTangent;
in mat3 viewToTangentMatrix;

uniform vec3 lightPositionWorld;
uniform vec3 lightPositionTangent;
uniform vec3 eyePositionWorld;
//uniform vec3 eyePositionTangent;
uniform vec4 ambientLight;
uniform sampler2D colorTex;
uniform sampler2D normalMapTex;

uniform mat4 worldToViewMatrix;

/*
struct LightInfo
{
	vec4 Position; // Light position in eye coords.
	vec3 Intensity; // A,D,S intensity
};

uniform LightInfo Light;

struct MaterialInfo
{
	vec3 Ka; // Ambient reflectivity
	vec3 Ks; // Specular reflectivity
	float shininess; // Specular shininess factor
};

uniform MaterialInfo Material;

vec3 phongModel(vec3 norm, vec3 diffR)
{
	vec3 r = reflect(-lightDirTangent, norm);
	vec3 ambient = Light.Intensity * Material.Ka;
	float sDotN = max(dot(lightDirTangent, norm), 0.0);
	vec3 diffuse = Light.Intensity * diffR * sDotN;

	vec3 spec = vec3(0.0);
	if(sDotN > 0.0)
		spec = Light.Intensity * Material.Ks *
			pow(max(dot(r, viewDirTangent), 0.0), Material.shininess);

	return ambient + diffuse + spec;
}
*/

void main()
{
	//The color texture is used as the diffuse reflectivity
	vec4 texColor = texture(colorTex, texCoord);

	//Lookup the normal from the normal map
	vec4 normal = texture(normalMapTex, texCoord);
	
	
	/*
	// Diffuse in world space
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, texColor.xyz);
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
	*/	

	/*
	// Specular in world space
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float specIntensity = dot(reflectedLightVectorWorld, eyeVectorWorld);
	specIntensity = pow(specIntensity, 50);
	vec4 specularLight = vec4(0, 0, specIntensity, 1);
	*/
	
	//Diffuse in tangent space
	vec3 lightPositionView = vec3(worldToViewMatrix * vec4(lightPositionWorld, 1.0));
	vec3 lightVectorTangent = normalize(viewToTangentMatrix * (lightPositionView - vertexPositionView));
	float brightness = dot(lightVectorTangent, normalize(normal.xyz));
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0) * 0.5;
	
	//float brightness = dot(lightDirTangent, normalize(normal.xyz)); //old
	//vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0); //old

	
	//Specular in tangent space
	vec3 reflectedLightVectorTangent = reflect(-lightVectorTangent, normal.xyz);
	vec3 eyeVectorTangent = viewToTangentMatrix * normalize(-vertexPositionView);
	vec4 specularLight = vec4(0.0);
	if(max(dot(lightVectorTangent, normal.xyz), 0.0) > 0.0)
	{
	float specIntensity = max(dot(reflectedLightVectorTangent, eyeVectorTangent), 0.0);
	specIntensity = pow(specIntensity, 50);
	specularLight = vec4(0, 0, specIntensity, 1);
	}


	/*
	//Specular in tangent space old
	vec3 reflectedLightVectorTangent = reflect(-lightVectorTangent, normal.xyz);
	//vec3 eyeVectorTangent = normalize(eyePositionTangent - vertexPositionTangent);
	float specIntensity = dot(reflectedLightVectorTangent, viewDirTangent);
	specIntensity = pow(specIntensity, 50);
	vec4 specularLight = vec4(0, 0, specIntensity, 1);
	*/

	/* //from vertex shader
	//Transform light dir and view dir (from view space???) to tangent space
	lightDirTangent = normalize(viewToTangentMatrix * (Light.Position.xyz - vertexPositionView));
	viewDirTangent = viewToTangentMatrix * normalize(-vertexPositionView);
	*/

	//The purpose of Clamp: Don't wanna distract from diffuse light when adding ambient light
	daColor = clamp(diffuseLight, 0, 1) + ambientLight + clamp(specularLight, 0, 1);
	


	//daColor = clamp(diffuseLight, 0, 1) * texColor + ambientLight * texColor + clamp(specularLight, 0, 1);
	//daColor = vec4(phongModel(normal.xyz, texColor,rgb), 1.0);
	//daColor = ambientLight + clamp(specularLight, 0, 1);
	//daColor = clamp(specularLight, 0, 1);

}