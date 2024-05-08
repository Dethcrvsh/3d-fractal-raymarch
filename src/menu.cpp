#include "menu.h"
#include "SimpleGUI.h"
#include <vector>

namespace Menu {

Parameters params{};
OperationMap op_map{};
std::vector<int> items;

Section main{100, 100, "menu"};
Section *dist_est{main.add_section("distance estimator")};
Section *ray_march{main.add_section("ray march")};
Section *color{main.add_section("color")};
Section *operations{main.add_section("operations")};

float r, g, b;

void init() {
    // Distance Estimator
    createSliderDisplay(&params.scale, "scale", -8, 8, dist_est);
    createSliderDisplay(&params.offset.x, "offset x: ", -8, 8, dist_est);
    createSliderDisplay(&params.offset.y, "offset y: ", -8, 8, dist_est);
    createSliderDisplay(&params.offset.z, "offset z: ", -8, 8, dist_est);
    createSliderDisplay(&params.dist_iterations, "iterations: ", 1, 50, dist_est);

    // Ray March
    createSliderDisplay(&params.min_dist, "min dist: ", 0, 0.2, ray_march);
    createSliderDisplay(&params.ray_iterations, "iterations: ", 1, 500, ray_march);

    // Color
    
    // Primary Color
    int const d1 = sgCreateColorDrop(0, 0, &params.primary_color.r, &params.primary_color.g, &params.primary_color.b);
    int const c1 = sgCreateSlider(0, 0, COLOR_SLIDER_LENGTH, &params.primary_color.r, 0, 1);
    int const c2 = sgCreateSlider(0, 0, COLOR_SLIDER_LENGTH, &params.primary_color.g, 0, 1);
    int const c3 = sgCreateSlider(0, 0, COLOR_SLIDER_LENGTH, &params.primary_color.b, 0, 1);

    sgSetSliderFillColor(c1, 1, 0, 0);
    sgSetSliderFillColor(c2, 0, 1, 0);
    sgSetSliderFillColor(c3, 0, 0, 1);

    int const s1 = sgCreateStaticString(0, 0, "Primary Color");

    color->add_item({s1});
    color->add_item({d1, c1, c2, c3});

    // Secondary Color
    int const d2 = sgCreateColorDrop(0, 0, &params.secondary_color.r, &params.secondary_color.g, &params.secondary_color.b);
    int const c4 = sgCreateSlider(0, 0, COLOR_SLIDER_LENGTH, &params.secondary_color.r, 0, 1);
    int const c5 = sgCreateSlider(0, 0, COLOR_SLIDER_LENGTH, &params.secondary_color.g, 0, 1);
    int const c6 = sgCreateSlider(0, 0, COLOR_SLIDER_LENGTH, &params.secondary_color.b, 0, 1);

    sgSetSliderFillColor(c4, 1, 0, 0);
    sgSetSliderFillColor(c5, 0, 1, 0);
    sgSetSliderFillColor(c6, 0, 0, 1);

    int const s2 = sgCreateStaticString(0, 0, "Secondary Color");

    color->add_item({s2});
    color->add_item({d2, c4, c5, c6});

    createSliderDisplay(&params.gradient, "Gradient", 0.01, 100, color);

    // Gradient Color
    int const d3 = sgCreateColorDrop(0, 0, &params.glow_color.r, &params.glow_color.g, &params.glow_color.b);
    int const c7 = sgCreateSlider(0, 0, COLOR_SLIDER_LENGTH, &params.glow_color.r, 0, 1);
    int const c8 = sgCreateSlider(0, 0, COLOR_SLIDER_LENGTH, &params.glow_color.g, 0, 1);
    int const c9 = sgCreateSlider(0, 0, COLOR_SLIDER_LENGTH, &params.glow_color.b, 0, 1);

    sgSetSliderFillColor(c7, 1, 0, 0);
    sgSetSliderFillColor(c8, 0, 1, 0);
    sgSetSliderFillColor(c9, 0, 0, 1);

    int const s3 = sgCreateStaticString(0, 0, "Gradient Color");

    color->add_item({s3});
    color->add_item({d3, c7, c8, c9});

    createSliderDisplay(&params.glow_amount, "Glow Amount", 0, 2, color);

    // Operations
    int const i1 = sgCreateButton(0, 0, "New X Rotation", &onCreateOperation);
    op_map.rotation_x = i1;

    int const i2 = sgCreateButton(0, 0, "New Y Rotation", &onCreateOperation);
    op_map.rotation_y = i2;

    int const i3 = sgCreateButton(0, 0, "New Z Rotation", &onCreateOperation);
    op_map.rotation_z = i3;

    int const i4 = sgCreateButton(0, 0, "New BoxFold", &onCreateOperation);
    op_map.box_fold = i4;

    int const i5 = sgCreateButton(0, 0, "New SphereFold", &onCreateOperation);
    op_map.sphere_fold = i5;

    int const i6 = sgCreateButton(0, 0, "New SierpinskiFold", &onCreateOperation);
    op_map.sierpinski_fold = i6;

    int const i7 = sgCreateButton(0, 0, "New OctahedralFold?", &onCreateOperation);
    op_map.octahedral_fold = i7;

    operations->add_item({i1, i2, i3});
    operations->add_item({i4, i5, i6});
    operations->add_item({i7});
}

void onButtonPress(int id) {
    params.has_changed = true;
    main.on_button_press(id);
}

void onCreateOperation(int id) {
    params.has_changed = true;
    op_map.create(id, operations);
}

Parameters *get_parameters() {
    params.ops.clear();

    for (Section *section : operations->get_sections()) {
        // Kinda dangerous ngl
        params.ops.push_back(dynamic_cast<Operation*>(section));
    }

    return &params;
}

void createSliderDisplay(float *v, std::string const &name, float const &min,
                         float const &max, Section *section) {

    int const i1 = (sgCreateSlider(0, 0, SLIDER_LENGTH, v, min, max));
    int const i2 =
        (sgCreateDisplayFloat(0, 0, (name + std::string(": ")).c_str(), v));
    section->add_item({i1, i2});
}

} // namespace Menu
