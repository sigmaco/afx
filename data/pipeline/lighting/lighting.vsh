#include <../data/pipeline/stdEnv.inc>
#include <../data/pipeline/stdView.inc>
#include <../data/pipeline/stdObject.inc>

IN(0, vec3, in_pos);
IN(3, vec3, in_nrm);
IN(6, vec2, in_uv0);

out block
{
    vec4 posn;
    vec3 nrm;
    vec2 uv0;
} sgl_out;

void main()
{
    sgl_out.posn = m * vec4(in_pos, 1);
    sgl_out.nrm = in_nrm;
    sgl_out.uv0 = in_uv0;
    gl_Position = p * v * m * vec4(in_pos, 1);
}
