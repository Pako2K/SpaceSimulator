#version 400 core


in vec2 textCoord;

out vec4 fColor;

uniform sampler2D uActiveTexture;
uniform float uAlfa;


void main() {
  fColor = texture(uActiveTexture, textCoord);
  fColor.a = fColor.a * uAlfa;
}
