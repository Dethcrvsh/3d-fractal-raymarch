#include "menu.h"

std::string Menu::Parameters::get_variables() {
    has_changed = false;
    std::string vars{};

    for (Operation *op : ops) {
        vars += op->s_get_variable() + "\n";
    }
    return vars;
}

std::string Menu::Parameters::get_code() {
    has_changed = false;
    std::string code{};

    for (Operation *op : ops) {
        code += op->s_get_code() + "\n";
    }
    return code;
}

void Menu::Parameters::upload(GLuint program) {
    // Distance Estimator
    glUniform1f(glGetUniformLocation(program, "in_Scale"), scale);
    glUniform3f(glGetUniformLocation(program, "in_Offset"), offset.x, offset.y,
                offset.z);
    glUniform1i(glGetUniformLocation(program, "in_DistEstIter"),
                static_cast<int>(dist_iterations));

    // Ray March
    glUniform1f(glGetUniformLocation(program, "in_RayMarchMinDist"), min_dist);
    glUniform1i(glGetUniformLocation(program, "in_RayMarchIterations"),
                static_cast<int>(ray_iterations));

    for (Operation *op : ops) {
        op->upload(program);
    }
};
