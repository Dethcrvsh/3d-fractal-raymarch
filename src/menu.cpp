#include "menu.h"
#include "SimpleGUI.h"
#include <vector>

namespace Menu {

Parameters params{};
std::vector<int> items;
Section main{100, 500, "Main"};

void init() {
    const float OFFSET{20};
    createSliderDisplay(0, 0, &params.scale, "scale",
                        -4, 4, main);
    createSliderDisplay(0, OFFSET, &params.offset.x,
                        "offset x: ", -4, 4, main);
    createSliderDisplay(0, OFFSET * 2, &params.offset.y,
                        "offset y: ", -4, 4, main);
    createSliderDisplay(0, OFFSET * 3, &params.offset.z,
                        "offset z: ", -4, 4, main);


    Section new_s {Section{50, 50, "section"}};
    main.add_section(new_s);
    createSliderDisplay(0, OFFSET * 3, &params.offset.z,
                        "offset z: ", -4, 4, new_s);
}

void onButtonPress(int id) {
    main.toggle(id);
}

Parameters &get_parameters() { return params; }

void createSliderDisplay(float const &x, float const &y, float *v,
                         std::string const &name, float const &min,
                         float const &max, Section &section) {
    const float OFFSET{20};
    section.add_item(sgCreateSlider(x, y, SLIDER_LENGTH, v, -4, 4));
    section.add_item(sgCreateDisplayFloat(x + SLIDER_LENGTH + OFFSET, y,
                                          (name + std::string(": ")).c_str(), v));
}

} // namespace Menu
