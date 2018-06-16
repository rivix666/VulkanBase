#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(binding = 0) uniform UniformBufferObject 
{
    // mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main()
{
    //mat4 view_proj = ubo.proj * ubo.view;
    //gl_Position = view_proj * vec4(inPosition, 1.0);

    gl_Position = ubo.proj * ubo.view * vec4(inPosition, 1.0);
    fragColor = inColor;
}