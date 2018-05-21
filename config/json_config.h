#ifndef _JSON_CONFIG_H
#define _JSON_CONFIG_H

#include <string>
#include "json/json.h"
#include "../base/misc/string_helper/string_helper.h"
using namespace std;
namespace easy
{
	namespace config
	{
		class json_config
		{
		public:
			void load_config(const string& config_file);
			const Json::Value& get_jobj() const;
		private:
			Json::Value m_jobj;
		};

		template<typename T>
		T json_config_get(const string path, const json_config& config)
		{
			const Json::Value& config_jobj = config.get_jobj();
			vector<string> vec = string_helper::split(path, ".");
			Json::Value jobj = config_jobj;
			for (size_t index = 0; index < vec.size() - 1; ++index)
			{
				if (jobj.isMember(vec[index]) == false)
				{
					string msg = "not find path:" + path;
					throw std::exception(msg.c_str());
				}
				jobj = jobj[vec[index]];
			}
			return json_util::get<T>(jobj, vec[vec.size() - 1]);
		}
	}
}

#endif //_JSON_CONFIG_H
