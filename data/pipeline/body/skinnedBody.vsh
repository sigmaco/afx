#include <data/pipeline/stdEnv.inc>
#include <data/pipeline/stdView.inc>
#include <data/pipeline/stdObject.inc>

IN(0, vec3, aPos);
IN(1, vec4, aBlendIndices);
IN(2, vec4, aBlendWeights);
IN(3, vec3, aNormal);
IN(6, vec2, aUv);

out block
{
    vec3 Position;
    vec4 Diffuse;
    vec2 Tex0;
} sgl_v;

const vec4 AmbientColour = vec4(0.2, 0.2, 0.2, 1.0);

void main()
{
    vec3 InPos     = aPos;
    vec4 WorldPos    = 0;
    vec3 WorldNormal = 0;
    
    // We're going to assume that we can transform the normal by the matrix, which is not
    // technically correct unless we're positive there's not a scaling factor involved.

    for( int i = 0; i < 4; ++i )
    {
        mat4 BoneMatrix = w[int(aBlendIndices[i])];
        WorldPos    += (aBlendWeights[i] * (BoneMatrix * vec4(InPos, 1)));
        WorldNormal += vec3(vec4(aBlendWeights[i] * (BoneMatrix * vec4(aNormal, 0))));
    }

    vec4 ViewPosL = (WorldPos * v);
    vec4 ClipPos = (ViewPosL * p);
    gl_Position = ClipPos;
    sgl_v.Position   = vec3(ClipPos);
    sgl_v.Tex0    = aUv;
    sgl_v.Diffuse = starKd * (dot(normalize(WorldNormal), starDir)) + AmbientColour;
}
