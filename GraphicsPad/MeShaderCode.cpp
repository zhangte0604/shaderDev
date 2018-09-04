////define the compilation unit
////source code
//const char* vertexShaderCode =
//"#version 430\r\n"
//""
//"in layout(location = 0) vec2 position;"
//"in layout(location = 1) vec3 vertexColor;"
//""
////send data out of vertex shadr
//"out vec3 theColor;"
//""
//"void main()"
//"{"
//"	gl_position = vec4(position, 0.0, 1.0);"
//"	theColor = vertexColor;"
//"}"
//"";
//
//const char* fragmentShaderCode =
//"#version 430\r\n"
//""
////"out" means sending color out to the fragment shader
//"out vec4 daColor;"
////what comes out of vertex shader must come in the fragment shader
//"in vec3 theColor;"
//""
//"void main()"
//"{"
//"daColor = vec4(0.0, 1.0, 0.0, 1.0);"
//"daColor = vec4(theColor, 1.0);"
//"}";


//const char* vertexShaderCode =
//"#version 430\r\n"
//""
//"in layout(location=0) vec2 position;"
//""
//"void main()"
//"{"
//"	gl_Position = vec4(position, 0.0, 1.0);"
//"}"
//"";
//
//const char* fragmentShaderCode =
//"#version 430\r\n"
//""
//"out vec4 daColor;"
//""
//"void main()"
//"{"
//"	daColor = vec4(0.0, 1.0, 0.0, 1.0);"
//"}";

const char* vertexShaderCode =
"#version 430\r\n"
""
"in layout(location=0) vec2 position;"
"in layout(location=1) vec3 vertexColor;"
""
"out vec3 theColor;"
""
"void main()"
"{"
"	gl_Position = vec4(position, 0.0, 1.0);"
"	theColor = vertexColor;"
"}"
"";

const char* fragmentShaderCode =
"#version 430\r\n"
""
"out vec4 daColor;"
"in vec3 theColor;"
""
"void main()"
"{"
"	daColor = vec4(theColor, 1.0);"
"}";