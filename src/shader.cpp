#include "shader.h"
#include "GL_utilities.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>
#include <sstream>

ShaderInstance load_shaders(char const *vertex_path,
                            char const *fragment_path) {

    // I have no idea why readFile doesn't take const. Kinda cursed
    char const *v_content = readFile(const_cast<char *>(vertex_path));
    char const *f_content = readFile(const_cast<char *>(fragment_path));

    return load_shaders_content(v_content, f_content);
}

ShaderInstance load_shaders_content(char const *v_content,
                                    char const *f_content) {

    ShaderInstance instance = {glCreateProgram(), v_content, f_content};

    GLuint const vertex_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint const shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    instance.vertex_id = vertex_id;
    instance.fragment_id = shader_id;

    // Active the vertex shader
    glShaderSource(vertex_id, 1, &v_content, NULL);
    glCompileShader(vertex_id);
    glAttachShader(instance.program, vertex_id);

    // Active the fragment shader
    glShaderSource(shader_id, 1, &f_content, NULL);
    glCompileShader(shader_id);
    glAttachShader(instance.program, shader_id);

    glLinkProgram(instance.program);
    glUseProgram(instance.program);

    return instance;
}

void swap_shaders(ShaderInstance &instance, std::string const &v_content, std::string const &f_content) {
    GLuint const vertex_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint const shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    char const *v_content_c = v_content.c_str();
    char const *f_content_c = f_content.c_str();

    // Active the vertex shader
    glShaderSource(vertex_id, 1, &v_content_c, NULL);
    glCompileShader(vertex_id);

    // Active the fragment shader
    glShaderSource(shader_id, 1, &f_content_c, NULL);
    glCompileShader(shader_id);

    glDetachShader(instance.program, instance.vertex_id);
    glDetachShader(instance.program, instance.fragment_id);

    glAttachShader(instance.program, vertex_id);
    glAttachShader(instance.program, shader_id);

    glLinkProgram(instance.program);
    glUseProgram(instance.program);

    instance.vertex_id = vertex_id;
    instance.fragment_id = shader_id;
}

void insert_shader(ShaderInstance &instance,
                   std::string const &fragment_input,
                   std::string const &fragment_code) {

    std::string const new_fragment =
        insert_string(instance.fragment, fragment_input, fragment_code);

    swap_shaders(instance, instance.vertex, new_fragment);
}

std::string insert_string(std::string const &shader, std::string const &input,
                          std::string const &code) {
    std::istringstream stream(shader);
    std::string new_shader{};
    std::string line{};

    while (std::getline(stream, line)) {
        // Trim the leading and trailing whitespace
        int const start = line.find_first_not_of(" \t");
        int const end = line.find_last_not_of(" \t");

        if (start == std::string::npos || end == std::string::npos) {
            continue;
        }

        std::string const trimmed_line = line.substr(start, end - start + 1);

        if (trimmed_line == INPUT_INSERT_MARKER) {
            new_shader += input + "\n";
        }

        if (trimmed_line == CODE_INSERT_MARKER) {
            new_shader += code + "\n";
        }
        new_shader += line + "\n";
    }

    return new_shader;
}
