#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <cstdint>

struct Config {
    const size_t default_width = 1920;
    const size_t default_height = 1080;
    // const size_t default_width = 800;
    // const size_t default_height = 600;
    bool log_validation_layer = false;
    std::vector<VkPresentModeKHR> preferredPresentModes = {
        VK_PRESENT_MODE_IMMEDIATE_KHR,      // no vsync, tearing possible
        VK_PRESENT_MODE_MAILBOX_KHR,        // vsync
    };
};

const Config config;
