#include <data/pipeline/stdEnv.inc>
#include <data/pipeline/stdView.inc>
#include <data/pipeline/stdObject.inc>

IN(0, vec3, in_pos);
IN(1, vec4, in_wgt);
IN(2, vec4, in_jnt);
IN(3, vec3, in_nrm);
IN(6, vec2, in_uv0);

out block
{
    vec4 posn;
    vec2 uv0;
    vec3 rgba0;
} sgl_out;

float3 DirFromLight = vec3(100, 100, 0);
float4 LightColour = vec4(0.9, 0.9, 0.9, 1);
float4 AmbientColour = vec4(0.1, 0.1, 0.1, 0.1);

void main()
{
    float BoneWeights[4] = (float[4])in_wgt;
    int BoneIndices[4]   = (int[4])in_jnt;

    float4 InPos     = float4( in_pos, 1 );
    float4 InNormal  = float4( in_nrm, 0 );
    float3 WorldPos    = 0;
    float3 WorldNormal = 0;

    for( int i = 0; i < 4; ++i )
    {
        float4x3 BoneMatrix = BoneMatrices[BoneIndices[i]];
        WorldPos    += BoneWeights[i] * (BoneMatrix * InPos);
        WorldNormal += BoneWeights[i] * (BoneMatrix * InNormal);
    }

    float3 ViewPos = vec3(WorldToView * float4(WorldPos, 1));
    float4 ClipPos = ViewToClip * float4(ViewPos, 1);
    sgl_out.posn   = ClipPos;

    sgl_out.uv0    = in_uv0;
    sgl_out.rgba0 = LightColour * dot(float4(normalize(WorldNormal),0), DirFromLight) + AmbientColour;

    gl_Position = ClipPos;
}
