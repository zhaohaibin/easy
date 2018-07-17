#include "string_helper.h"
#include <codecvt>
#include <algorithm>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <regex> 
#include <windows.h>
namespace string_helper
{
	std::string wstring_to_utf8( const std::wstring& str )
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>,wchar_t> conv;
		return conv.to_bytes(str);
	}

	std::string wstring_to_ansi(const std::wstring& str)
	{
		int len = WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.size(), NULL,0,NULL,FALSE);
		char* p_buffer = new char[len];
		int ret = WideCharToMultiByte(CP_ACP, NULL, str.c_str(), str.size(), p_buffer, len, NULL, FALSE);
		if (ret < 0)
		{
			delete[]p_buffer;
			unsigned long error_code = GetLastError();
			std::string msg;
			switch (error_code)
			{
			case ERROR_INSUFFICIENT_BUFFER:
				msg = "ERROR_INSUFFICIENT_BUFFER";
				break;
			case ERROR_INVALID_FLAGS:
				msg = "ERROR_INVALID_FLAGS";
				break;
			case ERROR_INVALID_PARAMETER:
				msg = "ERROR_INVALID_PARAMETER";
				break;
			case ERROR_NO_UNICODE_TRANSLATION:
				msg = "ERROR_NO_UNICODE_TRANSLATION";
				break;
			default:
				break;
			}
			throw std::exception(msg.c_str(), error_code);
		}
		std::string ansi_string;
		ansi_string.append(p_buffer, len);
		delete[]p_buffer;
		return ansi_string;
	}

	std::wstring utf8_to_wstring(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>,wchar_t> conv;
		return conv.from_bytes(str.c_str());
	}

	/*
	CodePage：指定执行转换的多字节字符所使用的字符集
	这个参数可以为系统已安装或有效的任何字符集所给定的值。你也可以指定其为下面的任意一值：
	Value	Description
	CP_ACP	ANSI code page
	CP_MACCP	Not supported
	CP_OEMCP	OEM code page
	CP_SYMBOL	Not supported
	CP_THREAD_ACP	Not supported
	CP_UTF7	UTF-7 code page
	CP_UTF8	UTF-8 code page
	*/
	std::wstring ansi_to_wstring(const std::string& str)
	{
		int nw_len = MultiByteToWideChar(CP_ACP, 0,  str.c_str(), str.size(), NULL, 0);
		wchar_t * buffer = new wchar_t[nw_len];
		int ret = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, nw_len);
		if (ret < 0)
		{
			delete []buffer;
			unsigned long error_code = GetLastError();
			std::string msg;
			switch (error_code)
			{
			case ERROR_INSUFFICIENT_BUFFER:
				msg = "ERROR_INSUFFICIENT_BUFFER";
				break;
			case ERROR_INVALID_FLAGS:
				msg = "ERROR_INVALID_FLAGS";
				break;
			case ERROR_INVALID_PARAMETER:
				msg = "ERROR_INVALID_PARAMETER";
				break;
			case ERROR_NO_UNICODE_TRANSLATION:
				msg = "ERROR_NO_UNICODE_TRANSLATION";
				break;
			default:
				break;
			}
			throw std::exception(msg.c_str(), error_code);
		}
		std::wstring result;
		result.append(buffer, nw_len);
		delete[]buffer;
		return result;
	}

	std::wstring lower_string(const std::wstring& str)
	{
		std::wstring lowerStr = str;
		std::transform(lowerStr.begin(), lowerStr.end(),lowerStr.begin(),towlower);
		return lowerStr;
	}

	std::string string_To_UTF8(const std::string & str)
	{
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

		wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴  
		ZeroMemory(pwBuf, nwLen * 2 + 2);

		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

		char * pBuf = new char[nLen + 1];
		ZeroMemory(pBuf, nLen + 1);

		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		std::string retStr(pBuf);

		delete[]pwBuf;
		delete[]pBuf;

		pwBuf = NULL;
		pBuf = NULL;

		return retStr;
	}

	std::string replace(std::string& input, const std::string& target, const std::string& curent)
	{
		return std::regex_replace(input, std::regex(target), curent);
	}

	std::vector<std::string> split(const std::string& str, const std::string& sc)
	{
		std::vector<std::string> vec;
		boost::split(vec, str, boost::is_any_of(sc));
		return vec;
	}

}