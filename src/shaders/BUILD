genrule(
    name = "shaders",
    tools = [],
    srcs = [
        "shader.frag",
        "shader.vert",
    ],
    outs = [
        "shader.frag.spv",
        "shader.vert.spv",
    ],
    cmd = "glslc $(location shader.frag) -o $(location shader.frag.spv) && glslc $(location shader.vert) -o $(location shader.vert.spv)",
    visibility = ["//visibility:public"],
)
