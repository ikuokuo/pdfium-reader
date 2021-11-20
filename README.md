# PDFium Reader

## How to build

1. Get the code and its dependencies

    ```bash
    git clone --depth 1 https://github.com/ikuokuo/pdfium-reader.git
    cd pdfium-reader

    # get submodules
    git submodule update --init

    # get dependencies
    #  mac
    brew install glfw
    #  debian/ubuntu
    sudo apt install libglfw3-dev libgtk-3-dev
    ```

2. Get prebuilt PDFium libs to `third_party/pdfium-<linux|mac|win>-<x86|x64|arm|arm64>`

    - [Pre-compiled binaries of PDFium](https://github.com/bblanchon/pdfium-binaries)

3. Build the project using [CMake >= 3.10](https://cmake.org/download/)

    ```bash
    make clean; make
    ```

### Depends

- [PDFium](https://pdfium.googlesource.com/pdfium/): reading, parsing, and rasterizing the pages of PDF files
- [ImGui](https://github.com/ocornut/imgui): a bloat-free graphical user interface library for C++
- [GLFW](https://github.com/glfw/glfw): a multi-platform library for OpenGL, OpenGL ES and Vulkan development
- [GLM](https://github.com/g-truc/glm): a header only C++ mathematics library for OpenGL
- [stb](https://github.com/nothings/stb): single-file public domain libraries for C/C++
- [Native File Dialog Extended](https://github.com/btzy/nativefiledialog-extended.git): cross platform native file dialog library with C and C++ bindings

## About PDFium

- [docs/pdfium.md](docs/pdfium.md)

### Samples

- [pdfium_start](samples/pdfium_start.c): sample in [PDFium / Getting Started](https://pdfium.googlesource.com/pdfium/+/refs/heads/main/docs/getting-started.md)
- pdf/
  - [pdf_info](samples/pdf/pdf_info.cc): print pdf meta data, page infos
  - [pdf_render](samples/pdf/pdf_render.cc): render pdf pages to png files

## License

The project is [BSD 3-Clause](/LICENSE) licensed.
