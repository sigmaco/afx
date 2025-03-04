// (c) 2017 SIGMA Technology Group

PUSH(pushes)
{
    mat4 p;
    mat4 v;
};

IN(0, vec3, a_xyz);

out block
{
    vec3 uvw0;
} sgl_v;

void main()
{
    sgl_v.uvw0 = a_xyz;
    
    // Remove translation from view matrix
    vec4 pos = p * mat4(mat3(v)) * vec4(a_xyz, 1.0);
    
    gl_Position = pos.xyww;
}
