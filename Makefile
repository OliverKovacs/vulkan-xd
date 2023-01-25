CXX = clang++
LIBS = -lvulkan -lglfw -ldl
CXXFLAGS = -Wall -std=c++20 -stdlib=libstdc++ $(LIBS)

GLSLC = glslc
BEAR_CMD = bear
MAKE_CMD = make

SRC_DIR = src
SRC_CPP_DIR = $(SRC_DIR)/main
SRC_ASSET_DIR = $(SRC_DIR)/assets
SRC_SHADER_DIR = $(SRC_DIR)/shaders

BUILD_DIR = build
ASSET_DIR = $(BUILD_DIR)/assets
SHADER_DIR = $(BUILD_DIR)/shaders
PROGRAM_NAME = main
PROGRAM_FULL = $(BUILD_DIR)/$(PROGRAM_NAME)

VERT_SHADER = $(SHADER_DIR)/shader.vert.spv
FRAG_SHADER = $(SHADER_DIR)/shader.frag.spv

COMPILATION_DB = compile_commands.json
CACHE = .cache

ALL = all

$(ALL): $(BUILD_DIR) assets shaders $(PROGRAM_FULL)

run: $(ALL)
	cd $(BUILD_DIR) && ./$(PROGRAM_NAME)

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

# $(wildcard $(SRC_CPP_DIR)/*.cpp)
$(PROGRAM_FULL): $(SRC_CPP_DIR)/main.cpp $(SRC_CPP_DIR)/xdvk.cpp $(SRC_CPP_DIR)/vertex.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

$(SHADER_DIR)/shader.%.spv: $(SRC_SHADER_DIR)/shader.%
	$(GLSLC) $^ -o $@

$(COMPILATION_DB): clean
	$(BEAR_CMD) -- $(MAKE_CMD) $(ALL)

clean:
	rm -rf $(BUILD_DIR) $(COMPILATION_DB) $(CACHE)

