#include <algorithm>
#include "vulkan.cpp"
#include "keybinds.cpp"
#include "xdvk.hpp"
#include "scene.cpp"

void Vulkan::callback() {
    for (size_t i = 0; i < scene.entities.size(); i++) {
        auto &entity = scene.entities[i];
        for (size_t j = 0; j < xdvk::rotationSize(DIMENSION); j++) {
            entity.transform.rotation[j] += deltaTime * 0.6 * (j + 0.2) / xdvk::rotationSize(DIMENSION);
        }
        bool shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
        // bool alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
        float sign = (1 - 2 * shift);
        for (size_t j = 0; j < std::min(xdvk::rotationSize(DIMENSION), keybinds.rotations.size()); j++) {
            entity.transform.rotation[j] += deltaTime * 2 * sign * (glfwGetKey(window, keybinds.rotations[j]) == GLFW_PRESS);
        }
        for (size_t j = 0; j < std::min((size_t)DIMENSION * 2, keybinds.directions.size()); j++) {
            entity.transform.position[j >> 1] += deltaTime * (1.0 - (!(j & 1) << 1)) * (glfwGetKey(window, keybinds.directions[j]) == GLFW_PRESS);
        }
        std::copy_n(scene.entities[i].transform.buffer, entity.geometry.vertices.size(), &storageVectors[1][i * xdvk::transformSize(DIMENSION)]);
        entity.geometry.transformBufferIndex = i * xdvk::transformSize(DIMENSION);
    }
}

auto main() -> int {
    Vulkan app;

    std::cout << "vsync: " << config.vsync << std::endl;

    try {
        app.run();
    }
    catch (const std::exception& error) {
        std::cerr << "ERROR: " << error.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
