#version 430

out vec4 daColor;

in vec3 vertexPositionWorld;
in vec3 normalWorld;
in vec2 texCoord;
in vec3 lightDirTangent;
in vec3 viewDirTangent;

uniform vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;
uniform vec4 ambientLight;
uniform sampler2D colorTex;
uniform sampler2D normalMapTex;

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
	
	// Diffuse
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	//float brightness = dot(lightVectorWorld, normalize(normalWorld));
	float brightness = dot(lightVectorWorld, texColor.xyz);
	
	//vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0) * texColor;


	// Specular
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float s = dot(reflectedLightVectorWorld, eyeVectorWorld);
	s = pow(s, 50);
	vec4 specularLight = vec4(0, 0, s, 1);

	
	
	//The purpose of Clamp: Don't wanna distract from diffuse light when adding ambient light
	//daColor = clamp(diffuseLight, 0, 1) * texColor + ambientLight * texColor + clamp(specularLight, 0, 1);
	daColor = clamp(diffuseLight, 0, 1) * normal + ambientLight * normal + clamp(specularLight, 0, 1);
	
	//daColor = vec4(phongModel(normal.xyz, texColor,rgb), 1.0);

	//daColor = ambientLight + clamp(specularLight, 0, 1);
	//daColor = clamp(specularLight, 0, 1);

}