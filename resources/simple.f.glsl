#version 150

in vec3 color ; 
in vec3 fragNormal;
in vec2 fragUv;
out vec4 fragColor;

uniform vec3 lightdirn;
vec3 lightcolor = vec3(1, 1, 1);
vec3 ambient = vec3(0.5, 0.5, 0.5);

uniform float time;
uniform sampler2D colormap;

vec3 ComputeLightLambert(const in vec3 lightdirn, const in vec3 lightcolor, const in vec3 normal, const in vec3 mydiffuse)
{
    float U = dot(lightdirn, normal);
    vec3 lambert = lightcolor * mydiffuse * max(0, U);    
    return lambert;
}

void main() {
    vec3 normal = normalize(fragNormal);
    //lightdirn = vec3(cos(time), sin(time), 1);
    vec3 lambert = ComputeLightLambert(lightdirn, lightcolor, normal, vec3(0.5,0.5,0.5));

    //fragColor = vec4( color * ambient + lambert, 1);
    fragColor = texture(colormap, fragUv) * vec4(color, 1);
    //fragColor*= vec4(ambient, 1);
    //fragColor+= vec4(lambert, 1);
    // FOG
    //fragColor *= smoothstep(25, 20, dist);
}
