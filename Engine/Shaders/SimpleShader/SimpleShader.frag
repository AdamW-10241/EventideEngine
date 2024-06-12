#version 460 core

in vec3 fColour;

out vec4 finalColour;

void main() {
	finalColour = vec4(fColour, 1.0);
}