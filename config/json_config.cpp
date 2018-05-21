#include "json_config.h"
#include "../base/json_util/json_util.h"
#include "../base/misc/string_helper/string_helper.h"
namespace easy
{
	namespace config
	{
		void json_config::load_config(const string& config_file)
		{
			m_jobj = json_util::parse_file(config_file);
		}

		const Json::Value& json_config::get_jobj() const
		{
			return m_jobj;
		}
	}
}


