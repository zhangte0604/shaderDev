#version 430

layout( vertices=16 ) out;
uniform int TessLevel;

in PipelineData {
    vec4 position;
} tc_in[];

out PipelineData {
    vec4 position;
} tc_out[];

void main()
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	tc_out[gl_InvocationID].position = tc_in[gl_InvocationID].position;

	gl_TessLevelOuter[0] = float(TessLevel);
	gl_TessLevelOuter[1] = float(TessLevel);
	gl_TessLevelOuter[2] = float(TessLevel);
	gl_TessLevelOuter[3] = float(TessLevel);

	gl_TessLevelInner[0] = float(TessLevel);
	gl_TessLevelInner[1] = float(TessLevel);
}