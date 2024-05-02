#include "menu.h"
#include <array>

Menu::Operation::Operation(int const &x, int const &y, std::string const &title,
                           Section *parent, bool close_button)
: Section(x, y, title, parent, close_button){};

std::string Menu::Operation::gen_var_name() const {
    int const LENGTH = 16;
    std::string const CHARSET =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, CHARSET.length() - 1);

    std::string result;
    for (int i = 0; i < LENGTH; ++i) {
        result += CHARSET[dis(gen)];
    }
    return result;
}

/* CREATE ROTATION SECTIONS */

void Menu::OperationMap::create(int id, Section *parent) {
    if (id == rotation_x) {
        Operation *op = new Rotation_X(0, 0, "Rotation_X", parent, true);
        parent->add_section(op);
    } else if (id == rotation_y) {
        Operation *op = new Rotation_Y(0, 0, "Rotation_Y", parent, true);
        parent->add_section(op);
    } else if (id == rotation_z) {
        Operation *op = new Rotation_Z(0, 0, "Rotation_Z", parent, true);
        parent->add_section(op);
    }
}

/* ROTATION */

Menu::Rotation::Rotation(int const &x, int const &y,
                             std::string const &title, Section *parent,
                             bool close_button)
: Operation(x, y, title, parent, close_button) {
    createSliderDisplay(&angle, "rotation: ", MIN, MAX, this);
};
std::string Menu::Rotation::s_get_variable() { 
    return "uniform mat4 " + angle_var + ";";
};

std::string Menu::Rotation::s_get_code() const {
    return "z = z * " + angle_var + ";";
};

void Menu::Rotation::upload(GLuint program) {
    glUniformMatrix4fv(glGetUniformLocation(program, angle_var.c_str()), 1, GL_TRUE, get_matrix().data());
};

/* ROTATION X */

Menu::Rotation_X::Rotation_X(int const &x, int const &y,
                             std::string const &title, Section *parent,
                             bool close_button)
: Rotation(x, y, title, parent, close_button) {
};

std::array<GLfloat, 16> Menu::Rotation_X::get_matrix() {
    return {
        1, 0, 0, 0,
        0, cos(angle), -sin(angle), 0,
        0, sin(angle), cos(angle), 0,
        0, 0, 0, 1
    };
};

/* ROTATION Y */

Menu::Rotation_Y::Rotation_Y(int const &x, int const &y,
                             std::string const &title, Section *parent,
                             bool close_button)
: Rotation(x, y, title, parent, close_button) {
};

std::array<GLfloat, 16> Menu::Rotation_Y::get_matrix() {
    return {
        cos(angle), 0, sin(angle), 0,
        0, 1, 0, 0,
        -sin(angle), 0, cos(angle), 0,
        0, 0, 0, 1
    };
};

/* ROTATION Z */

Menu::Rotation_Z::Rotation_Z(int const &x, int const &y,
                             std::string const &title, Section *parent,
                             bool close_button)
: Rotation(x, y, title, parent, close_button) {
};

std::array<GLfloat, 16> Menu::Rotation_Z::get_matrix() {
    return {
        cos(angle), -sin(angle), 0, 0,
        sin(angle), cos(angle), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
};

