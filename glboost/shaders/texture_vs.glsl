#version 400 core

in vec4 vPosition;
in vec2 vTexCoord;

out vec2 textCoord;

void main() {
	gl_Position = vPosition;
	textCoord = vTexCoord;
}
