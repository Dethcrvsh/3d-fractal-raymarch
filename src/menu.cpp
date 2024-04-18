#include "menu.h"
#include "SimpleGUI.h"
#include <vector>

namespace Menu {

struct Coordinates {
    int x;
    int y;
};

Coordinates const MENU_POS{100, 100};
int const SLIDER_LENGTH = 500;

Parameters parameters {};

bool visible{true};
// Keep track of the items' id
std::vector<int> items{};

void init() {
    const float OFFSET{20};

    createSliderDisplay(MENU_POS.x, MENU_POS.y + OFFSET, &parameters.scale,
                        "scale", -4, 4);

    createSliderDisplay(MENU_POS.x, MENU_POS.y + OFFSET * 2, &parameters.offset.x,
                        "offset x: ", -4, 4);
    createSliderDisplay(MENU_POS.x, MENU_POS.y + OFFSET * 3, &parameters.offset.y,
                        "offset y: ", -4, 4);
    createSliderDisplay(MENU_POS.x, MENU_POS.y + OFFSET * 4, &parameters.offset.z,
                        "offset z: ", -4, 4);

    sgCreateButton(MENU_POS.x, MENU_POS.y, "click", &onButtonPress);
}

Parameters &get_parameters() { 
    return parameters; }

void createSliderDisplay(float const &x, float const &y, float *v,
                         std::string const &name, float const &min,
                         float const &max) {
    const float OFFSET{20};
    items.push_back(sgCreateSlider(x, y, SLIDER_LENGTH, v, -4, 4));
    items.push_back(sgCreateDisplayFloat(x + SLIDER_LENGTH + OFFSET, y,
                                         (name + std::string(": ")).c_str(), v));
}

void hideItems() {
    for (int const item : items) {
        sgRemoveItem(item);
    }
    items.clear();
}

void showItems() { init(); }

void onButtonPress() {
    visible = !visible;

    if (visible) {
        showItems();
    } else {
        hideItems();
    }
};

} // namespace Menu
