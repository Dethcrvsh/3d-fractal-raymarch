#include "LittleOBJLoader.h"
#include "SimpleGUI.h"
#include <initializer_list>
#include <iostream>
#include <random>
#include <string>
#include <variant>
#include <vector>

#pragma once

struct Coordinates {
    int x;
    int y;
};

namespace Menu {

/* A standalone part of the menu */
class Section {
public:
    struct Row {
        int height{PADDING.y};
        std::vector<int> items{};

        ~Row() {
            for (int const item : items) {
                sgRemoveItem(item);
            }
        }
    };

    Section(int const &x, int const &y, std::string const &title,
            Section *parent = nullptr, bool close_button = false);

    virtual ~Section();

    void add_item(std::initializer_list<int> &&items,
                  int const &height = PADDING.x);

    Section *add_section(std::string const &title, bool close_button = false);
    Section *add_section(Section *section);

    std::vector<Section*> get_sections();

    void remove_section(Section *section);

    void on_button_press(int id);

    void toggle(int id, bool root_visibility = false,
                bool set_visibility = false);

protected:
    Coordinates pos;
    // The height relative to the section
    int height{};
    Section *parent;

    void update_height(int const &new_height);

    void update_helper();

private:
    static int const TITLE_HEIGHT{30};
    static int const BUTTON_WIDTH{30};
    static int const INDENT_WIDTH{50};
    constexpr static Coordinates const PADDING{20, 20};

    // Constants used for checking variant types
    static int const ROW_TYPE = 0;
    static int const SECTION_TYPE = 1;

    bool visible{true};
    bool close_button{false};

    int button_id;
    int title_id;
    // The sections close button. Not all sections have this.
    int close_id{-1};

    std::vector<std::variant<Row *, Section *>> children{};
};

int const SLIDER_LENGTH = 500;
void onButtonPress(int id);
void onCreateOperation(int id);

Coordinates const MENU_POS{100, 100};

void init();
void createSliderDisplay(float *v, std::string const &name, float const &min,
                         float const &max, Section *section);

struct Operation : public Section {
    Operation(int const &x, int const &y, std::string const &title,
              Section *parent = nullptr, bool close_button = false);

    std::string gen_var_name() const;

    virtual std::string s_get_variable() = 0;

    virtual std::string s_get_code() const = 0;

    virtual void upload(GLuint program) = 0;
};

struct Rotation_X : public Operation {
    constexpr float static const MIN{-3.14};
    constexpr float static const MAX{3.14};
    float angle{};
    std::string angle_var {gen_var_name()};

    Rotation_X(int const &x, int const &y, std::string const &title,
              Section *parent = nullptr, bool close_button = false);

    std::string s_get_variable() override;
    std::string s_get_code() const override;
    void upload(GLuint program) override;

    std::array<GLfloat, 16> get_matrix();
};

/* Map button presses to the creation of operations */
struct OperationMap {
    int rotation_x;
    int rotation_y;
    int rotation_z;
    
    void create(int id, Section *parent);
};

struct Parameters {
    // Distance estimator
    float scale{2};
    vec3 offset{};
    float dist_iterations{12};

    // Ray march
    float min_dist{0.05};
    float ray_iterations{32};

    std::vector<Operation*> ops {};

    bool has_changed {};

    std::string get_variables() {
        has_changed = false;
        std::string vars{};

        for (Operation *op : ops) {
            vars += op->s_get_variable() + "\n";
        }
        return vars;
    }

    std::string get_code() {
        has_changed = false;
        std::string code{};

        for (Operation *op : ops) {
            code += op->s_get_code() + "\n";
        }
        return code;
    }

    void upload(GLuint program) {
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

        for (Operation* op : ops) {
            op->upload(program);
        }
    };
};

Parameters *get_parameters();
} // namespace Menu
