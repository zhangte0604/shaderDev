#version 430

out vec4 daColor;

in vec3 reflectDir; //The direction of the reflected ray

uniform samperCube cubeMapTex;
uniform bool drawSkyBox; //are we drawing the sky box?
uniform float reflectFactor; //Amount of reflection
uniform vec4 materialColor; //Color of the object's "Tint"

void main()
{
    //Access the cube map texture
    vec4 cubeMapColor = texture(cubeMapTex, reflectDir);

    if(drawSkyBox)
    {
        daColor = cubeMapColor;
    }
    else
        daColor = mix(materialColor, cubeMapColor, reflectFactor);

}