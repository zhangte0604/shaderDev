#version 430


in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normalModel;
in layout(location = 3) vec2 uvModel;
in layout(location = 4) vec3 tangentModel;

uniform mat4 modelToProjectionMatrix; //MVP 
uniform mat4 modelToWorldMatrix;
//uniform mat4 worldToViewMatrix;

out vec3 vertexPositionWorld;
//out vec3 vertexPositionView;
//out vec3 normalWorld;
out vec2 uvFrag;
//out mat3 viewToTangentMatrix;
out mat3 tangentToModelMatrix;


/*
1. model space -> eye space $$
We'll transform the position, normal, and tangent to eye space, 
2. compute binormal vector (in eye space) $$
and then compute the binormal vector (in eye space). 
3. Next, we'll compute the viewing direction (v) and the direction towards the light source (s) in eye space, 
4. eye space -> tangent space
and then transfer them to tangent space. 
5. pass data to fragment shader
We'll pass the tangent space v and s vectors and the (unchanged) texture
coordinate to the fragment shader, where we'll evaluate the Phong reflection model, using the
tangent space vectors and the normal vector retrieved from the normal map.
*/

void main()
{
	vec3 bitangentModel = normalize(cross(normalModel, tangentModel));
	tangentToModelMatrix = mat3(tangentModel, bitangentModel, normalModel);//??????????????why
	//normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 0));
	vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);

	//Pass along the texture coordinate
	uvFrag = uvModel;
	
	gl_Position = modelToProjectionMatrix * vertexPositionModel;

}
