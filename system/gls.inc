#version 330 core // QWADRO. Engineered by SIGMA Technology Group.

#define afxInt int
#define afxNat uint

#define afxReal float

#define afxV2d vec2
#define afxV3d vec3
#define afxV4d vec4

#define afxM2d mat2
#define afxM3d mat3
#define afxM4d mat4

#define float2 vec2
#define float3 vec3
#define float4 vec4

#define IN(_at_, _type_, _name_) layout(location = _at_) in _type_ _name_ 
#define OUT(_at_, _type_, _name_) layout(location = _at_) out _type_ _name_ 
// enforce early tests
#define IN_EARLY_FRAGMENT_TESTS() layout(early_fragment_tests) in 
//#define UNIFORM(_set_, _binding_, _type_, _name_) uniform _type_ _name_ 
//#define MULTIFORM(_set_, _binding_, _type_, _name_, _count_) _type_ _name_[_count_] 

#define IN_CANVAS_COORD() in vec4 gl_FragCoord 
#define IN_WINDOW_COORD() layout(origin_upper_left) in vec4 gl_FragCoord 

// ubo = std140, ssbo = std430
#define UNIFORM(_set_, _binding_, _name_) layout(std140) uniform _name_ 
#define BUFFER(_set_, _binding_, _access_, _name_) layout(std430) _access_ buffer _name_ 
#define SAMPLER(_set_, _binding_, _type_, _name_) uniform _type_ _name_ 
#define TEXTURE(_set_, _binding_, _type_, _name_) uniform _type_ _name_ 
#define RASTER(_set_, _binding_, _type_, _name_) uniform _type_ _name_ 
#define PUSH() layout(std140) uniform PushConstants 
#define SPECIALIZED(_specid_, _type_, _name_) layout(_specid_) const _type_ _name_ 
#define FETCH(_set_, _binding_, _name_) usamplerBuffer _name_ 

#define fetch(_bufName_, _elemIdx_) texelFetch(_bufName_, _elemIdx_) 



struct DrawArraysIndirect
{
    uint vtxCnt;
    uint instCnt;
    uint baseVtx;
    uint baseInst;
    // Optional user-defined data goes here - if nothing, stride is 0
};
// sizeof(DrawArraysIndirect) == 16

// Struct for MultiDrawElements
struct DrawElementsIndirect
{
    uint idxCnt;
    uint instaCnt;
    uint baseIdx;
    int  baseVtx;
    uint baseInst;
    // Optional user-defined data goes here - if nothing, stride is 0
};
// sizeof(DrawElementsIndirect) == 20
