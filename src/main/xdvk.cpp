// Oliver Kovacs 2021 - xdvk - MIT

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "vertex.hpp"
#include "xdvk.hpp"

namespace xdvk {

    void createHypercubeVertices(std::vector<float> &vertices, const uint32_t dimension, float size) {
        const uint32_t block = 1;
        const auto n1 = static_cast<uint32_t>(pow(2, dimension));
        const uint32_t n2 = dimension;
        vertices.resize(static_cast<size_t>(n1) * n2 * block);
        for (size_t i = 0; i < n1; i++) {
            for (size_t j = 0; j < n2; j++) {
                const uint32_t index = (i * n2 + j) * block;
                vertices[index] = size * (1.0 - (static_cast<uint32_t>(floor(i / pow(2.0, j))) % 2) * 2.0);
            }
        }
    }
    
    void hypercubeVertices(std::vector<float> &vertices, const uint32_t dimension, float size, uint32_t stride, uint32_t offset) {
        const uint32_t block = 1 + stride;
        const uint32_t n1 = pow(2, dimension);
        const uint32_t n2 = dimension;
        vertices.resize(offset + n1 * n2 * block);
        for (size_t i = 0; i < n1; i++) {
            for (size_t j = 0; j < n2; j++) {
                const uint32_t index = offset + (i * n2 + j) * block;
                vertices[index] = - size * (1.0 - (static_cast<uint32_t>(floor(i / pow(2.0, j))) % 2) * 2.0);
            }
        }
    }

    void hypercubeIndices(std::vector<uint32_t> &buffer, const uint32_t dimension, uint32_t stride, uint32_t offset) {
        const uint32_t block = 2 + stride;
        const uint32_t n1 = dimension;
        const uint32_t n2 = pow(2, (dimension - 1));
        buffer.resize(offset + n1 * n2 * block);
        for (size_t i = 0; i < n1; i++) {
            for (size_t j = 0; j < n2; j++) {
                const uint32_t base = (j % static_cast<uint32_t>(pow(2, i))) + pow(2, (i + 1)) * floor(j / pow(2, i));
                const uint32_t index = offset + (i * n2 + j) * block;
                buffer[index]     = base;
                buffer[index + 1] = base + pow(2, i);
            }
        }
    }

    void hypercubeEdges(std::vector<float> &buffer, uint32_t dimension, uint32_t stride, uint32_t offset) {
        const uint32_t block = 8 + stride;
        const uint32_t n1 = dimension;
        const uint32_t n2 = pow(2, (dimension - 1));
        buffer.resize(offset + n1 * n2 * block);
        for (size_t i = 0; i < n1; i++) {
            for (size_t j = 0; j < n2; j++) {
                const uint32_t index = offset + (i * n2 + j) * block;
                buffer[index]     = index;
                buffer[index + 1] = i;
                buffer[index + 2] = j;
                buffer[index + 3] = 0;
                buffer[index + 4] = index;
                buffer[index + 5] = i;
                buffer[index + 6] = j;
                buffer[index + 7] = 1;
            }          
        }
    }

    void icositetrachoronVertices(std::vector<float> &buffer, float size, uint32_t stride, uint32_t offset) {
        const uint32_t block = 1 + stride;
        const uint32_t n1 = 8;
        const uint32_t n2 = 4;
        const uint32_t _offset = offset + 8 * 4 * block;
        buffer.resize(offset + 24 * 4 * block);

        // { ±1, 0, 0, 0 } → 8 vertices
        for (size_t i = 0; i < n1; i++) {
            for (size_t j = 0; j < n2; j++) {
                const uint32_t index = offset + (i * n2 + j) * block;
                buffer[index] = j == (i / 2) ? (static_cast<bool>(i % 2) ? size : -size) : 0.0F;
            }
        }
        
        // { ±0.5, ±0.5, ±0.5, ±0.5 } → 16 vertices
        hypercubeVertices(buffer, 4, 0.5 * size, stride, _offset);
    }

    void icositetrachoronIndices(std::vector<uint32_t> &buffer, uint32_t stride, uint32_t offset) {
        const size_t N_EDGES = 96;
        buffer.resize(static_cast<size_t>(N_EDGES * 2));

        // 64 edges
        for (size_t i = 0; i < 4; i++) {
            for (size_t j = 0; j < 2; j++) {
                for (size_t k = 0; k < 8; k++) {
                    const uint32_t index = i * 2 * 8 * 2 + j * 8 * 2 + k * 2;
                    buffer[index]     = i * 2 + j;
                    buffer[index + 1] = (k % (1 << i)) + (k / (1 << i) * (2 << i)) + 8 + j * pow(2, i);
                }
            }
        }
        
        // 32 edges
        xdvk::hypercubeIndices(buffer, 4, stride, offset + 64 * 2);
        for (size_t i = 0; i < 32 * 2; i++) {
            buffer[i + 64 * 2] += 8;
        }

        xdvk::printVector(buffer, "buffer");
    }
    
    auto rotationSize(const uint32_t dimension) -> size_t {
        return dimension * (dimension - 1) / 2;
    }

    auto transformSize(const uint32_t dimension) -> uint32_t {
        return 2 * dimension + static_cast<uint32_t>(rotationSize(dimension));
    }
}

/*
             0,  8,
             0, 10,
             0, 12,
             0, 14,
             0, 16,
             0, 18,
             0, 20,
             0, 22,
             1,  9,
             1, 11,
             1, 13,
             1, 15,
             1, 17,
             1, 19,
             1, 21,
             1, 23,

             2,  8,
             2,  9,
             2, 12,
             2, 13,
             2, 16,
             2, 17,
             2, 20,
             2, 21,
             3, 10,
             3, 11,
             3, 14,
             3, 15,
             3, 18,
             3, 19,
             3, 22,
             3, 23,

             4,  8,
             4,  9,
             4, 10,
             4, 11,
             4, 16,
             4, 17,
             4, 18,
             4, 19,
             5, 12,
             5, 13,
             5, 14,
             5, 15,
             5, 20,
             5, 21,
             5, 22,
             5, 23,

             6,  8,
             6,  9,
             6, 10,
             6, 11,
             6, 12,
             6, 13,
             6, 14,
             6, 15,
             7, 16,
             7, 17,
             7, 18,
             7, 19,
             7, 20,
             7, 21,
             7, 22,
             7, 23,

             8,  9,  
            10, 11,
            12, 13,
            14, 15,
            16, 17,
            18, 19,
            20, 21,
            22, 23,
             8, 10,
             9, 11, 
            12, 14,
            13, 15,
            16, 18,
            17, 19,
            20, 22,
            21, 23,
             8, 12,
             9, 13,
            10, 14,
            11, 15,
            16, 20,
            17, 21,
            18, 22,
            19, 23,
             8, 16, 
             9, 17,
            10, 18,
            11, 19,
            12, 20,
            13, 21,
            14, 22,
            15, 23
*/
