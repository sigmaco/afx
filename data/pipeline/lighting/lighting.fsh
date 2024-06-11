#include <../data/pipeline/stdEnv.inc>
#include <../data/pipeline/stdView.inc>
#include <../data/pipeline/stdMaterial.inc>

SAMPLING_UNIT(2, 1, sampler2D, floorTexture);

vec3 lightPos = vec3(100, 100, 0);
int blinn = 1;

in block
{
    vec4 posn;
    vec3 nrm;
    vec2 uv0;
} sgl_in;

OUT(0, vec4, out_rgba0);

void main (void)
{ 
    vec3 color = vec3(0.5,0.5,0.5);//texture(floorTexture, sgl_in.uv0).rgb;
    
    // ambient
    vec3 ambient = 0.05 * color;
    
    // diffuse
    vec3 lightDir = normalize(lightPos - vec3(sgl_in.posn));
    vec3 normal = normalize(sgl_in.nrm);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    
    // specular
    vec3 viewDir = vec3(normalize(viewPos - sgl_in.posn));
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    
    if(blinn == 1)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    
    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    out_rgba0 = vec4(ambient + diffuse + specular, 1.0);
}
