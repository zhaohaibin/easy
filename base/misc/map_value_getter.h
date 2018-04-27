#ifndef _MAP_VALUE_GETTER_H
#define _MAP_VALUE_GETTER_H

#include <map>
#include <boost/any.hpp>

namespace base
{
	namespace misc
	{
		template<typename T>
		bool get_val(const string& key, std::map<string, boost::any>& key_value, T& val)
		{
			std::map<string, boost::any>::iterator it = key_value.find(key);
			if (it == key_value.end())
				return false;
			val = boost::any_cast<T>(it->second);
			return true;
		}

		template<typename T>
		bool get_val(const string& key, std::map<string, string>& key_value, T& val)
		{
			std::map<string, string>::iterator it = key_value.find(key);
			if (it == key_value.end())
				return false;
			val = boost::lexical_cast<T>(it->second);
			return true;
		}
	}
}
#endif //_MAP_VALUE_GETTER_H
