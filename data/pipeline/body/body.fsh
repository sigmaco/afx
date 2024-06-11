#include <../data/pipeline/stdEnv.inc>
#include <../data/pipeline/stdView.inc>
#include <../data/pipeline/stdMaterial.inc>

UNIFORM(0, 0, sampler2D, diffuse_sampler);
        
in block
{
    vec3 xyz;
    vec3 nrm;
    vec2 uv0;
} sgl_v;

OUT(0, vec4, FragColor);

const vec3 lightPosition = vec3(10, 10, 10);
const vec3 lightAmbient = vec3(0.1, 0.1, 0.1);
const vec3 lightDiffuse = vec3(0.5, 0.5, 0.5);
const vec3 lightSpecular = vec3(0.5, 0.5, 0.5);

void main()
{
    // ambient
    vec3 ambient = lightAmbient;
    
    // diffuse 
    vec3 norm = normalize(sgl_v.nrm);
    vec3 lightDir = normalize(lightPosition - sgl_v.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * (diff * Kd);
    
    // specular
    vec3 viewDir = normalize(vec3(viewpoint.xyz) - sgl_v.xyz);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Ns);
    vec3 specular = lightSpecular * (spec * Ks);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
