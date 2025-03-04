#include <//./z/video/stdEnv.inc>

TEXTURE(0, 1, sampler2D, tex);

in block
{
    vec2 vUv;
    vec4 vRgba;
} sgl_v;

OUT(0, vec4, outColor);

void main(void)
{
    outColor = sgl_v.vRgba * texture(tex, sgl_v.vUv.st);
}
