#include <//./z/video/stdEnv.inc>

//UNIFORM_UNIT(0, 0, buffer)
PUSHABLE(buffer)
{
    mat4 p;
};

IN(0, vec2, aXy);
IN(1, vec2, aUv);
IN(2, vec4, aRgba);

out block
{
    vec2 vUv;
    vec4 vRgba;
} sgl_v;

void main(void)
{
	sgl_v.vUv = aUv;
    sgl_v.vRgba = aRgba;
    gl_Position = p * vec4(aXy.xy, 0, 1);
}
