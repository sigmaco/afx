#include <../system/video/stdEnv.inc>

SAMPLING_UNIT(0, 0, sampler2D, samp);

in block
{
    vec2 uv0;
} sgl_v;

OUT(0, vec4, sgl_rgba);

void main()
{
    sgl_rgba = texture(samp, sgl_v.uv0);
}
