#include "LittleOBJLoader.h"
#include "SimpleGUI.h"
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

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
    Section(int const &x, int const &y, std::string const &title)
    : pos{x, y}, height{y} {
        indent_level = 0;

        button_id = sgCreateButton(0, 0, "toggle", &onButtonPress);
        int const title_id = sgCreateStaticString(0, 0, title.c_str());
        add_items({button_id, title_id});
    }

    Section(std::string const &title, Section *parent) {
        indent_level = parent->get_indent() + 1;
        pos = parent->get_pos();
        height = parent->get_height();
        this->parent = parent;

        button_id = sgCreateButton(0, 0, "toggle", &onButtonPress);
        int const title_id = sgCreateStaticString(0, 0, title.c_str());
        add_items({button_id, title_id});

        parent->add_section(this);
    }

    ~Section() {
        for (Section *section : sections) {
            delete section;
        }
    }

    int get_indent() const { return indent_level; }

    int get_height() const { return height; }

    Coordinates const &get_pos() const { return pos; }

    int get_last_item() const {
        return items.size() > 0 ? items[items.size()-1] : -1;
    };

    void add_item(int const id) {
        height += SPACE_Y;

        if (parent != nullptr) {
            parent->height += SPACE_Y;
        }

        sgPosItem(id, pos.x + indent_level * INDENT_WIDTH, height);
        items.push_back(id);
    }

    // Position items on the same line
    void add_items(std::initializer_list<int> ids) {
        int width{indent_level * INDENT_WIDTH};
        height += SPACE_Y;

        if (parent != nullptr) {
            parent->height += SPACE_Y;
        }

        for (int const id : ids) {
            // If an item doesn't have a hitbox, were fucked
            sgPosItem(id, pos.x + width, height);
            width += sgGetItemWidth(id) + SPACE_X;
        }

        items.insert(items.end(), ids);
    }

    void add_section(Section *section) { sections.push_back(section); }

    // TODO: fix this spaghett
    void toggle(int id, bool root_visibility = false, bool set_visibility = false) {
        if (set_visibility) {
            // The menu and title should always have the root visibility
            sgSetVisibility(items[0], root_visibility);
            sgSetVisibility(items[1], root_visibility);

           for (int i = 2; i < items.size(); i++) {
                int const item_visibility = visible && root_visibility;

                sgSetVisibility(items[i], item_visibility);
            } 
        }

        if (id == button_id) {
            set_visibility = true;
            this->visible = !this->visible;
            root_visibility = this->visible;

            // Skip the menu and title
            for (int i = 2; i < items.size(); i++) {
                sgSetVisibility(items[i], visible);
            }
        }

        for (Section *section : sections) {
            section->toggle(id, root_visibility, set_visibility);
        }

        // if (id == button_id || set_visibility) {
        //     set_visibility = true;

        //     auto it = items.begin();
        //     // Skip the button and title
        //     std::advance(it, 2 * set_visibility);

        //     for (; it != items.end(); it++) {
        //         visible = sgGetVisibility(*it) || visible;
        //         sgSetVisibility(*it, !visible);
        //     }
        // }

        // for (Section *section : sections) {
        //     section->toggle(id, visible, set_visibility);
        // }
    }

protected:
    int height{};

private:
    static int const TITLE_HEIGHT{30};
    static int const BUTTON_WIDTH{100};
    static int const SPACE_X{25};
    static int const SPACE_Y{20};
    static int const INDENT_WIDTH{20};

    Coordinates pos;
    int button_id;
    int indent_level;
    bool visible{true};
    Section *parent{nullptr};

    std::vector<int> items{};
    std::vector<Section *> sections{};

int get_current_height() const {
        int height = 0;

        if (items.size() != 0) {
            height = sgGetItemY(items[items.size() - 1]);
        }

        if (sections.size() != 0) {
            int const last_item = sections[sections.size()-1]->get_last_item();
            height = std::max(height, sgGetItemY(last_item));
        }

        return height;
    }
};

Coordinates const MENU_POS{100, 100};
int const SLIDER_LENGTH = 500;

void init();
Parameters &get_parameters();
void createSliderDisplay(float const &x, float const &y, float *v,
                         std::string const &name, float const &min,
                         float const &max, Section &section);

} // namespace Menu
