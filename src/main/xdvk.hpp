// Oliver Kovacs 2021 - xdvk - MIT

#ifndef XDVK_HPP
#define XDVK_HPP

#include <cstdint>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace xdvk {

    template<uint32_t D>
    struct Transform {
        // float buffer[2 * D + D * (D - 1) / 2]; // NOLINT(*-avoid-c-arrays)
        float buffer[2 * D + D * (D - 1) / 2]; // NOLINT(cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays)
        float *position = buffer;
        float *scale = &buffer[D];
        float *rotation = &buffer[2 * D];

        Transform();
    };

    struct Geometry {

        std::vector<float> vertices;
        
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;

        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        uint32_t indexBufferSize;
        bool drawIndexed = true;

        size_t vertexBufferIndex;
        size_t indexBufferIndex;
        size_t transformBufferIndex;
    };

    template<uint32_t D>
    struct Entity {
        uint64_t id;
        uint64_t components;
        Transform<D> transform;
        Geometry geometry;
    };

    struct Index {
        uint64_t id;
        uint32_t index;
        uint32_t next;
    };

    template<uint32_t D>
    class Scene {
        public:
        
        std::vector<Index> indices;
        std::vector<Entity<D>> entities;
        
        explicit Scene(size_t reserve);

        auto has(uint64_t id) -> bool;
        auto get(uint64_t id) -> Entity<D> &;
        auto add() -> uint64_t;
        void remove(uint64_t id);

        private:
        uint32_t entity_count = 0;
        uint32_t freelist;
    };

    void createHypercubeVertices(std::vector<float> &vertices, uint32_t dimension, float size);
    void hypercubeVertices(std::vector<float> &vertices, uint32_t dimension, float size, uint32_t stride, uint32_t offset);
    void hypercubeIndices(std::vector<uint32_t> &buffer, uint32_t dimension, uint32_t stride, uint32_t offset);
    void hypercubeEdges(std::vector<float> &buffer, uint32_t dimension, uint32_t stride, uint32_t offset);
    void icositetrachoronVertices(std::vector<float> &buffer, float size, uint32_t stride, uint32_t offset);
    void icositetrachoronIndices(std::vector<uint32_t> &buffer, uint32_t stride, uint32_t offset);

    template<uint32_t D>
    void hypercubeTransform(std::vector<float> &buffer, Transform<D> transform, uint32_t index, uint32_t stride, uint32_t offset);

    auto rotationSize(uint32_t dimension) -> size_t;
    auto transformSize(uint32_t dimension) -> uint32_t;

    template<typename T>
    void printVector(std::vector<T> vector, const std::string &name);

    template<typename T, size_t N>
    void printArray(std::array<T, N> array, const std::string &name);
}

#include "xdvk.t.hpp"

#endif /* XDVK_HPP */
