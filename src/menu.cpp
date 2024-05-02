#include "menu.h"
#include "SimpleGUI.h"
#include <vector>

namespace Menu {

Parameters params{};
OperationMap op_map{};
std::vector<int> items;

Section main{100, 100, "menu"};
Section *dist_est{main.add_section("distance_estimator")};
Section *ray_march{main.add_section("ray_march")};
Section *operations{main.add_section("operations")};

void init() {
    // Distance Estimator
    createSliderDisplay(&params.scale, "scale", -4, 4, dist_est);
    createSliderDisplay(&params.offset.x, "offset x: ", -4, 4, dist_est);
    createSliderDisplay(&params.offset.y, "offset y: ", -4, 4, dist_est);
    createSliderDisplay(&params.offset.z, "offset z: ", -4, 4, dist_est);
    createSliderDisplay(&params.dist_iterations, "iterations: ", 1, 50, dist_est);

    // Ray March
    createSliderDisplay(&params.min_dist, "min dist: ", 0, 1, ray_march);
    createSliderDisplay(&params.ray_iterations, "iterations: ", 1, 50, ray_march);

    // Operations
    // NOTE Just test code for now
    int const i1 = sgCreateButton(0, 0, "New X Rotation", &onCreateOperation);
    op_map.rotation_x = i1;

    int const i2 = sgCreateButton(0, 0, "New Y Rotation", &onCreateOperation);
    op_map.rotation_y = i2;

    int const i3 = sgCreateButton(0, 0, "New Z Rotation", &onCreateOperation);
    op_map.rotation_z = i3;

    int const i4 = sgCreateButton(0, 0, "New BoxFold", nullptr);
    int const i5 = sgCreateButton(0, 0, "New SphereFold", nullptr);

    operations->add_item({i1, i2, i3});
    operations->add_item({i4, i5});
}

void onButtonPress(int id) { main.on_button_press(id); }

void onCreateOperation(int id) {
    op_map.create(id, operations);
}

Parameters &get_parameters() {
    params.ops.clear();

    for (Section *section : operations->get_sections()) {
        // Kinda dangerous ngl
        params.ops.push_back(dynamic_cast<Operation*>(section));
    }

    return params;
}

void createSliderDisplay(float *v, std::string const &name, float const &min,
                         float const &max, Section *section) {

    int const i1 = (sgCreateSlider(0, 0, SLIDER_LENGTH, v, min, max));
    int const i2 =
        (sgCreateDisplayFloat(0, 0, (name + std::string(": ")).c_str(), v));
    section->add_item({i1, i2});
}

} // namespace Menu
