#include <../data/pipeline/stdEnv.inc>
#include <../data/pipeline/stdView.inc>

IN(0, vec3, a_xyz);

void main()
{
    vec4 pos = p * v * vec4(a_xyz, 1.0);
    gl_Position = pos;
}
