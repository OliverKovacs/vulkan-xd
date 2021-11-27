genrule(
    name = "assets",
    tools = [],
    srcs = [
        "asset.png",
    ],
    outs = [
        "texture.png"
    ],
    cmd = "cp $(location asset.png) $(location texture.png)",
    visibility = ["//visibility:public"],
)
