#include <GLFW/glfw3.h>

#include "array"
#include "cstdint"

struct Keybinds {
    std::array<uint32_t, 8> directions = {
        GLFW_KEY_A,
        GLFW_KEY_D,
        GLFW_KEY_W,
        GLFW_KEY_S,
        GLFW_KEY_Q,
        GLFW_KEY_E,
        GLFW_KEY_R,
        GLFW_KEY_F
    };
    std::array<uint32_t, 10> rotations = {
        GLFW_KEY_1,
        GLFW_KEY_2,
        GLFW_KEY_3,
        GLFW_KEY_4,
        GLFW_KEY_5,
        GLFW_KEY_6,
        GLFW_KEY_7,
        GLFW_KEY_8,
        GLFW_KEY_9,
        GLFW_KEY_0
    };
};

const Keybinds keybinds {};
