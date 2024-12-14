#version 460

in vec2 fragTextureCoordinates;

out vec4 fragColor;

layout(binding=0) uniform sampler2D myTexture;

void main()
{
    fragColor = texture2D(myTexture, fragTextureCoordinates);
}