#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;
uniform float alpha = 1.0; 

void main()
{
    color = vec4(spriteColor, alpha) * texture(sprite, TexCoords);
}
