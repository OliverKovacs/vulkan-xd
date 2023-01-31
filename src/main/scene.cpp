// #define SCENE_DEFAULT
// #define SCENE_HYPERCUBE
#define SCENE_TESSERACTS

#ifdef SCENE_DEFAULT

void Vulkan::createModel() {

    const auto hypercube_id = scene.add();
    auto &hypercube = scene.get(hypercube_id);

    const auto icositetrachoron_id = scene.add();
    auto &icositetrachoron = scene.get(icositetrachoron_id);

    std::vector<Vertex> vertices;

    std::vector<std::vector<uint32_t>> indices;
    indices.resize(scene.entities.size());
    xdvk::hypercubeIndices(indices[0], DIMENSION);
    xdvk::icositetrachoronIndices(indices[1]);

    const float size = 0.45;
    xdvk::hypercubeVertices(hypercube.geometry.vertices, DIMENSION, size);
    xdvk::icositetrachoronVertices(icositetrachoron.geometry.vertices, 2 * size);

    for (size_t i = 0; i < scene.entities.size(); i++) {
        Vertex vertex{ static_cast<glm::float32>(i) };
        vertices.resize(scene.entities[i].geometry.vertices.size());
        std::fill_n(vertices.begin(), vertices.size(), vertex);
        createVertexBuffer(vertices, scene.entities[i].geometry.vertexBuffer, scene.entities[i].geometry.vertexBufferMemory);
        createIndexBuffer(indices[i], scene.entities[i].geometry.indexBuffer, scene.entities[i].geometry.indexBufferMemory);
        scene.entities[i].geometry.indexBufferSize = indices[i].size();
    }

    storageVectors[1].resize(scene.entities.size() * xdvk::transformSize(DIMENSION));

    hypercube.transform.position[0] = -1.0;
    icositetrachoron.transform.position[0] =  1.0;

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
}

#endif /* SCENE_DEFAULT */

#ifdef SCENE_HYPERCUBE

void Vulkan::createModel() {

    const float size = 0.45;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    // hypercube entity
    const auto entity_id = scene.add();
    auto &entity = scene.get(entity_id);

    xdvk::hypercubeIndices(indices, DIMENSION);
    xdvk::hypercubeVertices(entity.geometry.vertices, DIMENSION, size);

    Vertex vertex{ static_cast<glm::float32>(0) };
    vertices.resize(entity.geometry.vertices.size());
    std::fill_n(vertices.begin(), vertices.size(), vertex);

    createVertexBuffer(vertices, entity.geometry.vertexBuffer, entity.geometry.vertexBufferMemory);
    createIndexBuffer(indices, entity.geometry.indexBuffer, entity.geometry.indexBufferMemory);

    entity.geometry.indexBufferSize = indices.size();
    
    // vertex ssbo data
    storageVectors[0].resize(entity.geometry.vertices.size());
    std::copy_n(entity.geometry.vertices.data(), entity.geometry.vertices.size(), &storageVectors[0][0]);
    entity.geometry.vertexBufferIndex = 0;

    // transform ssbo data
    storageVectors[1].resize(xdvk::transformSize(DIMENSION));
    std::copy_n(entity.transform.buffer, xdvk::transformSize(DIMENSION), &storageVectors[1][0]);
    entity.geometry.transformBufferIndex = 0;

    // ??
    storageVectors[2].resize(1);
}

#endif /* SCENE_HYPERCUBE */

#ifdef SCENE_TESSERACTS

void Vulkan::createModel() {

    const size_t n = 250;
    const float size = 0.05;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<float> vertex_data;

    xdvk::hypercubeIndices(indices, DIMENSION);
    xdvk::hypercubeVertices(vertex_data, DIMENSION, size);

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    createIndexBuffer(indices, indexBuffer, indexBufferMemory);
    
    vertices.resize(vertex_data.size());

    // vertex ssbo data
    storageVectors[0].resize(vertex_data.size());
    std::copy_n(vertex_data.data(), vertex_data.size(), &storageVectors[0][0]);

    // transform ssbo data
    storageVectors[1].resize(n * xdvk::transformSize(DIMENSION));

    for (size_t i = 0; i < n; i++) {
        const auto entity_id = scene.add();
        auto &entity = scene.get(entity_id);

        entity.geometry.vertices.resize(vertices.size());

        float x_range = 4.0;
        float y_range = 3.0;

        entity.transform.position[0] = x_range * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - (x_range / 2.0);
        entity.transform.position[1] = y_range * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - (y_range / 2.0);

        for (size_t j = 0; j < xdvk::rotationSize(DIMENSION); j++) {
            entity.transform.rotation[j] = 2.0 * M_PI * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        }

        Vertex vertex{ static_cast<glm::float32>(i) };
        std::fill_n(vertices.begin(), vertices.size(), vertex);

        createVertexBuffer(vertices, entity.geometry.vertexBuffer, entity.geometry.vertexBufferMemory);

        entity.geometry.indexBuffer = indexBuffer;
        entity.geometry.indexBufferMemory = indexBufferMemory;
        entity.geometry.indexBufferSize = indices.size();

        entity.geometry.vertexBufferIndex = 0;
        entity.geometry.transformBufferIndex = i * xdvk::transformSize(DIMENSION);

        std::copy_n(entity.transform.buffer, xdvk::transformSize(DIMENSION), &storageVectors[1][i * xdvk::transformSize(DIMENSION)]);
    }

    storageVectors[2].resize(1);
}

#endif /* SCENE_TESSERACTS */
