#include "LittleOBJLoader.h"
#include <initializer_list>
#include <string>
#include <variant>
#include <vector>

struct Coordinates {
    int x;
    int y;
};

namespace Menu {

class Section;

struct Parameters {
    float scale{2};
    vec3 offset{};
    float iterations{12};

    void upload(GLuint program) {
        glUniform1f(glGetUniformLocation(program, "in_Scale"), scale);
        glUniform3f(glGetUniformLocation(program, "in_Offset"), offset.x, offset.y,
                    offset.z);
    };
};

void onButtonPress(int id);

Coordinates const MENU_POS{100, 100};
int const SLIDER_LENGTH = 500;

void init();
Parameters &get_parameters();
void createSliderDisplay(float *v, std::string const &name, float const &min,
                         float const &max, Section *section);

/* A standalone part of the menu */
class Section {
public:
    struct Row {
        std::vector<int> items{};
    };

    Section(int const &x, int const &y, std::string const &title,
            Section *parent = nullptr);

    Section(std::string const &title);

    ~Section();

    void add_item(std::initializer_list<int> &&items);

    Section *add_section(std::string const &title);

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
    static int const BUTTON_WIDTH{100};
    static int const INDENT_WIDTH{50};
    constexpr static Coordinates const PADDING{20, 20};

    // Constants used for checking variant types
    static int const ROW_TYPE = 0;
    static int const SECTION_TYPE = 1;

    bool visible{true};

    int button_id;
    int title_id;

    std::vector<std::variant<Row *, Section *>> children{};
};

} // namespace Menu
