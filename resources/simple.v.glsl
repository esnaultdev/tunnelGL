#version 150
// ins (inputs)
in vec3 position;
in vec3 vertexColor;
in vec3 normal;
in vec2 uv;

// out (outputs)
out vec3 color; 
out vec3 fragNormal;
out vec2 fragUv;
out vec4 vPosition;

uniform mat4 viewProjection;
uniform mat4 modelWorldMatrix;
uniform float time;		 

void main() {
    vec4 positionH = vec4(position, 1);
    vPosition = positionH;
    gl_Position = viewProjection * modelWorldMatrix * positionH;
    color = vertexColor;
    fragNormal = normal;
    fragUv = uv;
}
