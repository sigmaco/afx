#include <//./z/video/stdEnv.inc>

UNIFORM_UNIT(0, 0, buffer)
{
    vec2 viewSize;
};

IN(0, vec2, vertex);
IN(1, vec2, tcoord);

out block
{
    vec2 ftcoord;
    vec2 fpos;
} sgl_v;

void main(void)
{
	sgl_v.ftcoord = tcoord;
	sgl_v.fpos = vertex;
	//gl_Position = vec4(2.0*vertex.x/viewSize.x - 1.0, 2.0*vertex.y/viewSize.y - 1.0, 0, 1); // vk [0,1]
    gl_Position = vec4(2.0*vertex.x/viewSize.x - 1.0, 1.0 - 2.0*vertex.y/viewSize.y, -1, 1); // gl [-1,1]
}
