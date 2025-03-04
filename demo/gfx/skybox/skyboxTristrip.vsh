// (c) 2017 SIGMA Technology Group

const vec3 pos[14] =
{
    vec3(-1, 1, 1),
    vec3( 1, 1, 1),
    vec3(-1,-1, 1),
    vec3( 1,-1, 1),
    vec3( 1,-1,-1),
    vec3( 1, 1, 1),
    vec3( 1, 1,-1),
    vec3(-1, 1, 1),
    vec3(-1, 1,-1),
    vec3(-1,-1, 1),
    vec3(-1,-1,-1),
    vec3( 1,-1,-1),
    vec3(-1, 1,-1),
    vec3( 1, 1,-1)
};

const vec2 uv[14] =
{
    vec2(0, 0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1, 1),
    vec2(1, 0),
    vec2(0, 1),
    vec2(0, 0),
    vec2(1, 1),
    vec2(1, 0),
    vec2(0, 1),
    vec2(0, 0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1, 1)
};

out block
{
    vec2 uv0;
} sgl_v;

// to use, draw 12 triangles in TRI_STRIP mode.

void main()
{
    sgl_v.uv0 = uv[gl_VertexID];            
    gl_Position = pos[gl_VertexID];
}
