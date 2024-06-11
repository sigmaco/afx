#include <../data/pipeline/stdEnv.inc>
#include <../data/pipeline/stdView.inc>
#include <../data/pipeline/stdObject.inc>

IN(0, vec3, in_pos);
IN(3, vec3, in_nrm);

out block
{
    vec4 posn;
    vec3 nrm;
} sgl_out;

void main()
{
    sgl_out.posn = v * m * vec4(in_pos, 1);
    mat3 n = mat3(transpose(inverse(m))); // gambiarra
    sgl_out.nrm = normalize(n * in_nrm);

    gl_Position = p * v * m * vec4(in_pos, 1);
}
