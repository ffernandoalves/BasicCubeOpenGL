#shader vertex
#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) uniform vec3 vcolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_color;

void main() {
    // gl_Position = projection * view * model * vec4(position, 1.0);
    gl_Position = vec4(position, 1.0);
    frag_color = vcolor;
}

#shader fragment
#version 450 core

in vec3 frag_color;
out vec4 outColor;

void main() {
    outColor = vec4(frag_color, 1.0);
}
