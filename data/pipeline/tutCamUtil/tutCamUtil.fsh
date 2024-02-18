#include <data/pipeline/stdEnv.inc>

in block
{
    vec2 uv0;
    vec3 rgba0;
} sgl_in;

OUT(0, vec4, out_rgba0);

SAMPLING_UNIT(0, 1, sampler2D, diffuse_texture);

void main (void)
{ 
    float3 DiffuseSamp = vec3(1, 1, 1); //texture( diffuse_texture, sgl_in.uv0 );
    DiffuseSamp *= sgl_in.rgba0;

    out_rgba0 = float4(DiffuseSamp, 1);
}
