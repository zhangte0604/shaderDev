#version 430

in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normalModel;
in layout(location = 3) vec2 uvModel;
in layout(location = 4) vec3 tangentModel;

uniform mat4 modelToProjectionMatrix;
uniform mat4 biasModelToProjectMatrix;
uniform mat4 modelToWorldMatrix;

out vec3 normalWorld;
out vec4 vertexPositionWorld;
out vec2 uvFragment;
out vec4 shadowCoord;

void main()
{
	// the position of the vertex as seen from the current camera
	gl_Position = modelToProjectionMatrix * vertexPositionModel;
	//the position of the vertex as seen in light's view matrix
	shadowCoord = biasModelToProjectMatrix * vertexPositionModel;

	normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 0));
	uvFragment = vec2(uvModel.x, uvModel.y);
	vertexPositionWorld = modelToWorldMatrix * vertexPositionModel;
}