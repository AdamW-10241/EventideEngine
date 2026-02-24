#version 460 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D sprite;
uniform vec4 color;
uniform bool useTexture;

void main() {
    if (useTexture) {
        FragColor = texture(sprite, TexCoord) * color;
    } else {
        FragColor = color;
    }
}