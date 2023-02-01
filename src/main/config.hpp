#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <cstdint>
#include <iostream>

const char* vblank_mode_str = std::getenv("vblank_mode");
size_t vblank_mode = vblank_mode_str
    ? std::stoi(vblank_mode_str)
    : 1;

struct Config {
    const size_t default_width = 1920;
    const size_t default_height = 1080;
    bool log_validation_layer = false;
    bool vsync = vblank_mode != 0;
    std::vector<VkPresentModeKHR> preferredPresentModes = {
        this->vsync
            ? VK_PRESENT_MODE_MAILBOX_KHR       // vsync
            : VK_PRESENT_MODE_IMMEDIATE_KHR,    // no vsync, tearing possible
    };
};

const Config config;
