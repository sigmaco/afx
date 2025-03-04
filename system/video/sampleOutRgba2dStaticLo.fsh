#include <//./z/video/stdEnv.inc>

TEXTURE(0, 0, sampler2D, samp);

uniform sampler2D tex;
uniform sampler2D noise_tex;
uniform float intensity;
uniform float time;
uniform vec2 resolution;

uniform vec2 block_size = vec2(16);
uniform vec2 noise_tex_size = vec2(64);

uniform int glitchAllBlocks = 0;
uniform int grayAllBlocks = 0;
uniform int interleaveAllLines = 0;

in block
{
    vec2 uv0;
} sgl_v;

OUT(0, vec4, sgl_rgba);

void main()
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    vec2 block = floor(gl_FragCoord.xy / block_size);
    vec2 uv_noise = block / noise_tex_size;
    uv_noise += floor(vec2(time) * vec2(1234.0, 3543.0)) / noise_tex_size;
    
    float block_thresh = pow(fract(time * 1236.0453), 2.0) * 0.2 * intensity;
    float line_thresh = pow(fract(time * 2236.0453), 3.0) * 0.7 * intensity;
    
    vec2 uv_r = uv;
    vec2 uv_g = uv;
    vec2 uv_b = uv;
    
    vec4 block_color = texture2D(noise_tex, uv_noise);
    vec4 line_color  = texture2D(noise_tex, vec2(uv_noise.y, 0.0));

    // glitch some blocks and lines
    if (block_color.r < block_thresh || line_color.g < line_thresh || glitchAllBlocks != 0)
    {
        vec2 dist = (fract(uv_noise) - 0.5) * 0.3;
        uv_r += dist * 0.1;
        uv_g += dist * 0.2;
        uv_b += dist * 0.125;
    }

    vec4 fragColor = texture2D(tex, uv);
    fragColor.r = texture2D(tex, uv_r).r;
    fragColor.g = texture2D(tex, uv_g).g;
    fragColor.b = texture2D(tex, uv_b).b;

    // loose luma for some blocks
    if (block_color.g < block_thresh || grayAllBlocks != 0)
        fragColor.rgb = fragColor.ggg;

    // discolor block lines
    if (line_color.b * 3.5 < line_thresh)
        fragColor.rgb = vec3(0.0, dot(fragColor.rgb, vec3(1.0)), 0.0);

    // interleave lines in some blocks
    if (block_color.g * 1.5 < block_thresh || line_color.g * 2.5 < line_thresh || interleaveAllLines != 0)
    {
        float line = fract(gl_FragCoord.y / 3.0);
        vec3 mask = vec3(3.0, 0.0, 0.0);
        if (line > 0.333)
            mask = vec3(0.0, 3.0, 0.0);
        if (line > 0.666)
            mask = vec3(0.0, 0.0, 3.0);
        
        fragColor.xyz *= mask;
    }
    gl_FragColor = fragColor;
}
