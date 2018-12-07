#version 430

out vec4 daColor;

in vec3 vertexPositionWorld;
in vec2 texCoord;

uniform vec3 lightPositionWorld;
uniform sampler2D normalMapTex;

void main()
{
	mat3 tangentToModel = mat3(tangent,   biTangent, normalFromModel)





	vec2 test = vec2(texCoord.x, -texCoord.y);
	vec3 normal = texture(normalMapTex, test).xyz * 2  - 1;
	vec3 lightVectorWorld = normalize(vec3(0,0,  3) - vertexPositionWorld);

	float b = dot(lightVectorWorld, normalize(normal));
	vec4 diffuseLight = vec4(b, b, b, 1.0);
	daColor = clamp(diffuseLight, 0, 1);
}