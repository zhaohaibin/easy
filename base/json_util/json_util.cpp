#include "json_util.h"
#include <json/reader.h>
#include "json/writer.h"
#include <fstream>
using namespace std;

json_util::json_util()
{
}


json_util::~json_util()
{
}

Json::Value json_util::parse(const std::string& str)
{
	Json::Value jobj;
	std::string error;
	Json::CharReader* pcharReader = Json::CharReaderBuilder().newCharReader();
	pcharReader->parse(str.data(), str.data() + str.size(), &jobj, &error);
	delete pcharReader;
	return jobj;
}

Json::Value json_util::parse_file(const std::string& file_path)
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

std::string json_util::to_string(const Json::Value& jobj)
{
	return Json::writeString(Json::StreamWriterBuilder(), jobj);
}
