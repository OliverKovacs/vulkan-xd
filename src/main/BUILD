load("@bazel_skylib//rules:common_settings.bzl", "string_flag")

string_flag(
    name = "mode",
    build_setting_default = "",
)

config_setting(
    name = "profile_mode",
    flag_values = {"mode": "profiler"},
)

cc_library(
    name = "config",
    srcs = ["config.hpp"],
    hdrs = ["config.hpp"],
)

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
        ":vertex",
    ]
)

cc_library(
    name = "vulkan",
    srcs = ["vulkan.cpp"],
    hdrs = ["vulkan.cpp"],
    deps = [
        ":config",
        ":vertex",
        ":xdvk",
        "//src/include:stb",
    ],
    data = [
        "//src/assets:assets",
        "//src/shaders:shaders",
    ]
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    linkopts = [
        "-lvulkan",
        "-lglfw",
        "-ldl",
    ],
    deps = [
        ":vulkan",
        ":keybinds"
    ],
    local_defines = select({
        "profile_mode": ["PROFILER"],
        "//conditions:default": [],
    }),
)

sh_binary(
    name = "profiler",
    srcs = ["profiler.sh"],
    data = [":main"],
    visibility = ["//visibility:public"],
)
