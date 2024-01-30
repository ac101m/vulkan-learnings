#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} uniforms;

layout(location = 0) in vec2 positionIn;
layout(location = 1) in vec3 colorIn;

layout(location = 0) out vec3 colorOut;

void main() {
    gl_Position = uniforms.projection * uniforms.view * uniforms.model * vec4(positionIn, 0.0, 1.0);
    colorOut = colorIn;
}
