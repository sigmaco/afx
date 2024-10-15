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

#define IN(_location_, _type_, _name_) layout(location = _location_) in _type_ _name_ 
#define OUT(_location_, _type_, _name_) layout(location = _location_) out _type_ _name_ 
//#define UNIFORM(_set_, _binding_, _type_, _name_) uniform _type_ _name_ 
//#define MULTIFORM(_set_, _binding_, _type_, _name_, _count_) _type_ _name_[_count_] 

// ubo = std140, ssbo = std430
#define UNIFORM_UNIT(_set_, _binding_, _name_) layout(std140) uniform _name_ 
#define STORAGE_UNIT(_set_, _binding_, _access_, _name_) layout(std430) _access_ buffer _name_ 
#define SAMPLING_UNIT(_set_, _binding_, _type_, _name_) uniform _type_ _name_ 
#define PUSH_UNIT() layout(std140) uniform PushConstants 
