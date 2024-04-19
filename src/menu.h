#include "LittleOBJLoader.h"
#include "SimpleGUI.h"
#include <string>
#include <vector>
#include <iostream>

struct Coordinates {
    int x;
    int y;
};

namespace Menu {

struct Parameters {
    float scale{2};
    vec3 offset{};
};

void onButtonPress(int id);

// A standalone part of the menu
// TODO: Move implementation
class Section {
public:
    Section(int const& x, int const& y, std::string const& title) : pos{x, y} {
        button_id = sgCreateButton(x, y, "toggle", &onButtonPress);
        sgCreateStaticString(x + BUTTON_WIDTH, y + 2, title.c_str());
    }

    void set_offset(int const x, int const y) {
        pos.x += x;
        pos.y += y;
    }

    void add_item(int const id) {
        sgOffsetItem(id, pos.x, pos.y + TITLE_HEIGHT);
        items.push_back(id); 
    }

    void add_section(Section &section) {
        section.set_offset(pos.x + 10, pos.y + 30);
        sections.push_back(section);
    }

    void toggle(int id) {
        if (id == button_id || id == -1) {
            for (int const item : items) {
                sgToggleItem(item);
            }
            id = -1;
        }

        for (Section &section: sections) {
            section.toggle(id);
        }
    }

private:
    static int const TITLE_HEIGHT {30};
    static int const BUTTON_WIDTH {100};

    Coordinates pos;
    int button_id;
    std::vector<int> items {};
    std::vector<Section> sections {};
};

Coordinates const MENU_POS{100, 100};
int const SLIDER_LENGTH = 500;

void init();
Parameters &get_parameters();
void createSliderDisplay(float const &x, float const &y, float *v,
                         std::string const &name, float const &min,
                         float const &max, Section &section);

}
