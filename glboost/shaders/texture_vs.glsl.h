const char __VERT_SHADER_SRC__ [] =
" \
#version 400 core \n\
 \n\
in vec4 vPosition; \n\
in vec2 vTexCoord; \n\
 \n\
out vec2 textCoord; \n\
 \n\
void main() { \n\
	gl_Position = vPosition; \n\
	textCoord = vTexCoord; \n\
} \n\
";