#version 430

out float FragmentDepth;

void main()
{
    FragmentDepth = gl_FragCoord.z;
}