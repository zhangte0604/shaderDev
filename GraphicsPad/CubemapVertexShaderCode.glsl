#version 430


in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normalModel;
in layout(location = 3) vec2 vertexTexCoord;
in layout(location = 4) vec3 tangentModel;


uniform bool drawSkyBox; // Are we drawing the sky box?
uniform vec3 worldCameraPosition;

out vec3 reflectDir; //The direction of the reflected ray


void main()
{
	
	//cube map
	if(drawSkyBox)
	{
		reflectDir = vertexPositionModel;
	}
	else
	{
		//Compute the reflected direction in world space
        //Compute normalWorld & vertexPositionWorld
	    //Smooth Surface Normals: the vertex of intersection uses the same normal which is the vertexPositionModel normal
	    vec3 vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);
        vec3 normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 0));
	    vec3 viewWorld = normalize(worldCameraPosition - vertexPositionWorld);
	}
	
	gl_Position = modelToProjectionMatrix * vertexPositionModel;

}
