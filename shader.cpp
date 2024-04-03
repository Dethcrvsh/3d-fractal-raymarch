#include <GL/gl.h>
#include "GL_utilities.h"
#include <GL/glext.h>
#include "shader.h"
#include <iostream>


ShaderInstance load_shaders(char const *vertex_path, char const *fragment_path) {
    // I have no idea why readFile doesn't take const. Kinda cursed
    char const *v_content = readFile(const_cast<char*>(vertex_path));
    char const *f_content = readFile(const_cast<char*>(fragment_path));

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

