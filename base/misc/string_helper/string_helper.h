#ifndef _STRING_HELPER_H
#define _STRING_HELPER_H

#include <string>
#include <locale>
#include <cstdint>
#include <vector>
namespace string_helper
{
	std::string wstring_to_utf8(const std::wstring& str);
	std::string wstring_to_ansi(const std::wstring& str);
	std::wstring utf8_to_wstring(const std::string& str);
	std::wstring ansi_to_wstring(const std::string& str);
	std::wstring lower_string(const std::wstring& str);
	std::string string_To_UTF8(const std::string & str);
	std::string replace(std::string& input, const std::string& target, const std::string& curent);
	std::vector<std::string> split(const std::string& str, const std::string& sc);
}
#endif //_STRING_HELPER_H