#ifndef _STRING_HELPER_H
#define _STRING_HELPER_H

#include <string>
#include <locale>
#include <cstdint>

namespace string_helper
{
	std::string wstring_to_utf8(const std::wstring& str);
	std::wstring utf8_to_wstring(const std::string& str);

	std::wstring lower_string(const std::wstring& str);
}
#endif //_STRING_HELPER_H