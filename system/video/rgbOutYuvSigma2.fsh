#include <../system/video/stdEnv.inc>

SAMPLING_UNIT(0, 0, sampler2D, samp0);
SAMPLING_UNIT(0, 1, sampler2D, samp1);
SAMPLING_UNIT(0, 2, sampler2D, samp2);

in block
{
    vec2 uv0;
} sgl_v;

OUT(0, vec4, sgl_rgba);

void main()
{
    vec4 c = vec4((texture(samp0, sgl_v.uv0).r - 16.0/255.0) * 1.164);
    vec4 u = vec4(texture(samp2, sgl_v.uv0).r - 128.0/255.0);
    vec4 v = vec4(texture(samp1, sgl_v.uv0).r - 128.0/255.0);
    c += v * vec4(1.596, -0.813, 0, 0);
    c += u * vec4(0, -0.392, 2.017, 0);
    c.a = 1.0;
    sgl_rgba = c;
}
