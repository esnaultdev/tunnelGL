#version 150

in vec2 UV;

out vec4 color;

uniform sampler2D textTexture;

void main(){
	color = texture( textTexture, UV );
}
