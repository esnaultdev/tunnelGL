#version 150

in vec3 color ; 
in vec3 fragNormal;
out vec4 fragColor ; 

vec3 lightdirn = vec3(0, 0, 1);
vec3 lightcolor = vec3(1, 1, 1);
vec3 ambient = vec3(0.6, 0.6, 0.6);

uniform float time;

vec3 ComputeLightLambert(const in vec3 lightdirn, const in vec3 lightcolor, const in vec3 normal, const in vec3 mydiffuse)
{
    float U = dot(lightdirn, normal);
    vec3 lambert = lightcolor * mydiffuse * max(0, U);    
    return lambert;
}

void main() {
    vec3 normal = normalize(fragNormal);
    //lightdirn = vec3(cos(time), sin(time), 1);
    vec3 lambert = ComputeLightLambert(lightdirn, lightcolor, normal, vec3(0.4,0.4,0.4));

    fragColor = vec4(color * ambient + lambert, 1);
}
