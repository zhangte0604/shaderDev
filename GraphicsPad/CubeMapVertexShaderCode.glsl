#version 430


in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normalModel;
in layout(location = 3) vec2 uvModel;
in layout(location = 4) vec3 tangentModel;

//uniform bool drawSkyBox; // Are we drawing the sky box?
uniform mat4 skyboxTransformMatrix;
uniform mat4 modelToWorldMatrix;
uniform vec3 eyePositionWorld;

//out vec3 reflectDir; //The direction of the reflected ray

out vec3 uvFrag;

void main()
{
	vec3 vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);
	vec3 normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 0.0));
	//uvFrag = vec3(vertexPositionModel.x, vertexPositionModel.y, vertexPositionModel.z);
	//uvFrag = vec3(vertexPositionModel);
	vec3 viewDirWorld = normalize(vertexPositionWorld - eyePositionWorld);
	uvFrag = reflect(-viewDirWorld, normalize(normalWorld));
	
	gl_Position = skyboxTransformMatrix * vertexPositionModel;
	
	
	
		/*
	if(drawSkyBox)
	{
		reflectDir = vec3(vertexPositionModel);
	}
	else
	{
		// Compute the reflected direction in world coords
		//Compute normalWorld & vertexPositionWorld
	    //Smooth Surface Normals: the vertex of intersection uses the same normal which is the vertexPositionModel normal
	    vec3 vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);
        vec3 normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 0.0));
	    vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);

		reflectDir = reflect(-eyeVectorWorld, normalWorld);
	}
	
	//Pass along the texture coordinate
	//uvFrag = uvModel;
	
	gl_Position = modelToProjectionMatrix * vertexPositionModel;
	*/



}
