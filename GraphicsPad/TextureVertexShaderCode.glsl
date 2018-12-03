#version 430


in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normalModel;
in layout(location = 3) vec2 vertexTexCoord;
in layout(location = 4) vec3 tangentModel;

struct LightInfo
{
	vec4 Position; // Light position in eye coords.
	vec3 Intensity; // A,D,S intensity
};

uniform LightInfo Light;

uniform mat4 modelToProjectionMatrix; //MVP - model to world -> world to view -> view to projection
uniform mat4 modelToWorldMatrix;
uniform mat3 modelToViewMatrix;
uniform mat4 modelViewMatrix;//????????????????????????
uniform mat4 worldToViewMatrix;
uniform vec3 lightPositionWorld; //MMMMM


out vec3 vertexPositionWorld;
out vec3 normalWorld;
out vec2 texCoord;
out vec3 lightDirTangent;
out vec3 viewDirTangent;


/*
1. model space -> eye space
We'll transform the position, normal, and tangent to eye space, 
2. compute binormal vector (in eye space)
and then compute the binormal vector (in eye space). 
3. 
Next, we'll compute the viewing direction (v) and the direction towards the light source (s) in eye space, 
4. eye space -> tangent space
and then transform them to tangent space. 
5. pass data to fragment shader
We'll pass the tangent space v and s vectors and the (unchanged) texture
coordinate to the fragment shader, where we'll evaluate the Phong reflection model, using the
tangent space vectors and the normal vector retrieved from the normal map.
*/

void main()
{
	

	//Smooth Surface Normals: the vertex of intersection uses the same normal which is the vertexPositionModel normal
	//normalWorld = vec3(modelToWorldMatrix * normalize(vertexPositionModel));
	normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 0));
	vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);

	//Transform normal and tangent from model space to view space
	vec3 normalView = normalize(modelToViewMatrix * normalModel);
	vec3 tangentView = normalize(modelToViewMatrix * tangentModel);

	//Compute the binormal in view space
	vec3 binormalView = normalize(cross(normalView, tangentView)) * 1;

	//Matrix for transformation from view space to tangent space
	mat3 viewToTangentMatrix = mat3(
		tangentView.x, binormalView.x, normalView.x,
		tangentView.y, binormalView.y, normalView.y,
		tangentView.z, binormalView.z, normalView.z);

	//Get the position in view coordinates ??????????what position
	vec3 PositionView = vec3(modelViewMatrix * vertexPositionModel);

	//Transform light dir and view dir from view space to tangent space
	//vec4 lightPositionView = normalize(worldToViewMatrix * vec4(lightPositionWorld, 1.0)); //MMMM
	lightDirTangent = normalize(viewToTangentMatrix * (Light.Position.xyz - PositionView));
	viewDirTangent = viewToTangentMatrix * normalize(-PositionView);

	//Pass along the texture coordinate
	texCoord = vertexTexCoord;

	//vec4 v = vec4(vertexPositionModel, 1.0);
	
	gl_Position = modelToProjectionMatrix * vertexPositionModel;

	//vec3 bitangentVector = normalize(cross(normalModel, tangentModel));
	//tangentToModelTransform = mat4(vec4(tangentModel, 0.0), vec4(bitangentVector, 0.0), vec4(normalModel, 0.0), vec4(vertexPositionModel, 1.0));
	
}
