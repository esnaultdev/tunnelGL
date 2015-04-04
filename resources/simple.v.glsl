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

uniform mat4 viewProjection;
uniform float time;

mat3x3 rotation = mat3x3(-sin(time), 0, cos(time),
				 cos(time),0, sin(time),
				 0, 1,0);
mat3x3 rotation2 = mat3x3(0, cos(time/3), -sin(time/3),
				 0, sin(time/3), cos(time/3),
				 1, 0,0);
mat3x3 rotation3 = mat3x3(cos(time/2), -sin(time/2), 0,
sin(time/2), cos(time/2), 0,
0,0, 1);		 

void main() {
	vec3 positionN = rotation * position;
	positionN = rotation2 * positionN;
	positionN = rotation3 * positionN;
    vec4 positionH = vec4(position, 1);
    gl_Position =  viewProjection * positionH;
    color = vertexColor;
    fragNormal = normal;
    fragUv = uv;
}
