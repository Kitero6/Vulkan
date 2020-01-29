#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 vFragColor;
layout (location = 1) in vec2 vUV;

layout (location = 0) out vec4 oColor;

layout (binding = 1) uniform sampler2D uTexture;

void main()
{
    oColor = texture(uTexture, vUV) * vec4(vFragColor, 1.0);
}