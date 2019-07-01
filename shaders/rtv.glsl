#version 410

layout(location = 0) in vec3 vertexPosition_modelspace;
out vec2 vertexPassThrough;
void main()
{
    vertexPassThrough = vertexPosition_modelspace.xy;
    gl_Position = vec4(vertexPosition_modelspace.xyz, 1.0); //so that it doesn't clip;
}
