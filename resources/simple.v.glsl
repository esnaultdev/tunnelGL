#version 150
// ins (inputs)
in vec3 vertexPosition;
in vec4 vertexColor;
// out (outputs)
out vec4 color; 

// all those uniforms are only there for gouraud shading (otherwise they are only in th fragment shader) 
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    vec4 positionH=vec4(vertexPosition,1);
    gl_Position =  projectionMatrix * modelViewMatrix *  positionH;
    color=vertexColor;
}
