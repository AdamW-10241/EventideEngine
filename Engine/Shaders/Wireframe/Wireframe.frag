#version 460 core

uniform vec3 wireColour = vec3(1.0f);

out vec4 finalColour;

void main() {
	finalColour = vec4(wireColour, 1.0f);
}