#pragma once

#define MY_APPNAME "PDFium Reader"

#define MY_VERSION_MAJOR 1
#define MY_VERSION_MINOR 0
#define MY_VERSION_PATCH 0

/*
 * Version integer format: (major << 16) | (minor << 8) | patch
 * \code{.cpp}
 * if (MY_VERSION <= MY_VERSION_CHECK(1, 0, 0)) {
 *   // ...
 * }
 * \endcode
 */
#define MY_VERSION_CHECK(major, minor, patch) ((major << 16) | (minor << 8) | (patch))
#define MY_VERSION MY_VERSION_CHECK(MY_VERSION_MAJOR, MY_VERSION_MINOR, MY_VERSION_PATCH)

/* Version string format: "X.Y.Z" */
#define MY_VERSION_STR (MY_STRINGIFY(MY_VERSION_MAJOR.MY_VERSION_MINOR.MY_VERSION_PATCH))
