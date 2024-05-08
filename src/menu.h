#include "LittleOBJLoader.h"
#include <array>
#include "SimpleGUI.h"
#include <initializer_list>
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

    std::vector<Section *> get_sections();

    void remove_section(Section *section);

    void on_button_press(int id);

    void toggle(int id, bool root_visibility = false, bool parent_visibility = false,
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

int const SLIDER_LENGTH = 400;
int const COLOR_SLIDER_LENGTH = 100;
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

struct Rotation : public Operation {
    constexpr float static const MIN{-3.14};
    constexpr float static const MAX{3.14};
    float angle{};
    std::string angle_var{gen_var_name()};

    Rotation(int const &x, int const &y, std::string const &title,
             Section *parent = nullptr, bool close_button = false);

    std::string s_get_variable() override;
    std::string s_get_code() const override;
    void upload(GLuint program) override;

    virtual std::array<GLfloat, 16> get_matrix() = 0;
};

struct Rotation_X : public Rotation {
    Rotation_X(int const &x, int const &y, std::string const &title,
               Section *parent = nullptr, bool close_button = false);

    std::array<GLfloat, 16> get_matrix() override;
};

struct Rotation_Y : public Rotation {
    Rotation_Y(int const &x, int const &y, std::string const &title,
               Section *parent = nullptr, bool close_button = false);

    std::array<GLfloat, 16> get_matrix() override;
};

struct Rotation_Z : public Rotation {
    Rotation_Z(int const &x, int const &y, std::string const &title,
               Section *parent = nullptr, bool close_button = false);

    std::array<GLfloat, 16> get_matrix() override;
};

struct BoxFold : public Operation {
    constexpr float static const MIN{0};
    constexpr float static const MAX{5};

    float fold_limit{1};
    std::string fold_limit_var {gen_var_name()};

    BoxFold(int const &x, int const &y, std::string const &title,
            Section *parent = nullptr, bool close_button = false);

    std::string s_get_variable() override;
    std::string s_get_code() const override;
    void upload(GLuint program) override;
};

struct SphereFold: public Operation {
    constexpr float static const MIN{0};
    constexpr float static const MAX{5};

    float fixed_radius{2};
    std::string fixed_radius_var {gen_var_name()};

    SphereFold(int const &x, int const &y, std::string const &title,
            Section *parent = nullptr, bool close_button = false);

    std::string s_get_variable() override;
    std::string s_get_code() const override;
    void upload(GLuint program) override;
};

struct SierpinskiFold: public Operation {
    constexpr float static const MIN{0};
    constexpr float static const MAX{5};

    float fold_limit{1};
    std::string fold_limit_var {gen_var_name()};

    SierpinskiFold(int const &x, int const &y, std::string const &title,
            Section *parent = nullptr, bool close_button = false);

    std::string s_get_variable() override;
    std::string s_get_code() const override;
    void upload(GLuint program) override;
};

struct OctahedralFold: public Operation {
    constexpr float static const MIN{0};
    constexpr float static const MAX{5};

    float fold_limit{1};
    std::string fold_limit_var {gen_var_name()};

    OctahedralFold(int const &x, int const &y, std::string const &title,
            Section *parent = nullptr, bool close_button = false);

    std::string s_get_variable() override;
    std::string s_get_code() const override;
    void upload(GLuint program) override;
};


/* Map button presses to the creation of operations */
struct OperationMap {
    int rotation_x;
    int rotation_y;
    int rotation_z;
    int box_fold;
    int sphere_fold;
    int sierpinski_fold;
    int octahedral_fold;

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

    // Color
    vec3 primary_color{0.7, 0.0, 0.8};
    vec3 secondary_color{0.0, 0.0, 1.0};
    vec3 glow_color{0.0, 1.0, 0.0};
    float gradient{0.01};
    float glow_amount{};

    std::vector<Operation *> ops{};

    bool has_changed{};

    std::string get_variables();

    std::string get_code();

    void upload(GLuint program);
};

Parameters *get_parameters();

} // namespace Menu
