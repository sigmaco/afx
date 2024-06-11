#include <../system/video/stdEnv.inc>

SAMPLING_UNIT(0, 1, sampler2D, atlas);

in block
{
    vec2 uv;
    vec4 rgba;
} sgl_v;

OUT(0, vec4, sgl_rgba);

void main()
{    
    vec4 sampled = texture(atlas, sgl_v.uv); //vec4(1.0, 1.0, 1.0, texture(samp, sgl_v.uv0).a);
    sgl_rgba = sgl_v.rgba * sampled;
    //sgl_rgba = vec4(/*sgl_v.rgba.rgb*/vec3(1.0, 1.0, 1.0), texture(atlas, sgl_v.uv).r);
}
