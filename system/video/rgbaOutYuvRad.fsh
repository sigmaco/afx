#include <../system/video/stdEnv.inc>

//#define ALPHA // enable auxiliar texture for alpha mask.
const vec4 crc = vec4( 1.595794678f , -0.813476563f , 0, 0.0 );
const vec4 crb = vec4( 0, -0.391448975f , 2.017822266f, 0.0 );
const vec4 adj = vec4( -0.87065506f, 0.529705048f, -1.081668854f, 0 );

SAMPLING_UNIT(0, 0, sampler2D, samp0);
SAMPLING_UNIT(0, 1, sampler2D, samp1);
SAMPLING_UNIT(0, 2, sampler2D, samp2);
#ifdef ALPHA
SAMPLING_UNIT(0, 3, sampler2D, samp3);
uniform vec4 consta;
#endif
vec4 consta = vec4(1.0); // gambiarra do Qwadro para evitar um constant buffer aqui nos testes.

in block
{
    vec2 uv0;
} sgl_v;

OUT(0, vec4, sgl_rgba);

void main()
{
    // method from Bink SDK, by RAD using DX Pixel Shader 2.0, adapted to Qwadro by SIGMA.
    vec4 p;

    float y = texture(samp0, sgl_v.uv0).r;
    float cr = texture(samp1, sgl_v.uv0).r;
    float cb = texture(samp2, sgl_v.uv0).r;
#   ifdef ALPHA
    float a = texture(samp3, sgl_v.uv0)).r;
#   endif

    p = vec4(y * 1.164123535f );
    p += crc * cr;
    p += crb * cb;
    p += adj;
    
#ifdef ALPHA
    p.w = a;
#else
    p.w = 1.0;
#endif
    p *= consta;
    sgl_rgba = p;
}
