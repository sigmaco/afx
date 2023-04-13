#version 330 core

layout(location = 0, std140, row_major) uniform sgl0
{
    mat4 sgl_p;
    mat4 sgl_v;
    mat4 sgl_m;
    mat3 sgl_mvit;
};

layout(location = 0) in vec4 a_xyzw;
layout(location = 1) in vec3 a_nrm;
layout(location = 2) in vec2 a_uv;

out block
{
    vec3 xyz;
    vec3 nrm;
    vec2 uv;
}
sgl_out;

void main()
{
    vec4 xyzw = sgl_v * sgl_m * a_xyzw;
    sgl_out.xyz = -vec3(xyzw);
    sgl_out.nrm = sgl_mvit * a_nrm;
    sgl_out.uv = a_uv;
    gl_Position = sgl_p * xyzw;
}
