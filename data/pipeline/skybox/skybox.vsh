#include <data/pipeline/stdEnv.inc>
#include <data/pipeline/stdView.inc>

IN(0, vec3, a_xyz);

out block
{
    vec3 uvw0;
} sgl_v;

void main()
{
    sgl_v.uvw0 = a_xyz;
    vec4 pos = p * mat4(mat3(v)) * vec4(a_xyz, 1.0);
    
    gl_Position = pos.xyww;
}
