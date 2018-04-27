#include "controller.h"
#include "response.h"
#include "http_header.h"
#include "base\string\string_helper\string_helper.h"
#include "civetweb.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>

controller::controller(const string& uri, http_method method) : m_uri(uri)
, m_http_method(method)
{
}

controller::~controller()
{
}

int controller::handler(struct mg_connection *conn)
{
	response rp(conn, "content-type application/json");
	rp.set_data(string_helper::wstring_to_utf8(L"{\"name\":\"’‘∫£±Ú\"}"));
	rp.perform();
	return 0;
}

std::string controller::get_uri()
{
	return m_uri;
}

http_method controller::get_http_method()
{
	return m_http_method;
}

std::map<std::string, string> controller::get_query_params(struct mg_connection *conn)
{
	map<string, string> query_params;
	const mg_request_info* p_mg_request_info = mg_get_request_info(conn);
	const char *p_query_string = p_mg_request_info->query_string;
	if (p_query_string != nullptr)
	{
		string str;
		str.append(p_query_string);
		vector<string> name_value_pair;
		boost::split(name_value_pair, str, boost::is_any_of("&"));
		for (size_t i = 0; i < name_value_pair.size(); ++i)
		{
			vector<string> name_value;
			boost::split(name_value, name_value_pair[i], boost::is_any_of("="));

			if (name_value.size() == 2)
				query_params.insert(std::make_pair(name_value[0], name_value[1]));
			else if (name_value.size() == 1)
				query_params.insert(std::make_pair(name_value[0], ""));
		}
	}
	return query_params;
}

