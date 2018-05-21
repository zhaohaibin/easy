#include "json_util.h"
#include <json/reader.h>
#include "json/writer.h"
#include <fstream>
using namespace std;

namespace json_util
{
	Json::Value parse(const std::string& str)
	{
		Json::Value jobj;
		std::string error;
		Json::CharReader* pcharReader = Json::CharReaderBuilder().newCharReader();
		pcharReader->parse(str.data(), str.data() + str.size(), &jobj, &error);
		delete pcharReader;
		return jobj;
	}

	Json::Value parse_file(const std::string& file_path)
	{
		ifstream json_file;
		json_file.open(file_path, ios::binary);
		Json::Value jobj;
		if (json_file.is_open())
		{
			std::string error;
			bool parse_ret = Json::parseFromStream(Json::CharReaderBuilder(), json_file, &jobj, &error);
		}
		return jobj;
	}

	std::string to_string(const Json::Value& jobj)
	{
		return Json::writeString(Json::StreamWriterBuilder(), jobj);
	}
}
