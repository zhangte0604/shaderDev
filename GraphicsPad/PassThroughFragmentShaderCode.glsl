#version 430

in vec3 normalWorld;
in vec4 vertexPositionWorld;
in vec2 uvFragment;
in vec4 shadowCoord;

out vec4 daColor;

uniform vec3 pointLightPosition;
uniform float pointLightIntensity;
uniform vec3 ambientLightUniform;
uniform vec3 cameraPositionWorld;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D frameBufferTexture;

void main()
{
	vec4 diffuseColor = texture(diffuseTexture, uvFragment);
	vec4 specularColor = texture(specularTexture, uvFragment);

	// diffuse
	vec3 lightVectorWorld = normalize(pointLightPosition - vec3(vertexPositionWorld));
	float diffuseIntensity = dot(lightVectorWorld,normalize(normalWorld));
	vec4 diffuseLight = vec4(diffuseColor.x *diffuseIntensity,diffuseColor.y *diffuseIntensity,diffuseColor.z * diffuseIntensity,1.0);

	//specular
	vec3 reflectedLightVectorWorld = reflect(lightVectorWorld, normalize(normalWorld));
	vec3 cameraToWorld = -normalize(cameraPositionWorld - vec3(vertexPositionWorld));
	float SpecIntensity =  dot(cameraToWorld,reflectedLightVectorWorld);
	SpecIntensity = pow(SpecIntensity,6);
	vec4 specLight = specularColor.x * vec4(SpecIntensity,SpecIntensity,SpecIntensity,1.0);



	vec2 poissonDisk[4] = vec2[](
		vec2( -0.94201624, -0.39906216 ),
		vec2( 0.94558609, -0.76890725 ),
		vec2( -0.094184101, -0.92938870 ),
		vec2( 0.34495938, 0.29387760 )
	);

	float bias = 0.004;
	float visibility = 1.0;
	for (int i=0;i<4;i++){
		if ( texture( frameBufferTexture, shadowCoord.xy + poissonDisk[i]/700.0 ).z  <  shadowCoord.z-bias ){
			visibility-=0.2;
		}
	}

	daColor = visibility * (clamp(diffuseLight,0,1) + vec4(ambientLightUniform,0.0) + clamp(specLight,0,1));
}