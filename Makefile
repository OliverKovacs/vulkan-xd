CXX = clang++
LIBS = -lvulkan -lglfw -ldl
CXXFLAGS = -Wall -std=c++20 -stdlib=libstdc++ $(LIBS)

debug:      CXXFLAGS    += -g
release:    CXXFLAGS    += -O3

GLSLC = glslc
BEAR_CMD = bear
MAKE_CMD = make

SRC_DIR = src
SRC_CPP_DIR = $(SRC_DIR)/main
SRC_ASSET_DIR = $(SRC_DIR)/assets
SRC_SHADER_DIR = $(SRC_DIR)/shaders
SRCS = $(SRC_CPP_DIR)/main.cpp $(SRC_CPP_DIR)/xdvk.cpp $(SRC_CPP_DIR)/vertex.cpp

BUILD_DIR = build
ASSET_DIR = $(BUILD_DIR)/assets
SHADER_DIR = $(BUILD_DIR)/shaders
PROGRAM_NAME = main
PROGRAM_FULL = $(BUILD_DIR)/$(PROGRAM_NAME)

VERT_SHADER = $(SHADER_DIR)/shader.vert.spv
FRAG_SHADER = $(SHADER_DIR)/shader.frag.spv

PROFILER_ENV = VK_INSTANCE_LAYERS=VK_LAYER_MESA_overlay VK_LAYER_MESA_OVERLAY_CONFIG=vertices=1,fps_sampling_period=10000

COMPILATION_DB = compile_commands.json
CACHE = .cache

ALL = all

$(ALL): $(BUILD_DIR) assets shaders $(PROGRAM_FULL)

debug: $(ALL)
release: $(ALL)

run: $(ALL)
	cd $(BUILD_DIR) && ./$(PROGRAM_NAME)

profile: release
	cd $(BUILD_DIR) && $(PROFILER_ENV) ./$(PROGRAM_NAME)

assets: $(ASSET_DIR) asset_files

asset_files: $(wildcard $(SRC_ASSET_DIR)/*)
	cp -t $(ASSET_DIR)/ $^

shaders: $(SHADER_DIR) $(VERT_SHADER) $(FRAG_SHADER)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(ASSET_DIR): $(BUILD_DIR)
	mkdir -p $(ASSET_DIR)

$(SHADER_DIR): $(BUILD_DIR)
	mkdir -p $(SHADER_DIR)

$(PROGRAM_FULL): $(wildcard $(SRC_CPP_DIR)/*.cpp) $(wildcard $(SRC_CPP_DIR)/*.hpp)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

$(SHADER_DIR)/shader.%.spv: $(SRC_SHADER_DIR)/shader.%
	$(GLSLC) $^ -o $@

$(COMPILATION_DB): clean
	$(BEAR_CMD) -- $(MAKE_CMD) $(ALL)

clean:
	rm -rf $(BUILD_DIR) $(COMPILATION_DB) $(CACHE)

