#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "fpdf_attachment.h"
#include "fpdf_edit.h"
#include "fpdf_doc.h"
#include "fpdf_text.h"
#include "fpdfview.h"

#include "helper.h"

void PrintPdfMetaData(FPDF_DOCUMENT doc);
void PrintPdfAttachments(FPDF_DOCUMENT doc);
void PrintPdfPageInfos(FPDF_DOCUMENT doc);

int main(int argc, char const *argv[]) {
  FPDF_STRING test_doc = "test_doc.pdf";
  if (argc >= 2) {
    test_doc = argv[1];
  }
  std::cout << "test_doc: " << test_doc << std::endl;

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

  PrintPdfMetaData(doc);
  PrintPdfAttachments(doc);
  PrintPdfPageInfos(doc);

  FPDF_CloseDocument(doc);
EXIT:
  FPDF_DestroyLibrary();
  return 0;
}

void PrintPdfMetaData(FPDF_DOCUMENT doc) {
  std::cout << "== PrintPdfMetaData =============================" << std::endl;
  static constexpr const char *kMetaTags[] = {
      "Title",   "Author",   "Subject",      "Keywords",
      "Creator", "Producer", "CreationDate", "ModDate"};
  for (const char *meta_tag : kMetaTags) {
    const unsigned long len = FPDF_GetMetaText(doc, meta_tag, nullptr, 0);
    if (!len)
      continue;

    std::vector<char16_t> buf(len);
    FPDF_GetMetaText(doc, meta_tag, buf.data(), buf.size());
    auto text = strings::FromUtf16(std::u16string(buf.data()));
    if (strcmp(meta_tag, "CreationDate") == 0 ||
        strcmp(meta_tag, "ModDate") == 0) {
      text = fpdf::DateToRFC3399(text);
    }
    std::cout << " " << meta_tag << ": " << text << std::endl;
  }
  std::cout << "=================================================" << std::endl;
}

void PrintPdfAttachments(FPDF_DOCUMENT doc) {
  std::cout << "== PrintPdfAttachments ==========================" << std::endl;
  std::cout << " count: " << FPDFDoc_GetAttachmentCount(doc) << std::endl;
  std::cout << "=================================================" << std::endl;
}

void PrintPdfPageInfos(FPDF_DOCUMENT doc) {
  std::cout << "== PrintPdfPageInfos ============================" << std::endl;
  int page_count = FPDF_GetPageCount(doc);
  std::cout << " count: " << page_count << std::endl;
  for (int i = 0; i < page_count; ++i) {
    FPDF_PAGE page = FPDF_LoadPage(doc, i);
    std::cout << "-- page " << std::setw(2) << std::setfill('0') << i
        << " ------------------------------------" << std::endl;

    double page_width = FPDF_GetPageWidth(page);
    double page_height = FPDF_GetPageHeight(page);
    int page_alpha = FPDFPage_HasTransparency(page) ? 1 : 0;
    std::cout << "  w*h alpha: " << page_width << "x" << page_height
        << " " << page_alpha << std::endl;

    FPDF_TEXTPAGE page_text = FPDFText_LoadPage(page);
    int page_text_count = FPDFText_CountChars(page_text);
    std::cout << "  text count: " << page_text_count << std::endl;

    std::cout << "-----------------------------------------------" << std::endl;
    std::vector<unsigned short> buf(page_text_count+1);
    int text_len = FPDFText_GetText(page_text, 0, page_text_count, buf.data());
    std::cout << strings::FromUtf16(std::u16string(
        reinterpret_cast<char16_t *>(buf.data()), text_len-1)) << std::endl;

    FPDFText_ClosePage(page_text);
    FPDF_ClosePage(page);
  }
  std::cout << "=================================================" << std::endl;
}
