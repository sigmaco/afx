
layout(std140) uniform sgl_mtl
{
    vec3  Kd;
    vec3  Ks;
    float Ns;
    float d;
    float Ni;
    int   illum;
};
