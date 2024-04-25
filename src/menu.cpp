#include "menu.h"
#include "SimpleGUI.h"
#include <vector>
#include <iostream>

namespace Menu {

Parameters params{};
std::vector<int> items;

Section main{100, 100, "menu"};
Section *dist_est{main.add_section("distance_estimator")};
Section *ray_march{main.add_section("ray_march")};


void init() {
    createSliderDisplay(&params.scale, "scale", -4, 4, dist_est);
    createSliderDisplay(&params.offset.x, "offset x: ", -4, 4, dist_est);
    createSliderDisplay(&params.offset.y, "offset y: ", -4, 4, dist_est);
    createSliderDisplay(&params.offset.z, "offset z: ", -4, 4, dist_est);
}

void onButtonPress(int id) { main.toggle(id); }

Parameters &get_parameters() { return params; }

void createSliderDisplay(float *v, std::string const &name, float const &min,
                         float const &max, Section *section) {

    int const i1 = (sgCreateSlider(0, 0, SLIDER_LENGTH, v, -4, 4));
    int const i2 =
        (sgCreateDisplayFloat(0, 0, (name + std::string(": ")).c_str(), v));
    section->add_item({i1, i2});
}

} // namespace Menu
