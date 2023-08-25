#include <chrono>

#include "fpdf_edit.h"
#include "fpdf_scopers.h"
#include "fpdfview.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "helper.h"

void PdfRender(const std::string &pdf_name, FPDF_DOCUMENT doc);
void PdfRenderPage(const std::string &pdf_name, FPDF_DOCUMENT doc, int index);
bool PdfWritePng(const std::string &img_name, void *buffer,
                 int width, int height, int stride);

int main(int argc, char const *argv[]) {
  FPDF_STRING test_doc = "test_doc.pdf";
  if (argc >= 2) {
    test_doc = argv[1];
  }
  fprintf(stdout, "test_doc: %s\n", test_doc);

  FPDF_LIBRARY_CONFIG config;
  config.version = 3;
  config.m_pUserFontPaths = NULL;
  config.m_pIsolate = NULL;
  config.m_v8EmbedderSlot = 0;
  config.m_pPlatform = NULL;
  config.m_pUserFontPaths = NULL;

  FPDF_InitLibraryWithConfig(&config);

  FPDF_DOCUMENT doc = FPDF_LoadDocument(test_doc, NULL);
  if (!doc) {
    fpdf::PrintLastError();
    goto EXIT;
  }

  PdfRender(test_doc, doc);

  FPDF_CloseDocument(doc);
EXIT:
  FPDF_DestroyLibrary();
  return 0;
}

class Timer {
 public:
  using clock = std::chrono::system_clock;
  Timer() { Start(); }
  ~Timer() = default;
  void Start() {
    time_start_ = clock::now();
  }
  int64_t Elapsed(bool restart = true) {
    using namespace std::chrono;  // NOLINT
    auto d = duration_cast<milliseconds>(clock::now() - time_start_).count();
    if (restart) Start();
    return d;
  }
 private:
  clock::time_point time_start_;
};

void PdfRender(const std::string &pdf_name, FPDF_DOCUMENT doc) {
  int page_count = FPDF_GetPageCount(doc);
  for (int i = 0; i < page_count; ++i) {
    PdfRenderPage(pdf_name, doc, i);
  }
}

void PdfRenderPage(const std::string &pdf_name, FPDF_DOCUMENT doc, int index) {
  Timer t;

  FPDF_PAGE page = FPDF_LoadPage(doc, index);

  double scale = 1.0;
  // double scale = 2.0;
  int width = static_cast<int>(FPDF_GetPageWidth(page) * scale);
  int height = static_cast<int>(FPDF_GetPageHeight(page) * scale);
  int alpha = FPDFPage_HasTransparency(page) ? 1 : 0;
  ScopedFPDFBitmap bitmap(FPDFBitmap_Create(width, height, alpha));  // BGRx

  if (bitmap) {
    FPDF_DWORD fill_color = alpha ? 0x00000000 : 0xFFFFFFFF;
    FPDFBitmap_FillRect(bitmap.get(), 0, 0, width, height, fill_color);

    int rotation = 0;
    int flags = FPDF_ANNOT;
    FPDF_RenderPageBitmap(bitmap.get(), page, 0, 0, width, height,
        rotation, flags);
    auto t_render = t.Elapsed();

    int stride = FPDFBitmap_GetStride(bitmap.get());
    void *buffer = FPDFBitmap_GetBuffer(bitmap.get());

    char img_name[256];
    int chars_formatted = snprintf(
        img_name, sizeof(img_name), "%s.%d.png", pdf_name.c_str(), index);
    if (chars_formatted < 0 ||
        static_cast<size_t>(chars_formatted) >= sizeof(img_name)) {
      fprintf(stderr, "Filename is too long: %s\n", img_name);
      exit(EXIT_FAILURE);
    }

    auto ok = PdfWritePng(img_name, buffer, width, height, stride);
    if (!ok) {
      fprintf(stderr, "Write png failed: %s\n", img_name);
      exit(EXIT_FAILURE);
    }
    auto t_write = t.Elapsed();

    fprintf(stdout, "%s\n", img_name);
    fprintf(stdout, " %02d: %dx%d, render=%ldms, write=%ldms\n",
        index, width, height, t_render, t_write);
  } else {
    fprintf(stderr, "Page was too large to be rendered.\n");
    exit(EXIT_FAILURE);
  }

  FPDF_ClosePage(page);
}

bool PdfWritePng(const std::string &img_name, void *buffer,
                 int width, int height, int stride) {
  // BGRA > RGBA
  auto buf = reinterpret_cast<uint8_t *>(buffer);
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      auto pixel = buf + (r*stride) + (c*4);
      auto b = pixel[0];
      pixel[0] = pixel[2];  // b = r
      pixel[2] = b;         // r = b
    }
  }
  return stbi_write_png(img_name.c_str(), width, height, 4, buf, stride) != 0;
}
