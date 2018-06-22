#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform SCamUniBuffer
{
    mat4 view;
    mat4 proj;
} cam_ubo;

layout(binding = 1) uniform SObjUniBuffer
{
    mat4 obj_world;
    float tex_mul;
} obj_ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out float fragTexMul;

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main()
{
    gl_Position = cam_ubo.proj * cam_ubo.view * obj_ubo.obj_world * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
    fragTexMul = obj_ubo.tex_mul;
}