#include <//./z/video/stdEnv.inc>

const vec4 gsTriPos[3] = vec4[](vec4(-1, -1, 0, 1), vec4(3,-1, 0, 1), vec4(-1, 3, 0, 1));

void main()
{
    // draw a full coverage triangle (3 indices). AfxDraw(0, 1, 0, 3)
    gl_Position = gsTriPos[gl_VertexID];
}
