#ifndef VERTEX_CPP
#define VERTEX_CPP

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GL_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

struct Vertex {
    glm::float32 entity = 0.0f;
    // glm::vec3 pos;
    // glm::vec3 color;
    // glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions{};

        // attributeDescriptions[0].binding = 0;
        // attributeDescriptions[0].location = 0;
        // attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        // attributeDescriptions[0].offset = offsetof(Vertex, pos);

        // attributeDescriptions[1].binding = 0;
        // attributeDescriptions[1].location = 1;
        // attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        // attributeDescriptions[1].offset = offsetof(Vertex, color);

        // attributeDescriptions[2].binding = 0;
        // attributeDescriptions[2].location = 2;
        // attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        // attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
        
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, entity);

        return attributeDescriptions;
    }

    bool operator==(const Vertex &other) const {
        //TODO fix
        // return pos == other.pos && color == other.color && texCoord == other.texCoord;
        return entity == other.entity;
    }
};

#endif
