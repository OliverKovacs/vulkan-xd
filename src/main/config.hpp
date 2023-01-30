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
    std::array<VkPresentModeKHR, 1> preferredPresentModes = {
        VK_PRESENT_MODE_MAILBOX_KHR
    };
};

const Config config;
