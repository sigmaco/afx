#include <../system/video/stdEnv.inc>

const vec4 gsTriPos[3] = vec4[](vec4(-1, -1, 0, 1), vec4(3,-1, 0, 1), vec4(-1, 3, 0, 1));
const vec2 gsTriUv[3] = vec2[](vec2(0, 0), vec2(2, 0), vec2(0, 2));

out block
{
    vec2 uv0;
} sgl_v;

void main()
{
    // draw a full coverage triangle (3 indices). AfxDraw(0, 1, 0, 3)
    gl_Position = gsTriPos[gl_VertexID];
    sgl_v.uv0 = gsTriUv[gl_VertexID];
}
