#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;
uniform float alpha = 1.0f;

void main()
{
    vec3 highlightColor = vec3(1.0f, 0.0f, 0.0f);  // Red color

    if (TexCoords.x < 0.5f) {  // Compare x component of TexCoords to 0.5
        color = vec4(highlightColor, alpha) * texture(sprite, TexCoords);
    } else {
        color = vec4(spriteColor, alpha) * texture(sprite, TexCoords);
    }
}
