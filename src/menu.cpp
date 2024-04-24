#include "menu.h"
#include "SimpleGUI.h"
#include <vector>

namespace Menu {

Parameters params{};
std::vector<int> items;
Section main{100, 100, "Main"};

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


    auto s = new Section("test", &main);
    createSliderDisplay(0, OFFSET * 3, &params.offset.z,
                        "offset z: ", -4, 4, *s);
}

void onButtonPress(int id) {
    main.toggle(id);
}

Parameters &get_parameters() { return params; }

void createSliderDisplay(float const &x, float const &y, float *v,
                         std::string const &name, float const &min,
                         float const &max, Section &section) {
    const float OFFSET{20};
    int const i1 = (sgCreateSlider(0, 0, SLIDER_LENGTH, v, -4, 4));
    int const i2 = (sgCreateDisplayFloat(0, 0, (name + std::string(": ")).c_str(), v));

    section.add_items({i1, i2});
}

} // namespace Menu
