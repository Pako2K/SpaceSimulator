const char __FRAG_SHADER_SRC__ [] =
" \
#version 400 core \n\
 \n\
uniform vec4 uColor; \n\
 \n\
out vec4 fColor; \n\
 \n\
void main() { \n\
	fColor = uColor; \n\
} \n\
";