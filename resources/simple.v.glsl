#version 150
// ins (inputs)
in vec3 position;
in vec3 vertexColor;
in vec3 normal;
in vec2 uv;

// out (outputs)
out vec4 color; 


void main() {
    vec4 positionH = vec4(position, 1);
    gl_Position =  positionH;
    color = vec4(vertexColor, 1);
}
