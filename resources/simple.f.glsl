#version 150

in vec3 color ; 
in vec3 fragNormal;
in vec2 fragUv;
in vec4 vPosition;
out vec4 fragColor;

vec3 lightcolor = vec3(1, 1, 1);
vec3 ambient = vec3(0.5, 0.5, 0.5);

uniform vec3 posShip;
uniform float time;
uniform sampler2D colormap;
uniform mat4 modelWorldMatrix;

vec3 ComputeLightLambert(const in vec3 lightdirn, const in vec3 lightcolor, const in vec3 normal, const in vec3 mydiffuse)
{
    float U = dot(lightdirn, normal);
    vec3 lambert = lightcolor * mydiffuse * max(0, U);    
    return lambert;
}

void main() {
    vec3 normal = normalize(fragNormal);
    fragColor = texture(colormap, fragUv) * vec4(color, 1);
    fragColor*= vec4(ambient, 1);

    vec3 spotpos = vec3(0, 0, posShip.z);
    vec3 spotlookat = vec3(0, 0, posShip.z + 1);
    float spotangle = 0.5;
    vec3 spotcolor = vec3(1,1,1);

    vec3 spotdirn= normalize(spotpos - spotlookat);
	vec3 dirn=normalize(spotpos - vec3(modelWorldMatrix * vPosition));
	float att = 1-cos(spotangle);
	float diff = dot(spotdirn, dirn) - cos(spotangle);
	if(diff > 0)
	{
		att = pow(diff/att, 2);
		vec3 lambert = ComputeLightLambert(spotdirn.xyz, spotcolor, fragNormal, vec3(0.5,0.5,0.5));
		fragColor+=vec4(lambert*texture(colormap, fragUv).rgb,1) * att;
	}
}
