#version 450

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

void main() 
{
    vec3 pos = inPosition;
    pos.x = pos.x * 80;
    pos.y = pos.y * 60;
    pos.z = 1.0f;
    gl_Position = ubo.proj * ubo.view * vec4(pos, 1.0);
    fragTexCoord = inTexCoord;
}