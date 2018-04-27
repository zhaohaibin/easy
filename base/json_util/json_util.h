#pragma once

#include <json/value.h>

class json_util
{
public:
	json_util();
	~json_util();

	static Json::Value parse(const std::string& str);

	static Json::Value parse_file(const std::string& file_path);

	static std::string to_string(const Json::Value& jobj);
};

