#include <//./z/video/stdEnv.inc>

const vec4 gsQuadPos[4] = vec4[](vec4(-1, 1, 0, 1), vec4(-1,-1, 0, 1), vec4(1, 1, 0, 1), vec4(1,-1, 0, 1));

void main()
{
    // draw a full coverage quad based on two tristripped triangles (4 indices). AfxDraw(0, 1, 0, 4)    
    gl_Position = gsQuadPos[gl_VertexID];
}
