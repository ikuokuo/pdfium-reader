# [PDFium][]

[PDFium]: https://pdfium.googlesource.com/pdfium/

## Start

```zsh
# get depot_tools, contains: gclient, ninja, gn, ...
git clone --depth 1 https://chromium.googlesource.com/chromium/tools/depot_tools.git
export PATH="$PATH:$HOME/Codes/Star/depot_tools"

# get pdfium
cd pdfium-reader/
mkdir -p third_party/chromium
cd third_party/chromium
gclient config --unmanaged https://pdfium.googlesource.com/pdfium.git
gclient sync
cd pdfium

# get deps
#  on linux, install additional build dependencies
./build/install-build-deps.sh

# gn config
#  args see the following `out/Release/args.gn`
gn args out/Release

# ninja build
#  pdfium
ninja -C out/Release pdfium
#  pdfium_test
ninja -C out/Release pdfium_test

# run sample: pdf > ppm
./out/Release/pdfium_test --ppm path/to/myfile.pdf
```

`out/Release/args.gn`:

```zsh
use_goma = false  # Googlers only. Make sure goma is installed and running first.
is_debug = false  # Enable debugging features.

# Set true to enable experimental Skia backend.
pdf_use_skia = false
# Set true to enable experimental Skia backend (paths only).
pdf_use_skia_paths = false

pdf_enable_xfa = false  # Set false to remove XFA support (implies JS support).
pdf_enable_v8 = false  # Set false to remove Javascript support.
pdf_is_standalone = true  # Set for a non-embedded build.
pdf_is_complete_lib = true  # Set for a static library build.
is_component_build = false  # Disable component build (Though it should work)
```

## See also

- [Pre-compiled binaries of PDFium](https://github.com/bblanchon/pdfium-binaries)
- [QtWebEngine / QtPdf](https://code.qt.io/cgit/qt/qtwebengine.git/tree/src)

## References

- [PDFium / README](https://pdfium.googlesource.com/pdfium/)
- [PDFium / Getting Started](https://pdfium.googlesource.com/pdfium/+/refs/heads/main/docs/getting-started.md)
