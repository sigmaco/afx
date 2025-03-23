
#if 0
layout(constant_id = 0) const uint EDGE_AA = 0;
#else
const uint EDGE_AA = 0;
#endif
struct FragmentData {
    mat3 scissorMat;
    mat3 paintMat;
    vec4 innerCol;
    vec4 outerCol;
    vec2 scissorExt;
    vec2 scissorScale;
    vec2 extent;
    float radius;
    float feather;
    float strokeMult;
    float strokeThr;
    int texType;
    int type;
};
UNIFORM(0, 1, std140, FragmentBuffer) {
    FragmentData data[320];
};
TEXTURE(0, 2, sampler2D, tex);

in block
{
    vec2 ftcoord;
    vec2 fpos;
    flat int fid;
} sgl_v;
OUT(0, vec4, outColor);

float sdroundrect(vec2 pt, vec2 ext, float rad) {
    vec2 ext2 = ext - vec2(rad, rad);
    vec2 d = abs(pt) - ext2;
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - rad;
}

// Scissoring
float scissorMask(vec2 p) {
    vec2 sc = (abs((data[sgl_v.fid].scissorMat * vec3(p, 1.0)).xy) - data[sgl_v.fid].scissorExt);
    sc = vec2(0.5, 0.5) - sc * data[sgl_v.fid].scissorScale;
    return clamp(sc.x, 0.0, 1.0) * clamp(sc.y, 0.0, 1.0);
}

// Stroke - from [0..1] to clipped pyramid, where the slope is 1px.
float strokeMask() {
    return min(1.0, (1.0-abs(sgl_v.ftcoord.x*2.0-1.0))*data[sgl_v.fid].strokeMult) * min(1.0, sgl_v.ftcoord.y);
}

void main(void) {
    vec4 result;
    float scissor = scissorMask(sgl_v.fpos);
    float strokeAlpha = 1.0;
    if (EDGE_AA == 1) {
        strokeAlpha = strokeMask();
        if (strokeAlpha < data[sgl_v.fid].strokeThr) discard;
    }
    if (data[sgl_v.fid].type == 0) { // Gradient
        // Calculate gradient color using box gradient
        vec2 pt = (data[sgl_v.fid].paintMat * vec3(sgl_v.fpos, 1.0)).xy;
        float d = clamp((sdroundrect(pt, data[sgl_v.fid].extent, data[sgl_v.fid].radius) + data[sgl_v.fid].feather*0.5) / data[sgl_v.fid].feather, 0.0, 1.0);
        vec4 color = mix(data[sgl_v.fid].innerCol, data[sgl_v.fid].outerCol, d);
        // Combine alpha
        color *= strokeAlpha * scissor;
        result = color;
    } else if (data[sgl_v.fid].type == 1) { // Image
        // Calculate color fron texture
        vec2 pt = (data[sgl_v.fid].paintMat * vec3(sgl_v.fpos, 1.0)).xy / data[sgl_v.fid].extent;
        vec4 color = texture(tex, pt);
        if (data[sgl_v.fid].texType == 1) color = vec4(color.xyz*color.w, color.w);
        if (data[sgl_v.fid].texType == 2) color = vec4(color.x);
        // Apply color tint and alpha.
        color *= data[sgl_v.fid].innerCol;
        // Combine alpha
        color *= strokeAlpha * scissor;
        result = color;
    } else if (data[sgl_v.fid].type == 2) { // Stencil fill
        result = vec4(1, 1, 1, 1);
    } else if (data[sgl_v.fid].type == 3) { // Textured tris
        vec4 color = texture(tex, sgl_v.ftcoord);
        if (data[sgl_v.fid].texType == 1) color = vec4(color.xyz*color.w, color.w);
        if (data[sgl_v.fid].texType == 2) color = vec4(color.x);
        color *= scissor;
        result = color * data[sgl_v.fid].innerCol;
    }
    outColor = result;
}
