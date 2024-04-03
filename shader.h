#include <GL/gl.h>


struct ShaderInstance {
    GLuint program;
    char const *vertex;
    char const *fragment;
};


ShaderInstance load_shaders(char const *vertex_path, char const *fragment_path);
