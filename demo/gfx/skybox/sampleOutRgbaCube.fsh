// (c) 2017 SIGMA Technology Group

TEXTURE(0, 1, samplerCube, samp);

in block
{
    vec3 uvw0;
} sgl_v;

OUT(0, vec4, sgl_rgba);

void main()
{
    sgl_rgba = texture(samp, sgl_v.uvw0);
}
