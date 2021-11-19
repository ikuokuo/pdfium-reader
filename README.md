# PDFium Reader

## How to build

1. Get prebuilt PDFium libs to `third_party/pdfium-<linux|mac|win>-<x86|x64|arm|arm64>`

    - [Pre-compiled binaries of PDFium](https://github.com/bblanchon/pdfium-binaries)

2. Build using [CMake >= 3.10](https://cmake.org/download/)

    ```bash
    git clone --depth 1 https://github.com/ikuokuo/pdfium-reader.git
    cd pdfium-reader
    make clean; make
    ```

## About PDFium

- [docs/pdfium.md](docs/pdfium.md)

### Samples

- [pdfium_start](samples/pdfium_start.c): sample in [PDFium / Getting Started](https://pdfium.googlesource.com/pdfium/+/refs/heads/main/docs/getting-started.md)
- pdf/
  - [pdf_info](samples/pdf/pdf_info.cc): print pdf meta data, page infos
  - [pdf_render](samples/pdf/pdf_render.cc): render pdf pages to png files

## License

The project is [BSD 3-Clause](/LICENSE) licensed.
