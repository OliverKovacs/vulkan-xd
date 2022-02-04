#ifndef XDVK_TPP
#define XDVK_TPP

#include <iostream>
#include "xdvk.hpp"

template<uint32_t D>
xdvk::Transform<D>::Transform() {
    std::fill_n(buffer, 2 * D + D * (D - 1) / 2, 0.0F);
    std::fill_n(scale, D, 1.0F);
};

namespace xdvk {

    #define INDEX_MASK 0xffffffff
    #define NEW_OBJECT_ID_ADD 0x100000000
    template<uint32_t D>
    Scene<D>::Scene(size_t reserve) {
        indices.resize(reserve);
        entities.reserve(reserve);
        for (size_t i = 0; i < reserve; i++) {
            indices[i].id = i;
            indices[i].next = i + 1;
        }
        freelist = 0;
    };

    template<uint32_t D>
    auto Scene<D>::has(uint64_t id) -> bool {
        Index &index = indices[id & INDEX_MASK];
        return index.id == id && index.index != UINT32_MAX;
    }

    template<uint32_t D>
    auto Scene<D>::get(uint64_t id) -> Entity<D> & {
        return entities[indices[id & INDEX_MASK].index];
    }

    template<uint32_t D>
    auto Scene<D>::add() -> uint64_t {
        Index &index = indices[freelist];
        freelist = index.next;
        index.id += NEW_OBJECT_ID_ADD;
        index.index = entity_count++;
        Entity<D> &entity = entities.emplace_back();
        // Entity<D> &entity = entities[index.index];
        entity.id = index.id;
        return entity.id;
    }

    template<uint32_t D>
    void Scene<D>::remove(uint64_t id) {
        Index &index = indices[id & INDEX_MASK];
        Entity<D> &entity = entities[index.index];
        entity = entities[--entity_count];
        entities.pop_back();
        indices[entity.id & INDEX_MASK].index = index.index;
        index.index = UINT32_MAX;
        index.next = freelist;
        freelist = id & INDEX_MASK;
    }

    template<uint32_t D>
    void hypercubeTransform(std::vector<float> &buffer, Transform<D> transform, uint32_t index, uint32_t stride, uint32_t offset) {
        uint32_t size = transformSize(D);
        const uint32_t block = size + stride;
        buffer.resize(offset + (index + 1) * block);
        std::copy_n(&transform.buffer[offset + index * stride], size, buffer.begin());
    }

    template<typename T>
    void printVector(std::vector<T> vector, const std::string &name) {
        std::cout << name << "[" << vector.size() << "] = [ ";
        for (auto elem : vector) {
            std::cout << elem << " ";
        }
        std::cout << "]" << std::endl;
    }

    template<typename T, size_t N>
    void printArray(std::array<T, N> array, const std::string &name) {
        std::cout << name << "[" << array.size() << "] = [ ";
        for (auto elem : array) {
            std::cout << elem << " ";
        }
        std::cout << "]" << std::endl;
    }
};

#endif /* XDVK_TPP */
