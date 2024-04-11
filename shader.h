#include <GL/gl.h>
#include <string>


std::string const INSERT_MARKER = "/* INSERT */";


struct ShaderInstance {
    GLuint program;
    std::string vertex;
    std::string fragment;
};


ShaderInstance load_shaders(char const *vertex_path, char const *fragment_path);
ShaderInstance load_shaders_content(char const *vertex_content, char const *fragment_content);
void insert_shader(ShaderInstance const &instance, std::string const &vertex_content, std::string const &fragment_content);
std::string insert_string(std::string const &shader, std::string const &content);
