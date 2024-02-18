#include <data/pipeline/stdEnv.inc>
#include <data/pipeline/stdView.inc>
#include <data/pipeline/stdObject.inc>

IN(0, vec3, aPos);
IN(3, vec3, aNormal);
IN(6, vec2, aUv);

const vec4 AmbientColour = vec4(0.2, 0.2, 0.2, 1.0);

out block
{
    vec3 xyz;
    vec3 nrm;
    vec2 uv0;
} sgl_v;

void main()
{
    sgl_v.xyz = vec3(m * vec4(aPos, 1));
    sgl_v.nrm = mat3(transpose(inverse(m))) * aNormal;  
    
    gl_Position = p * v * vec4(sgl_v.xyz, 1.0);
}
