#include "menu.h"
#include <random>

Menu::Operation::Operation(int const &x, int const &y, std::string const &title,
                           Section *parent, bool close_button)
: Section(x, y, title, parent, close_button){};

std::string Menu::Operation::gen_var_name() const {
    int const LENGTH = 16;
    std::string const CHARSET =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
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
    } else if (id == box_fold) {
        Operation *op = new BoxFold(0, 0, "BoxFold", parent, true);
        parent->add_section(op);
    } else if (id == sphere_fold) {
        Operation *op = new SphereFold(0, 0, "SphereFold", parent, true);
        parent->add_section(op);
    } else if (id == sierpinski_fold) {
        Operation *op = new SierpinskiFold(0, 0, "SierpinskiFold", parent, true);
        parent->add_section(op);
    } else if (id == octahedral_fold) {
        Operation *op = new OctahedralFold(0, 0, "OctahedralFold", parent, true);
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

/* BOX FOLD */

Menu::BoxFold::BoxFold(int const &x, int const &y,
                             std::string const &title, Section *parent,
                             bool close_button)
: Operation(x, y, title, parent, close_button) {
    createSliderDisplay(&fold_limit, "fold limit: ", MIN, MAX, this);
}

std::string Menu::BoxFold::s_get_variable() {
    return "uniform float " + fold_limit_var + ";"; 
}
std::string Menu::BoxFold::s_get_code() const {
    return "box_fold(z, " + fold_limit_var + ");";

}
void Menu::BoxFold::upload(GLuint program) {
    glUniform1f(glGetUniformLocation(program, fold_limit_var.c_str()), fold_limit);
}

/* SPHERE FOLD */

Menu::SphereFold::SphereFold(int const &x, int const &y,
                             std::string const &title, Section *parent,
                             bool close_button)
: Operation(x, y, title, parent, close_button) {
    createSliderDisplay(&fixed_radius, "fixed radius: ", MIN, MAX, this);
}

std::string Menu::SphereFold::s_get_variable() {
    return "uniform float " + fixed_radius_var + ";"; 
}
std::string Menu::SphereFold::s_get_code() const {
    return "sphere_fold(z, dr, " + fixed_radius_var + ");";

}
void Menu::SphereFold::upload(GLuint program) {
    glUniform1f(glGetUniformLocation(program, fixed_radius_var.c_str()), fixed_radius);
}

/* SIERPINSKI FOLD */

Menu::SierpinskiFold::SierpinskiFold(int const &x, int const &y,
                             std::string const &title, Section *parent,
                             bool close_button)
: Operation(x, y, title, parent, close_button) {
    createSliderDisplay(&fold_limit, "fold limit: ", MIN, MAX, this);
}

std::string Menu::SierpinskiFold::s_get_variable() {
    return "uniform float " + fold_limit_var + ";"; 
}
std::string Menu::SierpinskiFold::s_get_code() const {
    return "sierpinski_fold(z, " + fold_limit_var + ");";

}
void Menu::SierpinskiFold::upload(GLuint program) {
    glUniform1f(glGetUniformLocation(program, fold_limit_var.c_str()), fold_limit);
}

/* OCTAHEDRAL FOLD */

Menu::OctahedralFold::OctahedralFold(int const &x, int const &y,
                             std::string const &title, Section *parent,
                             bool close_button)
: Operation(x, y, title, parent, close_button) {
    createSliderDisplay(&fold_limit, "fold limit: ", MIN, MAX, this);
}

std::string Menu::OctahedralFold::s_get_variable() {
    return "uniform float " + fold_limit_var + ";"; 
}
std::string Menu::OctahedralFold::s_get_code() const {
    return "octahedral_symmetry_fold(z, " + fold_limit_var + ");";

}
void Menu::OctahedralFold::upload(GLuint program) {
    glUniform1f(glGetUniformLocation(program, fold_limit_var.c_str()), fold_limit);
}

