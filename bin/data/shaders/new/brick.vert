/* Set the default floating point precision */
precision mediump float;

uniform vec3 LightPosition;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
attribute vec4 coord;
attribute vec3 normal;

const float SpecularContribution = 0.3;
const float DiffuseContribution  = 1.0 - SpecularContribution;

varying float LightIntensity;
varying vec2  MCposition;

void main()
{
    vec3 ecPosition = vec3(ModelViewMatrix * coord);
    vec3 tnorm      = normalize(NormalMatrix * normal);
    vec3 lightVec   = normalize(LightPosition - ecPosition);
    vec3 reflectVec = reflect(-lightVec, tnorm);
    vec3 viewVec    = normalize(-ecPosition);
    float diffuse   = max(dot(lightVec, tnorm), 0.0);
    float spec      = 0.0;

    if (diffuse > 0.0)
    {
        spec = max(dot(reflectVec, viewVec), 0.0);
        spec = pow(spec, 16.0);
    }

    LightIntensity  = DiffuseContribution * diffuse +
                      SpecularContribution * spec;

    MCposition      = (coord).xy;
    gl_Position     = 	ModelViewMatrix*coord;
}

