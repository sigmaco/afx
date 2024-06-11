#include <../data/pipeline/stdEnv.inc>
#include <../data/pipeline/stdView.inc>
#include <../data/pipeline/stdObject.inc>

IN(0, vec3, in_pos);
IN(3, vec3, in_nrm);
IN(6, vec2, in_uv0);

out block
{
    vec2 uv0;
    vec3 rgba0;
} sgl_out;

float3 DirFromLight = vec3(100, 100, 0);
float4 LightColour = vec4(0.9, 0.9, 0.9, 1);
float4 AmbientColour = vec4(0.1, 0.1, 0.1, 0.1);

void main()
{
    float4 WorldPos = m * vec4(in_pos, 1);
    float4 ViewPos  = v * WorldPos;
    float4 ClipPos  = p * ViewPos;

    float4 InNormal  = float4( in_nrm, 0);
    float3 ObjNormal = vec4(m * InNormal).xyz;

    sgl_out.uv0     = in_uv0;
    sgl_out.rgba0  = vec3(LightColour * dot(vec4(ObjNormal,0), vec4(DirFromLight,0)) + AmbientColour);

    gl_Position = ClipPos;
}
