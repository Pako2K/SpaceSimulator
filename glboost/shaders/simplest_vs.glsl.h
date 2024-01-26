const char __VERT_SHADER_SRC__ [] =
" \
#version 400 core \n\
 \n\
in vec4 vPosition; \n\
 \n\
void main() { \n\
	gl_Position = vPosition; \n\
} \n\
";