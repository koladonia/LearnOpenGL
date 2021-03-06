#version 460 core
layout (binding = 0) uniform sampler2D screenTexture;
layout (binding = 1) uniform sampler2D bloomTexture;

layout (location = 0) in vec2 texCoord;

layout (location = 0) uniform int bloomBool;

layout (location = 0) out vec4 fragColor;

void main()
{
    const float gamma = 2.2;
    const float exposure = 1.0;
    vec3 hdrColor = texture(screenTexture, texCoord).rgb + texture(bloomTexture, texCoord).rgb * bloomBool;
    //Reinhard
    //vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));

    fragColor = vec4(mapped, 1.0);
}