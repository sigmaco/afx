
PUSH(VertexUniform)
{
    vec2 viewSize;
    int uniformOffset;
};

IN(0, vec2, vertex);
IN(1, vec2, tcoord);
out block
{
    vec2 ftcoord;
    vec2 fpos;
    flat int fid;
} sgl_v;

void main(void)
{
    sgl_v.ftcoord = tcoord;
    sgl_v.fpos = vertex;
    sgl_v.fid = uniformOffset;
    sgl_v.fid = 0;
    //gl_Position = vec4(2.0*vertex.x/viewSize.x - 1.0, 2.0*vertex.y/viewSize.y - 1.0, 0, 1); // VK
    gl_Position = vec4(2.0*vertex.x/viewSize.x - 1.0, 1.0 - 2.0*vertex.y/viewSize.y, 0, 1); // GL
}
