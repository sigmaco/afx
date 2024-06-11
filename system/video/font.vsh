#include <../system/video/stdEnv.inc>
#include <../system/video/stdView.inc>

IN(0, vec2, in_instPos);
IN(1, vec2, in_instUv);
IN(2, vec4, in_instRgba);

const vec2 pos[4] =
{
    vec2( 0, 16),
    vec2( 0, 0),
    vec2(16, 16),
    vec2(16, 0)
};
const vec2 uv[4] =
{
    vec2(0.00625, -0.00625),
    vec2(0.00625, -0.05625),
    vec2(0.05625, -0.00625),
    vec2(0.05652, -0.05625)
};

out block
{
    vec2 uv;
    vec4 rgba;
} sgl_v;

void main()
{
    gl_Position = p * vec4((pos[gl_VertexID] + in_instPos), 0.0, 1.0);
    sgl_v.uv = uv[gl_VertexID] + in_instUv;
    sgl_v.rgba = vec4(1,1,1,1);//in_instRgba;
}
