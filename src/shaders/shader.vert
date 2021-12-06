#version 450

#define pi 3.1415926535

layout(constant_id = 0) const int n = 3;
const int a_n = n * (n - 1) / 2;
const int t_n = 2 * n + a_n;

layout(push_constant) uniform Constants {
    float entity;
    float vertexIndex;
    float indexIndex;
    float transformIndex;
} constants;

layout(binding = 0) uniform UniformBufferObject {
    vec2 res;
    float time;
} ubo;

layout(binding = 2) readonly buffer StorageBuffer {
    float vertices[];
} ssbo;

layout(binding = 3) readonly buffer StorageBuffer2 {
    float transforms[];
} ssbo2;

layout(location = 0) in float entity;

layout(location = 0) out vec4 fragColor;

struct Transform {
    float position[n];
    float scale[n];
    float rotation[a_n];
};

vec3 xy_scale = vec3(1.0, 1.0, 1.0);

float canvas_z = 4.0;
float camera_z = 0.0;

void fetchVertex(inout float[n] vertex, int stride, int offset) {
    int block = n + stride;
    int index = offset + gl_VertexIndex * block;
    for (int i = 0; i < n; i++) {
        vertex[i] = ssbo.vertices[index + i];
    }
}

void fetchTransform(inout Transform transform, int transform_index, int stride, int offset) {
    int block = t_n + stride;
    int index = transform_index;
    for (int i = 0; i < n; i++) {
        transform.position[i] = ssbo2.transforms[index + i];
        transform.scale[i] = ssbo2.transforms[index + n + i];
    }
    for (int i = 0; i < a_n; i++) {
        transform.rotation[i] = ssbo2.transforms[index + 2 * n + i];
    }
}

void scaleVertex(inout float vertex[n], inout float scale[n]) {
    for (int i = 0; i < n; i++) {
        vertex[i] *= scale[i];
    }
}

void rotateVertex(inout float vertex[n], inout float rotation[a_n], inout float scale[n]) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n; j++) {
            if (j <= i) continue;
            const float a = rotation[a_n - int(float((n - i - 1) * (n - i)) / 2.0) + j - i - 1];
            const float cos_a = cos(a);
            const float sin_a = sin(a);
            const float vi = vertex[i];
            const float vj = vertex[j];
            vertex[i] = vi *  cos_a + vj * sin_a;
            vertex[j] = vi * -sin_a + vj * cos_a;
        }
    }
}

void translateVertex(inout float vertex[n], inout float position[n]) {
    for (int i = 0; i < n; i++) {
        vertex[i] += position[i];
    }
}

void transformVertex(inout float vertex[n], Transform transform) {
    scaleVertex(vertex, transform.scale);
    rotateVertex(vertex, transform.rotation, transform.scale);
    translateVertex(vertex, transform.position);
}

void projectVertex(inout float vertex[n]) {
    float z_diff = canvas_z - camera_z;
    for (int i = n - 1; i >= 2; i--) {
        float w = z_diff / (canvas_z - vertex[i]);
        for (int j = 0; j < n; j++) {
            if (j >= i) break;
            vertex[j] *= w;
        }
    }
}

vec3 arrayToVec3(float[n] array) {
    return vec3(array[0], array[1], array[2]);
}

void main() {
    if (ubo.res.x > ubo.res.y) xy_scale = vec3(ubo.res.y / ubo.res.x, 1.0, 1.0);
    else xy_scale = vec3(1.0, ubo.res.x / ubo.res.y, 1.0);

    float[n] vertex;
    Transform transform;
    fetchVertex(vertex, 0, int(constants.vertexIndex));
    fetchTransform(transform, int(constants.transformIndex), 0, 0);
    transformVertex(vertex, transform);
    projectVertex(vertex);

    vec3 pos = arrayToVec3(vertex) * vec3(0.5, 0.5, 0.5);

    pos *= xy_scale;
    gl_Position = vec4(pos.xy, 0.0, 1.0);

    float rgb_s = 2.0;
    vec4 color = vec4(
        sin(gl_VertexIndex * rgb_s + ubo.time + 0 * pi / 3),
        sin(gl_VertexIndex * rgb_s + ubo.time + 2 * pi / 3),
        sin(gl_VertexIndex * rgb_s + ubo.time + 4 * pi / 3),
        1.0
    );

    fragColor = color;
}
