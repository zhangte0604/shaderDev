#version 430

out vec4 daColor;

//in vec2 fragmentUV;
in vec4 clipSpaceCoordinate;

uniform sampler2D frameBufferTexture;


void main()
{
	//screen space coordinate
	//vec2 ndc = (clipSpaceCoordinate.xy / clipSpaceCoordinate.w) / 2.0 + 0.5;
	vec2 ndc = vec2(gl_FragCoord.x / 1024 + 1, gl_FragCoord.y / 1024 + 1); //Map to (0,1);
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

	//vec2 textureUV = vec2(gl_FragCoord.x / 1024 + 1, gl_FragCoord.y / 1024 + 1);
	//textureUV = normalize(textureUV);
	vec4 frameBufferColor = texture(frameBufferTexture, reflectTexCoords);

	//Clamp: Don't wanna distract from diffuse light when adding ambient light
	//daColor = clamp(diffuseLight, 0, 1) + ambientLight + clamp(specularLight, 0, 1);
	//daColor = ambientLight + clamp(specularLight, 0, 1);
	//daColor = vec4(1.0, 0.0, 0.0, 1.0);
	daColor = frameBufferColor;

}