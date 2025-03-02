#version 460

/* layout(binding = 0) uniform SpaceTransformObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} sto; */

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;


void main() {
    fragColor = color;
    gl_Position = /* sto.model * */ vec4(position, 1.f);
} 