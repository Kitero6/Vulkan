#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vFragColor;

layout(location = 0) out vec4 oColor;

void main()
{
    oColor = vec4(vFragColor, 1.0);
}