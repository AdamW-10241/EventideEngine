#version 460 core

in vec3 fColour;
in vec2 fTexCoords;

uniform sampler2D colourMap;

out vec4 finalColour;

void main() {
	finalColour = texture(colourMap, fTexCoords) * vec4(fColour, 1.0);
}