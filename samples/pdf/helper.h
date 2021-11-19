#pragma once

#include <string>

namespace fpdf {

void PrintLastError();

// convert "D:YYYYMMDDHHmmSSOHH'mm'" to "YYYY-MM-DDTHH:mm:ss+HH:mm"
std::string DateToRFC3399(const std::string &str);

}  // namespace fpdf

namespace strings {

std::string FromUtf16(const std::u16string &u16str);

bool EndsWith(const std::string &str, const std::string &suffix);
bool StartsWith(const std::string &str, const std::string &prefix);

std::size_t ReplaceAll(std::string &str,
    const std::string &what, const std::string &with);

}  // namespace strings
