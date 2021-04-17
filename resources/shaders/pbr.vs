#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 vNormal;
out vec3 vPosition;
out vec2 TexCoord;
void main() {
    mat3 modelMatrix = mat3(model);
    vPosition = aPos;
    vNormal = normalize(inverse(modelMatrix) * aNormal);
    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
