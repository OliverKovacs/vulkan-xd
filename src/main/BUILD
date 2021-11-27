cc_library(
    name = "keybinds",
    srcs = ["keybinds.cpp"],
    hdrs = ["keybinds.cpp"],
)

cc_library(
    name = "vertex",
    srcs = ["vertex.cpp"],
    hdrs = ["vertex.cpp"],
)

cc_library(
    name = "xdvk",
    srcs = ["xdvk.cpp"],
    hdrs = ["xdvk.cpp"],
    deps = [
        ":vertex"
    ]
)

cc_library(
    name = "vulkan",
    srcs = ["vulkan.cpp"],
    hdrs = ["vulkan.cpp"],
    deps = [
        ":vertex",
        ":xdvk",
        "//src/include:stb",
    ],
    data = [
        "//src/assets:assets",
        "//src/shaders:shaders"
    ]
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    linkopts = [
        "-lvulkan",
        "-lglfw"
    ],
    deps = [
        ":vulkan",
        ":keybinds"
    ],
)
