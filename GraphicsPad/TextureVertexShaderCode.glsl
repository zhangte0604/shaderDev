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

uniform mat4 modelToProjectionMatrix; //MVP 
uniform mat4 modelToWorldMatrix;
//uniform mat4 modelToViewMatrix;
uniform mat4 worldToViewMatrix;

//out vec3 vertexPositionWorld;
out vec3 vertexPositionView;
//out vec3 normalWorld;
out vec2 texCoord;
//out vec3 lightDirTangent;
//out vec3 viewDirTangent;
out mat3 viewToTangentMatrix;


/*
1. model space -> eye space $$
We'll transform the position, normal, and tangent to eye space, 
2. compute binormal vector (in eye space) $$
and then compute the binormal vector (in eye space). 
3. Next, we'll compute the viewing direction (v) and the direction towards the light source (s) in eye space, 
4. eye space -> tangent space
and then transform them to tangent space. 
5. pass data to fragment shader
We'll pass the tangent space v and s vectors and the (unchanged) texture
coordinate to the fragment shader, where we'll evaluate the Phong reflection model, using the
tangent space vectors and the normal vector retrieved from the normal map.
*/

void main()
{
	
	//Compute normalWorld & vertexPositionWorld
	//Smooth Surface Normals: the vertex of intersection uses the same normal which is the vertexPositionModel normal
	//normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 0));
	//vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);

	//Compute normalView & tangentView
	//Transform normal and tangent from model space to view space
	mat4 modelToViewMatrix = worldToViewMatrix * modelToWorldMatrix;
	vec3 normalView = normalize(mat3(modelToViewMatrix) * normalModel);
	vec3 tangentView = normalize(mat3(modelToViewMatrix) * tangentModel);

	//Compute binormalView
	vec3 binormalView = normalize(cross(normalView, tangentView)) * 1;

	//viewToTangentMatrix
	//Matrix for transformation from view space to tangent space
	viewToTangentMatrix = mat3(
		tangentView.x, binormalView.x, normalView.x,
		tangentView.y, binormalView.y, normalView.y,
		tangentView.z, binormalView.z, normalView.z);

	//Compute vertexPositionView
	vertexPositionView = vec3(modelToViewMatrix * vertexPositionModel);
	

	/*
	//Transform light dir and view dir (from view space???) to tangent space
	lightDirTangent = normalize(viewToTangentMatrix * (Light.Position.xyz - vertexPositionView));
	viewDirTangent = viewToTangentMatrix * normalize(-vertexPositionView);
	*/

	//Pass along the texture coordinate
	texCoord = vertexTexCoord;
	
	gl_Position = modelToProjectionMatrix * vertexPositionModel;

	//vec3 bitangentVector = normalize(cross(normalModel, tangentModel));
	//tangentToModelTransform = mat4(vec4(tangentModel, 0.0), vec4(bitangentVector, 0.0), vec4(normalModel, 0.0), vec4(vertexPositionModel, 1.0));
	
}
