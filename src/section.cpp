#include "SimpleGUI.h"
#include "menu.h"

using namespace Menu;

Section::Section(int const &x, int const &y, std::string const &title, Section *parent)
: pos{x, y}, parent {parent} {
    button_id = sgCreateButton(pos.x, pos.y, "toggle", &onButtonPress);
    title_id = sgCreateStaticString(pos.x + BUTTON_WIDTH, pos.y, title.c_str());

    //TODO: use the constant
    update_height(30);
}

Section::Section(std::string const &title) : Section(0, 0, title) {}

Section::~Section() {}

void Section::add_item(std::initializer_list<int>&& items, int const &height) {
    Row *row {new Row()};
    row->height = height;
    row->items.insert(row->items.end(), items.begin(), items.end());
    children.push_back(row);
    update_height(0);
}

Section* Section::add_section(std::string const& title) {
    Section *section = new Section(0, 0, title, this);
    children.push_back(section);
    update_height(0);
    return section;
}

// TODO: fix this spaghett
void Section::toggle(int id, bool root_visibility, bool set_visibility) {
    // if (set_visibility) {
    //     // The menu and title should always have the root visibility
    //     sgSetVisibility(button_id, root_visibility);
    //     sgSetVisibility(title_id, root_visibility);

    //     for (std::variant<int, Section*> child : children) {
    //         if (child.index() == INT_TYPE) {
    //             int const item = std::get<int>(child);
    //             sgSetVisibility(item, visible && root_visibility);
    //         } else if (child.index() == SECTION_TYPE) {
    //             Section *section = std::get<Section*>(child);
    //         }
    //     }
    // }

    // if (id == button_id) {
    //     set_visibility = true;
    //     this->visible = !this->visible;
    //     root_visibility = this->visible;

    //     // Skip the menu and title
    //     for (int i = 2; i < items.size(); i++) {
    //         sgSetVisibility(items[i], visible);
    //     }
    // }

    // for (Section *section : sections) {
    //     section->toggle(id, root_visibility, set_visibility);
    // }
}

void Section::update_height(int const& new_height) {
    if (parent != nullptr) {
        parent->update_height(new_height);
    } else {
        update_helper();
    }
}

void Section::update_helper() {
    sgPosItem(button_id, pos.x, pos.y);
    sgPosItem(title_id, pos.x + BUTTON_WIDTH, pos.y);
    height = TITLE_HEIGHT;

    for (std::variant<Row*, Section*> const child : children) {
        if (child.index() == ROW_TYPE) {
            Row *row = std::get<Row*>(child); 
            int width {0};
            
            for (const int item : row->items) {
                sgPosItem(item, pos.x + width, pos.y + height);
                width += sgGetItemWidth(item) + PADDING.x;
            }

            height += row->height;
        } else if (child.index() == SECTION_TYPE) {
            Section* section = std::get<Section*>(child);
            section->pos = {pos.x + INDENT_WIDTH, pos.y + height};
            section->update_helper();
            height += section->height + 25;
        }
    }
}


