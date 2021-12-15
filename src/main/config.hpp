#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>

struct Config {
    std::array<VkPresentModeKHR, 1> preferredPresentModes = {
        VK_PRESENT_MODE_MAILBOX_KHR
    };
};

Config config;
