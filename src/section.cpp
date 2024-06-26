#include "SimpleGUI.h"
#include "menu.h"
#include <algorithm>
#include <iostream>
#include <ostream>

using namespace Menu;

Section::Section(int const &x, int const &y, std::string const &title, Section *parent, bool close_button)
: pos{x, y}, parent {parent}, close_button {close_button} {
    button_id = sgCreateButton(0, 0, "V", &onButtonPress);
    title_id = sgCreateStaticString(0, 0, title.c_str());

    if (close_button) {
        close_id = sgCreateButton(0, 0, "X", &onButtonPress);
    }

    update_height(30);
}

Section::~Section() {
    sgRemoveItem(button_id);
    sgRemoveItem(title_id);
    sgRemoveItem(close_id);

    for (std::variant child : children) {
        if (child.index() == ROW_TYPE) {
            Row *row = std::get<Row*>(child);
            delete row;
        }
        else if (child.index() == SECTION_TYPE) {
            Section *section = std::get<Section*>(child);
            delete section;
        }
    }
}

void Section::add_item(std::initializer_list<int>&& items, int const &height) {
    Row *row {new Row()};
    row->height = height;
    row->items.insert(row->items.end(), items.begin(), items.end());
    children.push_back(row);
    update_height(0);
}

Section* Section::add_section(std::string const& title, bool close_button) {
    Section *section = new Section(0, 0, title, this, close_button);
    children.push_back(section);
    update_height(0);
    return section;
}

Section* Section::add_section(Section *section) {
    children.push_back(section);
    update_height(0);
    return section;
};

std::vector<Section*> Section::get_sections() {
    std::vector<Section*> sections{};

    for (std::variant child : children) {
        if (child.index() == SECTION_TYPE) {
            sections.push_back(std::get<Section*>(child));
        }
    }
    return sections;
};

void Section::remove_section(Section *section) {
    auto remove_it = std::find(children.begin(), children.end(), std::variant<Row*, Section*>(section));
    children.erase(remove_it);
    delete section;
    update_height(0);
}

void Section::on_button_press(int id) {
    if (id == close_id) {
        // Root should never have a close button
        parent->remove_section(this);
    }
    else if (id == button_id) {
        toggle(id);
        update_height(0);
    }

    for (std::variant<Row*, Section*> child : children) {
        if (child.index() == SECTION_TYPE) {
            Section *section = std::get<Section*>(child);
            section->on_button_press(id);
        }
    }
}

// NOTE: Little hack for now, only root works as intended
void Section::toggle(int id, bool root_visibility, bool parent_visibility, bool set_visibility) {
    if (id == button_id) {
        set_visibility = true;
        this->visible = !this->visible;
        root_visibility = this->visible;
        parent_visibility = root_visibility;
    }

    if (set_visibility) {
        // The menu and title should always have the root visibility
        if (id != button_id) {
            sgSetVisibility(button_id, root_visibility);
            sgSetVisibility(title_id, root_visibility);
            sgSetVisibility(close_id, root_visibility);

        }

        for (std::variant<Row*, Section*> child : children) {
            if (child.index() == ROW_TYPE) {
                Row *row = std::get<Row*>(child);

                for (int const item : row->items) {
                    sgSetVisibility(item, parent_visibility);
                }
            } else if (child.index() == SECTION_TYPE) {
                Section *section = std::get<Section*>(child);
                if (parent_visibility) {
                    section->toggle(id, root_visibility, section->visible, set_visibility);
                } else {
                    section->toggle(id, root_visibility, false, set_visibility);
                }
            }
        }
    }
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
    sgPosItem(title_id, pos.x + BUTTON_WIDTH + BUTTON_WIDTH*close_button, pos.y);

    if (close_button) {
        sgPosItem(close_id, pos.x + BUTTON_WIDTH, pos.y);
    }

    height = TITLE_HEIGHT;

    for (std::variant<Row*, Section*> const child : children) {
        if (child.index() == ROW_TYPE) {
            Row *row = std::get<Row*>(child); 
            int width {INDENT_WIDTH};
            
            for (const int item : row->items) {
                sgPosItem(item, pos.x + width, pos.y + height);
                width += sgGetItemWidth(item) + PADDING.x;
            }

            height += row->height;
        } else if (child.index() == SECTION_TYPE) {
            height += 25;
            Section* section = std::get<Section*>(child);
            section->pos = {pos.x + INDENT_WIDTH, pos.y + height};
            section->update_helper();
            height += section->height * section->visible;
        }
    }
}


