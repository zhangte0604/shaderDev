#version 430

layout( quads ) in;

in PipelineData {
    vec4 position;
} te_in[];

out PipelineData {
    vec4 position;
} te_out;

uniform mat4 modelToProjectionMatrix;

void basisFunctions(out float[4] b, out float[4] db, float t){
	float t1 = (1.0 - t);
	float t12 = t1 * t1;

	b[0] = t12 * t1;    
	b[1] = 3.0 * t12 * t;    
	b[2] = 3.0 * t1 * t * t;    
	b[3] = t * t * t;

	db[0] = -3.0 * t1 * t1;    
	db[1] = -6.0 * t * t1 + 3.0 * t12;   
	db[2] = -3.0 * t * t + 6.0 * t * t1;    
	db[3] = 3.0 * t * t;
}

void main()
{
	vec4 TEPosition; 
	float u = gl_TessCoord.x;    
	float v = gl_TessCoord.y;

	// The sixteen control points    
	vec4 p00 = gl_in[0].gl_Position;    
	vec4 p01 = gl_in[1].gl_Position;   
	vec4 p02 = gl_in[2].gl_Position;   
	vec4 p03 = gl_in[3].gl_Position;   


	float bu[4], bv[4];   
	// Basis functions for u and v  
	float dbu[4], dbv[4]; 
	// Derivitives for u and v  
	basisFunctions(bu, dbu, u);   
	basisFunctions(bv, dbv, v);

	TEPosition =    
	p00*bu[0]*bv[0] + p01*bu[0]*bv[1] + p02*bu[0]*bv[2] +     
	p03*bu[0]*bv[3];

	te_out.position  = gl_TessCoord.x * te_in[0].position;
   te_out.position += gl_TessCoord.y * te_in[1].position;
    te_out.position += gl_TessCoord.z * te_in[2].position;
	gl_Position = te_out.position;
}