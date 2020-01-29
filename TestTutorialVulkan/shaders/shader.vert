#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 projection;
} iUBO;

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColor;
layout (location = 2) in vec2 iUV;

layout (location = 0) out vec3 vFragColor;
layout (location = 1) out vec2 vUV;

void main() 
{
    gl_Position = iUBO.projection * iUBO.view * iUBO.model * vec4(iPosition, 1.0);

    vFragColor = iColor;
    vUV = iUV;
}