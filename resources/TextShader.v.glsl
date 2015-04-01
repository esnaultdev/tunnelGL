#version 150

in vec2 vertexPosition_screenspace;
in vec2 vertexUV;

out vec2 UV;

void main(){

	vec2 vertexPos = vertexPosition_screenspace - vec2(512,400); 
	vertexPos /= vec2(512.0,400.0);
	gl_Position = vec4(vertexPos,0,1);

	UV = vertexUV;
}