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


void insert_shader(ShaderInstance const &instance,
                   std::string const &vertex_content,
                   std::string const &fragment_content) {
    
    std::string const new_vertex = insert_string(instance.vertex, vertex_content);
    std::string const new_fragment = insert_string(instance.fragment, fragment_content);

    load_shaders_content(new_vertex.c_str(), new_fragment.c_str());
}


std::string insert_string(std::string const &shader,
                          std::string const &content)
{
    std::istringstream stream(shader);
    std::string new_shader{};
    std::string line{};

    while (std::getline(stream, line))
    {
        if (line == INSERT_MARKER)
        {
            new_shader += content + "\n";
        }
        new_shader += line + "\n";
    }

    return new_shader;
}

