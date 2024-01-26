const char __FRAG_SHADER_SRC__ [] =
" \
#version 400 core \n\
 \n\
 \n\
in vec2 textCoord; \n\
 \n\
out vec4 fColor; \n\
 \n\
uniform sampler2D uActiveTexture; \n\
uniform float uAlfa; \n\
 \n\
 \n\
void main() { \n\
  fColor = texture(uActiveTexture, textCoord); \n\
  fColor.a = fColor.a * uAlfa; \n\
} \n\
";