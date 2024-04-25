#include <GL/gl.h>
#include <string>


std::string const CODE_INSERT_MARKER = "/* INSERT CODE */";
std::string const INPUT_INSERT_MARKER = "/* INSERT INPUT */";

struct ShaderInstance {
    GLuint program;
    std::string vertex;
    std::string fragment;
    GLuint vertex_id;
    GLuint fragment_id;
};


ShaderInstance load_shaders(char const *vertex_path, char const *fragment_path);
ShaderInstance load_shaders_content(char const *vertex_content, char const *fragment_content);
void insert_shader(ShaderInstance &instance, std::string const &fragment_input, std::string const &fragment_code);
std::string insert_string(std::string const &shader, std::string const &fragment_input, std::string const &fragment_code);
