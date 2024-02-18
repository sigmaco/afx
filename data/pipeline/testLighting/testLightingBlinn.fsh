#include <data/pipeline/stdEnv.inc>
#include <data/pipeline/stdView.inc>
#include <data/pipeline/stdMaterial.inc>

in block
{
    vec4 posn;
    vec3 nrm;
} sgl_in;

OUT(0, vec4, out_rgba0);

vec4 litSrc0Pos = vec4(100, 1000, 0, 1);
vec4 litProd0Ambi = vec4(0.1, 0.1, 0.1, 1);
vec4 litProd0Diff = vec4(0.9, 0.8, 0.7, 1);
vec4 litProd0Spec = vec4(0.1, 0.1, 0.1, 1);
vec4 litModelAmbi = vec4(1, 1, 1, 1);
vec4 mtlAmbi = vec4(0.1, 0.1, 0.1, 1);
float mltShinisses = 10;

void main (void)
{ 
    // calculate light diffusion
    vec3 light = normalize(litSrc0Pos.xyz - sgl_in.posn.xyz); 
    vec3 view = normalize(-sgl_in.posn.xyz); 
    vec3 normal = normalize(sgl_in.nrm); 

    // calculate ambience
    vec4 ambient = litProd0Ambi;
    vec4 global = mtlAmbi * litModelAmbi;

    // get vector to light
    vec4 diffuse = litProd0Diff * max(dot(sgl_in.nrm, light), 0.0);
    diffuse = clamp(diffuse, 0.0, 1.0);

    // Blinn specular variation
    vec3 halfDir = normalize(light + view);
    float spec = pow(max(dot(normal, halfDir), 0.0), mltShinisses * 0.2);
    vec4 specular = litProd0Spec * spec;
    specular = clamp(specular, 0.0, 1.0);

    // do calculations on every fragment
    out_rgba0 = global + ambient + diffuse + specular;
}
