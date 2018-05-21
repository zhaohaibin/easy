#pragma once

#include <json/value.h>
#include <string>
#include "get_value.h"
using namespace std;

namespace json_util
{
	Json::Value parse(const std::string& str);

	Json::Value parse_file(const std::string& file_path);

	std::string to_string(const Json::Value& jobj);

	template<typename T>
	T get(const Json::Value& jobj, string& key)
	{
		return get_value<T>()(jobj, key);
	}
}

