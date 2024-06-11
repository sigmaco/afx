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
    vec3 yuv;
    yuv.x = texture(samp0, sgl_v.uv0).r;
    yuv.y = texture(samp2, sgl_v.uv0).r - 0.5;
    yuv.z = texture(samp1, sgl_v.uv0).r - 0.5;
    
    vec3 rgb = mat3( 1, 1, 1, 0, -0.34414, 1.772, 1.402, -0.71414, 0) * yuv;
    sgl_rgba = vec4(rgb, 1);
}
