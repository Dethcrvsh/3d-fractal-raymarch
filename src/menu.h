#include <string>
#include "LittleOBJLoader.h"

namespace Menu {

struct Parameters {
    float scale{2};
    vec3 offset{};
};

void init();
Parameters &get_parameters();
void createSliderDisplay(float const &x, float const &y, float *v,
                         std::string const &name, float const &min,
                         float const &max);
void onButtonPress();
} // namespace Menu
