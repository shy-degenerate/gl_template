#version 330 core 

layout (location = 0) in vec3 f_pos;
layout (location = 1) in vec3 f_color;

out vec4 color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * model * vec4(f_pos, 1.0);
    color = vec4(f_color, 1.0);
}