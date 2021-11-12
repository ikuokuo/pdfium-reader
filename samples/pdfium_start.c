#include <fpdfview.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  FPDF_STRING test_doc = "test_doc.pdf";
  if (argc >= 2) {
    test_doc = argv[1];
  }
  printf("test_doc: %s\n", test_doc);

  FPDF_LIBRARY_CONFIG config;
  config.version = 2;
  config.m_pUserFontPaths = NULL;
  config.m_pIsolate = NULL;
  config.m_v8EmbedderSlot = 0;

  FPDF_InitLibraryWithConfig(&config);

  FPDF_DOCUMENT doc = FPDF_LoadDocument(test_doc, NULL);
  if (!doc) {
    unsigned long err = FPDF_GetLastError();
    fprintf(stderr, "Load pdf docs unsuccessful: ");
    switch (err) {
      case FPDF_ERR_SUCCESS:
        fprintf(stderr, "Success");
        break;
      case FPDF_ERR_UNKNOWN:
        fprintf(stderr, "Unknown error");
        break;
      case FPDF_ERR_FILE:
        fprintf(stderr, "File not found or could not be opened");
        break;
      case FPDF_ERR_FORMAT:
        fprintf(stderr, "File not in PDF format or corrupted");
        break;
      case FPDF_ERR_PASSWORD:
        fprintf(stderr, "Password required or incorrect password");
        break;
      case FPDF_ERR_SECURITY:
        fprintf(stderr, "Unsupported security scheme");
        break;
      case FPDF_ERR_PAGE:
        fprintf(stderr, "Page not found or content error");
        break;
      default:
        fprintf(stderr, "Unknown error %ld", err);
    }
    fprintf(stderr, ".\n");
    goto EXIT;
  }

  FPDF_CloseDocument(doc);
EXIT:
  FPDF_DestroyLibrary();
  return 0;
}
