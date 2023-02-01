# vulkan-xd

[![Open in Visual Studio Code](https://open.vscode.dev/badges/open-in-vscode.svg)](https://open.vscode.dev/OliverKovacs/vulkan-xd)
[![License](https://img.shields.io/github/license/OliverKovacs/vulkan-xd)](https://github.com/OliverKovacs/vulkan-xd/blob/main/LICENSE.md)
[![Size](https://img.shields.io/github/repo-size/OliverKovacs/vulkan-xd)]()

Higher-dimensional renderer implemented in [Vulkan](https://www.vulkan.org/). This is mainly a research project. 

Currently only Ubuntu is actively supported, however you might be able to run it on other Linux distributions.

Documentation will be available in the future.

## Dependencies
- [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/)
- [GLFW](https://www.glfw.org/)
- [GLM](https://github.com/g-truc/glm)
- Some other platform specific headers
- [Git](https://git-scm.com/), [Make](https://www.gnu.org/software/make/), [Clang](https://clang.llvm.org/) 

## Linux

### Ubuntu

Install these dependencies:  
[Vulkan SDK](https://packages.lunarg.com/#)  
Other packages:

```bash
sudo apt update && sudo apt install mesa-common-dev libglfw3 libglfw3-dev libglm-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxxf86vm-dev git clang
```

Download the project:
```bash
git clone https://github.com/OliverKovacs/vulkan-xd && cd vulkan-xd && git submodule update --recursive --init
```

Build the project:
```bash
make all
```

Run:
```bash
make run
```

### Other distros

Repeat the steps above according to the distribution you are using. You will have to find out which packages you need to install to access the required headers. Download links:
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#linux)
- [GLFW](https://www.glfw.org/download)
- [GLM](https://github.com/g-truc/glm)
- [Git](https://git-scm.com/download/linux)

## Windows
Windows is currently not supported, altough it could still work if you somehow manage to get all the dependencies working.

## Acknowledgements
The basic Vulkan boilerplate code is largely based on [Alexander Overvoorde](https://github.com/Overv)'s [Vulkan Tutorial](https://vulkan-tutorial.com/).

## Author
[Oliver Kovacs](https://github.com/OliverKovacs)

## License
MIT
