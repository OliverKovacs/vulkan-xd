#ifndef VERTEX_H
#define VERTEX_H

#include <array>

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GL_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

struct Vertex {
    glm::float32 entity = 0.0F;

    static auto getBindingDescription() -> VkVertexInputBindingDescription;
    static auto getAttributeDescriptions() -> std::array<VkVertexInputAttributeDescription, 1>;

    auto operator==(const Vertex &other) const -> bool;
};

#endif
