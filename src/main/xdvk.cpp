// Oliver Kovacs 2021 - xdvk - MIT

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "vertex.hpp"
#include "xdvk.hpp"

#define N_EDGE 2

namespace xdvk {
    
    void hypercubeVertices(std::vector<float> &vertices, const uint32_t dimension, float size, uint32_t stride, uint32_t offset) {
        const uint32_t block = 1 + stride;
        const uint32_t n1 = pow(2, dimension);
        const uint32_t n2 = dimension;
        vertices.resize(offset + n1 * n2 * block);

        // { ±1, ..., ±1 } → dimension^2 vertices
        for (size_t i = 0; i < n1; i++) {
            for (size_t j = 0; j < n2; j++) {
                const uint32_t index = offset + (i * n2 + j) * block;
                vertices[index] = - size * (1.0 - (static_cast<uint32_t>(floor(i / pow(2.0, j))) % 2) * 2.0);
            }
        }
    }

    void hypercubeIndices(std::vector<uint32_t> &buffer, const uint32_t dimension, uint32_t stride, uint32_t offset) {
        const uint32_t block = N_EDGE + stride;
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
        const uint32_t n_block = 1 + stride;
        const uint32_t n1 = 8;
        const uint32_t n2 = 4;
        buffer.resize(offset + 24 * 4 * n_block);

        // { ±1, 0, 0, 0 } → 8 vertices
        for (size_t i = 0; i < n1; i++) {
            for (size_t j = 0; j < n2; j++) {
                const uint32_t index = offset + (i * n2 + j) * n_block;
                buffer[index] = j == (i / 2) ? (static_cast<bool>(i % 2) ? size : -size) : 0.0F;
            }
        }

        const uint32_t n_filled = offset + n1 * n2 * n_block;
        
        // { ±0.5, ±0.5, ±0.5, ±0.5 } → 16 vertices
        hypercubeVertices(buffer, 4, 0.5 * size, stride, n_filled);
    }

    void icositetrachoronIndices(std::vector<uint32_t> &buffer, uint32_t stride, uint32_t offset) {
        const uint32_t N_EDGES = 96;
        const uint32_t n_block = N_EDGE + stride;
        const uint32_t n1 = 4;
        const uint32_t n2 = 2;
        const uint32_t n3 = 8;

        buffer.resize(offset + std::size_t{ N_EDGES * n_block });

        // 64 edges of first 8 vertices
        for (size_t i = 0; i < n1; i++) {
            for (size_t j = 0; j < n2; j++) {
                for (size_t k = 0; k < n3; k++) {
                    const uint32_t pow1 = (1 << i);
                    const uint32_t pow2 = (2 << i);
                    const uint32_t index = ((i * n2 + j) * n3 + k) * n_block;
                    buffer[index]     = i * n2 + j;
                    buffer[index + 1] = (k % pow1) + (k / pow1 * pow2) + 8 + j * pow1;
                }
            }
        }

        const uint32_t n_filled = n1 * n2 * n3 * n_block;
 
        // 32 edges of hypercube
        xdvk::hypercubeIndices(buffer, 4, stride, offset + n_filled);
        for (size_t i = 0; i < 32; i++) {
            for (size_t j = 0; j < 2; j++) {
                const uint32_t index = i * n_block + j + n_filled;
                buffer[index] += 8;
            }
        }
    }
    
    auto rotationSize(const uint32_t dimension) -> size_t {
        return dimension * (dimension - 1) / 2;
    }

    auto transformSize(const uint32_t dimension) -> uint32_t {
        return 2 * dimension + static_cast<uint32_t>(rotationSize(dimension));
    }
}

