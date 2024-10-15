#include <//./z/video/stdEnv.inc>

SAMPLING_UNIT(0, 0, sampler2D, samp);

in block
{
    vec2 uv0;
} sgl_v;

OUT(0, vec4, sgl_rgba);

const float shift = 0.99;

void main()
{
    vec4 orig = texture(samp, sgl_v.uv0);
    vec3 col;
    col.r = texture(samp, vec2(sgl_v.uv0.x + shift, sgl_v.uv0.y)).r;
    col.g = orig.g;
    col.b = texture(samp, vec2(sgl_v.uv0.x - shift, sgl_v.uv0.y)).b;
    col *= (1.0 - shift * 0.5);
    sgl_rgba = vec4(col, orig.a);
}
