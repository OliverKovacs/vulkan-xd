#include "vulkan.cpp"
#include "keybinds.cpp"

#include <algorithm>

void Vulkan::callback() {
    for (size_t i = 0; i < scene.entities.size(); i++) {
        auto &entity = scene.entities[i];
        for (size_t j = 0; j < xdvk::rotationSize(DIMENSION); j++) {
            entity.transform.rotation[j] += deltaTime * 0.1 * (j + 0.2);
        }
        bool shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
        bool alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
        float sign = (1 - 2 * shift);
        size_t offset = alt = keybinds.rotations.size();
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

void Vulkan::createModel() {

    scene.add();
    scene.add();

    std::vector<Vertex> vertices;

    std::vector<std::vector<uint32_t>> indices;
    indices.resize(2);
    xdvk::hypercubeIndices(indices[0], DIMENSION, 0, 0);
    xdvk::icositetrachoronIndices(indices[1], 0, 0);

    xdvk::hypercubeVertices(scene.entities[0].geometry.vertices, DIMENSION, 0.3, 0, 0);
    xdvk::icositetrachoronVertices(scene.entities[1].geometry.vertices, 0.6, 0, 0);

    for (size_t i = 0; i < scene.entities.size(); i++) {
        Vertex vertex{ static_cast<glm::float32>(i) };
        vertices.resize(scene.entities[i].geometry.vertices.size());
        std::fill_n(vertices.begin(), vertices.size(), vertex);
        createVertexBuffer(vertices, scene.entities[i].geometry.vertexBuffer, scene.entities[i].geometry.vertexBufferMemory);
        createIndexBuffer(indices[i], scene.entities[i].geometry.indexBuffer, scene.entities[i].geometry.indexBufferMemory);
        scene.entities[i].geometry.indexBufferSize = indices[i].size();
    }

    // xdvk::printVector(scene.entities[1].geometry.vertices, "vertices");
    // xdvk::printVector(indices[1], "indices");

    // scene.entities[1].geometry.vertices = {
    //     -1, -1,  0,  0, -1,  1,  0,  0,  1, -1,  0,  0,
    //      1,  1,  0,  0,  1, -1,  0,  0, -1,  1,  0,  0
    // };
    // scene.entities[1].geometry.drawIndexed = false;

    storageVectors[1].resize(scene.entities.size() * xdvk::transformSize(DIMENSION));

    scene.entities[0].transform.position[0] = -0.6;
    scene.entities[1].transform.position[0] =  0.6;

    size_t index = 0;
    for (size_t i = 0; i < scene.entities.size(); i++) {
        auto &entity = scene.entities[i];
        entity.geometry.vertexBufferIndex = index;
        size_t size = entity.geometry.vertices.size();
        storageVectors[0].resize(index + size);
        std::copy_n(entity.geometry.vertices.data(), size, &storageVectors[0][index]);
        index += size;

        std::copy_n(scene.entities[i].transform.buffer, size, &storageVectors[1][i * xdvk::transformSize(DIMENSION)]);
        entity.geometry.transformBufferIndex = i * xdvk::transformSize(DIMENSION);
    }

    storageVectors[2].resize(1);

    // xdvk::printVector(storageVectors[0], "storageVectors0");
}

int main() {
    Vulkan app;

    try {
        app.run();
    }
    catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
